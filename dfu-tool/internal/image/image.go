package image

import (
	"encoding/binary"
	"errors"
	"fmt"

	"github.com/anyshake/explorer/dfu-tool/internal/protocol"
)

func Pad(image []byte) []byte {
	for len(image)%4 != 0 {
		image = append(image, 0xFF)
	}
	return image
}

// STM32F103 medium-density: core vectors and IRQs through USBWakeUp.
// Excludes the reserved tail and the BootRAM signature at offset 0x108.
const vectorWords = 59

func Check(info *protocol.DeviceInfo, image []byte) error {
	if len(image) < vectorWords*4 {
		return errors.New("image is too small")
	}
	if uint32(len(image)) > info.AppMaxSize {
		return fmt.Errorf("image is %d bytes, but the application area holds only %d", len(image), info.AppMaxSize)
	}
	sp := binary.LittleEndian.Uint32(image[0:])
	if sp <= 0x20000000 || sp > 0x20004FF0 || sp&3 != 0 {
		return fmt.Errorf("image does not start with a vector table (initial SP 0x%08x)", sp)
	}
	for i := 1; i < vectorWords; i++ {
		pc := binary.LittleEndian.Uint32(image[i*4:])
		if pc == 0 && i != 1 { // unused/reserved vectors
			continue
		}
		addr := uint64(pc &^ 1)
		if pc&1 == 0 || addr < uint64(info.AppBase)+vectorWords*4 || addr >= uint64(info.AppBase)+uint64(len(image)) {
			return fmt.Errorf("image vector %d (offset 0x%x) points to 0x%08x, outside the loaded application code; "+
				"use the *_bootloader.bin firmware (PlatformIO environment genericSTM32F103C8_bootloader), "+
				"not the standalone one", i, i*4, pc)
		}
	}
	return nil
}
