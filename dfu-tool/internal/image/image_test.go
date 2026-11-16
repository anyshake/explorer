package image

import (
	"encoding/binary"
	"testing"

	"github.com/anyshake/explorer/dfu-tool/internal/protocol"
)

func TestCheckRejectsUnrelocatedFirmware(t *testing.T) {
	info := &protocol.DeviceInfo{AppBase: 0x08001000, AppMaxSize: 60 * 1024}
	img := make([]byte, 1024)
	binary.LittleEndian.PutUint32(img[0:], 0x20004FF0)
	binary.LittleEndian.PutUint32(img[4:], 0x08002235)
	if err := Check(info, img); err != nil {
		t.Fatal(err)
	}
	binary.LittleEndian.PutUint32(img[4:], 0x08000135)
	if err := Check(info, img); err == nil {
		t.Fatal("expected an error")
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
