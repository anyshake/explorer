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

// TxTime: Drain() returns early on CH340, so wait for the bytes to leave before switching baud.
func TxTime(n, baud int) time.Duration {
	return time.Duration(n*10)*time.Second/time.Duration(baud) + 20*time.Millisecond
}
