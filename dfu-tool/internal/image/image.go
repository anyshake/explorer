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

func Check(info *protocol.DeviceInfo, image []byte) error {
	if len(image) < 8 {
		return errors.New("image is too small")
	}
	if uint32(len(image)) > info.AppMaxSize {
		return fmt.Errorf("image is %d bytes, but the application area holds only %d", len(image), info.AppMaxSize)
	}
	sp := binary.LittleEndian.Uint32(image[0:])
	pc := binary.LittleEndian.Uint32(image[4:])
	if sp&0xFFF00000 != 0x20000000 {
		return fmt.Errorf("image does not start with a vector table (initial SP 0x%08x)", sp)
	}
	if pc&1 == 0 || pc < info.AppBase || pc >= info.AppBase+info.AppMaxSize {
		return fmt.Errorf("image reset vector 0x%08x is outside the application area 0x%08x-0x%08x; "+
			"use the *_bootloader.bin firmware (PlatformIO environment genericSTM32F103C8_bootloader), "+
			"not the standalone one", pc, info.AppBase, info.AppBase+info.AppMaxSize)
	}
	return nil
}
