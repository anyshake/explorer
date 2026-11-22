package image

import (
	"encoding/binary"
	"testing"

	"github.com/anyshake/explorer/dfu-tool/internal/protocol"
)

func TestCheckRejectsUnrelocatedFirmware(t *testing.T) {
	info := &protocol.DeviceInfo{AppBase: 0x08001000, AppMaxSize: 60 * 1024}
	img := make([]byte, 8192)
	binary.LittleEndian.PutUint32(img[0:], 0x20004FF0)
	binary.LittleEndian.PutUint32(img[4:], 0x08002235)
	if err := Check(info, img); err != nil {
		t.Fatal(err)
	}
	// A standalone Reset_Handler can be above AppBase, but its early
	// Default_Handler/IRQ vectors still point below the relocated image.
	binary.LittleEndian.PutUint32(img[8:], 0x08000135)
	if err := Check(info, img); err == nil {
		t.Fatal("expected an error")
	}
}

func TestCheckInvalidVectors(t *testing.T) {
	info := &protocol.DeviceInfo{AppBase: 0x08001000, AppMaxSize: 60 * 1024}
	for _, tc := range []struct {
		name   string
		sp, pc uint32
		size   int
	}{
		{"unaligned stack", 0x20004FEF, 0x08001201, 1024},
		{"stack in mailbox", 0x20005000, 0x08001201, 1024},
		{"stack below RAM", 0x20000000, 0x08001201, 1024},
		{"non Thumb", 0x20004FF0, 0x08001200, 1024},
		{"inside vectors", 0x20004FF0, 0x08001081, 1024},
		{"past image", 0x20004FF0, 0x08001401, 1024},
		{"truncated table", 0x20004FF0, 0x08001201, 8},
	} {
		t.Run(tc.name, func(t *testing.T) {
			img := make([]byte, tc.size)
			binary.LittleEndian.PutUint32(img, tc.sp)
			binary.LittleEndian.PutUint32(img[4:], tc.pc)
			if err := Check(info, img); err == nil {
				t.Fatal("expected rejection")
			}
		})
	}
}

func TestPad(t *testing.T) {
	img := Pad(make([]byte, 1027))
	if len(img) != 1028 || img[1027] != 0xFF {
		t.Fatalf("bad padding: %d", len(img))
	}
	if len(Pad(make([]byte, 8))) != 8 {
		t.Fatal("aligned image padded")
	}
}
