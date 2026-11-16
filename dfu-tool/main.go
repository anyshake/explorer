package main

import (
	"errors"
	"flag"
	"fmt"
	"os"
	"strings"
	"time"

	"github.com/pterm/pterm"
	"go.bug.st/serial"

	"github.com/anyshake/explorer/dfu-tool/internal/autobaud"
	"github.com/anyshake/explorer/dfu-tool/internal/client"
	"github.com/anyshake/explorer/dfu-tool/internal/image"
	"github.com/anyshake/explorer/dfu-tool/internal/protocol"
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
		noBoot   = flag.Bool("no-boot", false, "stay in the bootloader after flashing")
		list     = flag.Bool("list", false, "list serial ports and exit")
		debug    = flag.Bool("debug", false, "dump every byte sent and received")
		noColor  = flag.Bool("no-color", false, "plain output without colors and animations")
	)
	flag.Usage = func() {
		pterm.Printfln("Usage: %s -port <port> [options] <firmware.bin>\n\nOptions:", os.Args[0])
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
		ports, err := serial.GetPortsList()
		if err != nil {
			return err
		}
		ui.Ports(ports)
		return nil
	}
	if *portName == "" || flag.NArg() != 1 {
		flag.Usage()
		os.Exit(2)
	}

	path := strings.ToLower(flag.Arg(0))
	if strings.HasSuffix(path, ".elf") || strings.HasSuffix(path, ".hex") {
		return errors.New("please pass the raw firmware.bin image")
	}
	img, err := os.ReadFile(flag.Arg(0))
	if err != nil {
		return err
	}
	img = image.Pad(img)

	ui.Banner()

	port, err := serial.Open(*portName, &serial.Mode{BaudRate: protocol.BootloaderBaudRate})
	if err != nil {
		return fmt.Errorf("open %s: %w", *portName, err)
	}
	defer port.Close()
	var link client.Port = port
	if *debug {
		link = &traceLink{Port: port}
	}
	c := client.New(link)
	setBaud := func(baud int) error { return port.SetMode(&serial.Mode{BaudRate: baud}) }

	var poke func() error
	if !*noEnter {
		step := ui.Start("Detecting the firmware baud rate on %s", *portName)
		bauds := autobaud.FirmwareBaudRates
		detected, err := autobaud.Detect(link, setBaud)
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
			for _, baud := range bauds {
				if err := setBaud(baud); err != nil {
					return err
				}
				if _, err := link.Write(frame); err != nil {
					return err
				}
				_ = port.Drain()
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
		return errors.New("check the port and that the bootloader has been flashed " +
			"(retry with -no-enter and power-cycle the device)")
	}
	if err != nil {
		step.Fail("Bootloader handshake failed")
		return err
	}
	step.Done("Bootloader is ready")
	ui.Device(info, len(img))

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
	ui.Success("New firmware started")
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
