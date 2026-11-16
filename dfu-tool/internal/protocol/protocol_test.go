package protocol

import (
	"bytes"
	"encoding/binary"
	"testing"
)

func TestCRC(t *testing.T) {
	for _, tc := range []struct {
		words []uint32
		want  uint32
	}{
		{[]uint32{0x12345678}, 0xDF8A8A2B},
		{[]uint32{0x00000000}, 0xC704DD7B},
		{[]uint32{}, 0xFFFFFFFF},
	} {
		var b []byte
		for _, w := range tc.words {
			b = binary.LittleEndian.AppendUint32(b, w)
		}
		if got := CRC(b); got != tc.want {
			t.Errorf("crc(%x) = %08x, want %08x", tc.words, got, tc.want)
		}
	}
}

func TestEnterFrameMatchesFirmware(t *testing.T) {
	// MCU_UTILS_BOOTLOADER_DFU_FRAME in firmware/Utils/Inc/bootloader.h.
	want := []byte{0xA5, 0x06, 0x00, 0x00, 0xB8, 0x5F, 0x38, 0xC4}
	if got := EncodeRequest(CmdEnter, nil); !bytes.Equal(got, want) {
		t.Fatalf("enter frame % x, want % x", got, want)
	}
}

func TestFindResponse(t *testing.T) {
	frame := EncodeResponse(CmdData, StatusOK, []byte{1, 2, 3, 4})
	buf := append([]byte{0x01, 0xFE, 0x33, 0x00, 0xEF, 0x10}, frame...)
	buf = append(buf, 0x5A)
	r, n := FindResponse(buf)
	if r == nil || r.Cmd != CmdData || !bytes.Equal(r.Payload, []byte{1, 2, 3, 4}) || n != len(buf)-1 {
		t.Fatalf("got %+v, %d", r, n)
	}
	if r, n := FindResponse(frame[:6]); r != nil || n != 0 {
		t.Fatalf("partial frame: got %+v, %d", r, n)
	}

	fake := append([]byte{0x5A, 0x00, 0x00, 0x04, 0, 0, 0, 0, 0, 0, 0, 0}, frame...)
	if r, n := FindResponse(fake); r == nil || n != len(fake) {
		t.Fatalf("fake header: got %+v, %d", r, n)
	}
}
