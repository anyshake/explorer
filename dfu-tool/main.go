package main

import (
	"errors"
	"flag"
	"fmt"
	"os"
	"strings"
	"time"

	"github.com/pterm/pterm"

	"github.com/anyshake/explorer/dfu-tool/internal/autobaud"
	"github.com/anyshake/explorer/dfu-tool/internal/client"
	"github.com/anyshake/explorer/dfu-tool/internal/image"
	"github.com/anyshake/explorer/dfu-tool/internal/protocol"
	"github.com/anyshake/explorer/dfu-tool/internal/serialport"
	"github.com/anyshake/explorer/dfu-tool/internal/ui"
)

func main() {
	if err := run(); err != nil {
		ui.Error(err)
		os.Exit(1)
	}
}

func run() error {
	var (
		portName = flag.String("port", "", "serial port connected to the Explorer UART1 (e.g. /dev/ttyUSB0, COM3)")
		noEnter  = flag.Bool("no-enter", false, "do not send the DFU-entry command; wait for the bootloader instead (power-cycle the device to recover a broken firmware)")
		wait     = flag.Duration("wait", 10*time.Second, "how long to wait for the bootloader to answer")
		noBoot   = flag.Bool("no-boot", false, "stay in the bootloader afterwards")
		list     = flag.Bool("list", false, "list serial ports and exit")
		debug    = flag.Bool("debug", false, "dump every byte sent and received")
		noColor  = flag.Bool("no-color", false, "plain output without colors and animations")
		check    = flag.Bool("check", false, "only check that the device enters the bootloader, then start the firmware again; nothing is flashed")
	)
	flag.Usage = func() {
		pterm.Printfln("Usage: %s -port <port> [options] <firmware.bin>\n       %s -port <port> -check [options]\n\nOptions:", os.Args[0], os.Args[0])
		flag.CommandLine.SetOutput(os.Stdout)
		flag.PrintDefaults()
	}
	flag.Parse()

	if *noColor {
		pterm.DisableStyling()
	}
	if *debug {
		ui.EnableTrace()
	}

	if *list {
		ports, err := serialport.List()
		if err != nil {
			return err
		}
		ui.Ports(ports)
		return nil
	}
	wantArgs := 1
	if *check {
		wantArgs = 0
	}
	if *portName == "" || flag.NArg() != wantArgs {
		flag.Usage()
		os.Exit(2)
	}

	var img []byte
	if !*check {
		path := strings.ToLower(flag.Arg(0))
		if strings.HasSuffix(path, ".elf") || strings.HasSuffix(path, ".hex") {
			return errors.New("please pass the raw firmware.bin image")
		}
		data, err := os.ReadFile(flag.Arg(0))
		if err != nil {
			return err
		}
		img = image.Pad(data)
	}

	ui.Banner()

	port, err := serialport.Open(*portName, protocol.BootloaderBaudRate)
	if err != nil {
		return fmt.Errorf("open %s: %w", *portName, err)
	}
	defer port.Close()
	var link client.Port = port
	if *debug {
		link = &traceLink{Port: port}
	}
	c := client.New(link)
	setBaud := func(baud int) error { return port.SetBaud(baud) }

	var poke func() error
	detected := 0
	if !*noEnter {
		step := ui.Start("Detecting the firmware baud rate on %s", *portName)
		bauds := autobaud.FirmwareBaudRates
		detected, err = autobaud.Detect(link, setBaud)
		if err != nil {
			step.Fail("Baud rate detection failed")
			return err
		}
		if detected != 0 {
			step.Done("Firmware is running at %d baud", detected)
			bauds = []int{detected}
		} else {
			step.Warn("No firmware data stream seen, trying every baud rate")
		}

		frame := protocol.EncodeRequest(protocol.CmdEnter, nil)
		poke = func() error {
			if detected != 0 {
				if err := setBaud(detected); err != nil {
					return err
				}
				if err := autobaud.SendBetweenPackets(link, frame, detected, 3); err != nil {
					return err
				}
				return setBaud(protocol.BootloaderBaudRate)
			}
			for _, baud := range bauds {
				if err := setBaud(baud); err != nil {
					return err
				}
				if _, err := link.Write(frame); err != nil {
					return err
				}
				time.Sleep(autobaud.TxTime(len(frame), baud))
			}
			return setBaud(protocol.BootloaderBaudRate)
		}
		if err := poke(); err != nil {
			return err
		}
		time.Sleep(50 * time.Millisecond)
	}

	text := "Requesting DFU mode from the running firmware"
	if *noEnter {
		text = "Waiting for the bootloader, power-cycle or reset the device now"
	}
	step := ui.Start("%s", text)
	info, err := c.Hello(*wait, poke)
	if errors.Is(err, client.ErrTimeout) {
		step.Fail("No answer from the bootloader")
		if detected != 0 && setBaud(detected) == nil {
			if running, _ := autobaud.Streaming(link, time.Second); running {
				return errors.New("the firmware is still running and never received the DFU request: " +
					"check that the adapter's TX line reaches the device, " +
					"and that the firmware is a *_bootloader.bin build")
			}
		}
		return errors.New("check the port and that the bootloader has been flashed " +
			"(retry with -no-enter and power-cycle the device)")
	}
	if err != nil {
		step.Fail("Bootloader handshake failed")
		return err
	}
	step.Done("Bootloader is ready")
	ui.Device(info, len(img))

	if *check {
		return finishCheck(c, info, *noBoot)
	}

	if err := image.Check(info, img); err != nil {
		return err
	}

	start := time.Now()
	bar := ui.NewProgress(len(img))
	err = c.Flash(info, img, bar.Set)
	bar.Stop()
	if err != nil {
		return err
	}
	ui.Success("Flashed and verified %d bytes in %s", len(img), ui.Elapsed(time.Since(start)))

	if *noBoot {
		ui.Info("Staying in the bootloader (-no-boot)")
		return nil
	}
	if err := c.Boot(); err != nil {
		return fmt.Errorf("boot: %w", err)
	}
	ui.Success("Bootloader accepted the firmware start request (application health is not verified)")
	return nil
}

func finishCheck(c *client.Client, info *protocol.DeviceInfo, noBoot bool) error {
	ui.Success("The device can enter DFU mode")
	booted, err := c.Leave(info, !noBoot)
	switch {
	case err != nil:
		return fmt.Errorf("boot: %w", err)
	case noBoot:
		ui.Info("Staying in the bootloader (-no-boot)")
	case !booted:
		ui.Warn("No valid firmware installed, the device stays in the bootloader")
	default:
		ui.Success("Firmware started again, nothing was flashed")
	}
	return nil
}

type traceLink struct {
	client.Port
}

func (t *traceLink) Write(p []byte) (int, error) {
	ui.Trace("TX", p)
	return t.Port.Write(p)
}

func (t *traceLink) Read(p []byte) (int, error) {
	n, err := t.Port.Read(p)
	if n > 0 {
		ui.Trace("RX", p[:n])
	}
	return n, err
}
