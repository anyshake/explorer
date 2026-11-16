package autobaud

import (
	"errors"
	"io"
	"time"

	"github.com/anyshake/explorer/dfu-tool/internal/client"
)

var FirmwareBaudRates = []int{57600, 115200, 230400, 460800}

// Firmware data packets (firmware/User/Src/packet.c): 01 FE body... xor EF 10
func hasDataPacket(buf []byte) bool {
	prev := -1
	for i := 0; i+1 < len(buf); i++ {
		if buf[i] != 0x01 || buf[i+1] != 0xFE {
			continue
		}
		if prev >= 0 && i-prev >= 5 && buf[i-2] == 0xEF && buf[i-1] == 0x10 {
			var x byte
			for _, b := range buf[prev+2 : i-3] {
				x ^= b
			}
			if x == buf[i-3] {
				return true
			}
		}
		prev = i
	}
	return false
}

func listen(port client.Port, d time.Duration) (bool, error) {
	var buf []byte
	tmp := make([]byte, 512)
	deadline := time.Now().Add(d)
	for time.Now().Before(deadline) {
		n, err := port.Read(tmp)
		if err != nil && !errors.Is(err, io.EOF) {
			return false, err
		}
		buf = append(buf, tmp[:n]...)
		if hasDataPacket(buf) {
			return true, nil
		}
	}
	return false, nil
}

// Detect returns the rate at which the firmware's data stream decodes, or 0.
func Detect(port client.Port, setBaud func(int) error) (int, error) {
	for _, baud := range FirmwareBaudRates {
		if err := setBaud(baud); err != nil {
			return 0, err
		}
		_ = port.ResetInputBuffer()
		ok, err := listen(port, 600*time.Millisecond)
		if err != nil {
			return 0, err
		}
		if ok {
			return baud, nil
		}
	}
	return 0, nil
}

// packetEnds reports whether buf ends exactly with a complete data packet.
func packetEnds(buf []byte) bool {
	n := len(buf)
	if n < 5 || buf[n-2] != 0xEF || buf[n-1] != 0x10 {
		return false
	}
	for s := n - 5; s >= 0; s-- {
		if buf[s] != 0x01 || buf[s+1] != 0xFE {
			continue
		}
		var x byte
		for _, b := range buf[s+2 : n-3] {
			x ^= b
		}
		if x == buf[n-3] {
			return true
		}
	}
	return false
}

// SendBetweenPackets writes frame right after a data packet ends, while the
// firmware's transmitter is idle. On half-duplex links (RS-485) a frame sent
// while the firmware transmits is lost. It gives up once the stream stops.
func SendBetweenPackets(port client.Port, frame []byte, baud, tries int) error {
	_ = port.ResetInputBuffer()
	var buf []byte
	tmp := make([]byte, 512)
	for sent := 0; sent < tries; {
		deadline := time.Now().Add(300 * time.Millisecond)
		buf = buf[:0]
		for {
			if time.Now().After(deadline) {
				return nil
			}
			n, err := port.Read(tmp)
			if err != nil && !errors.Is(err, io.EOF) {
				return err
			}
			if n == 0 {
				continue
			}
			buf = append(buf, tmp[:n]...)
			if packetEnds(buf) {
				break
			}
			if len(buf) > 4096 {
				buf = append(buf[:0], buf[len(buf)-2048:]...)
			}
		}
		// The board's RS-485 driver stays enabled ~1 ms (RC) after the last byte.
		time.Sleep(5 * time.Millisecond)
		if _, err := port.Write(frame); err != nil {
			return err
		}
		sent++
		time.Sleep(TxTime(len(frame), baud))
	}
	return nil
}

// Streaming reports whether firmware data packets arrive within d at the current baud rate.
func Streaming(port client.Port, d time.Duration) (bool, error) {
	_ = port.ResetInputBuffer()
	return listen(port, d)
}

// TxTime is how long n bytes take to leave the UART, so the baud rate is not switched mid-frame.
func TxTime(n, baud int) time.Duration {
	return time.Duration(n*10)*time.Second/time.Duration(baud) + 20*time.Millisecond
}
