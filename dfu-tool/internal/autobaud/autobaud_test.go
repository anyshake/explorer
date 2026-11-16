package autobaud

import (
	"bytes"
	"testing"
	"time"
)

func packet(body ...byte) []byte {
	var x byte
	for _, b := range body {
		x ^= b
	}
	p := append([]byte{0x01, 0xFE}, body...)
	return append(p, x, 0xEF, 0x10)
}

func TestHasDataPacket(t *testing.T) {
	good := append(append([]byte{0x33, 0x10}, packet(1, 2, 3, 0xEF, 0x10)...), 0x01, 0xFE)
	if !hasDataPacket(good) {
		t.Error("valid packet not detected")
	}
	bad := append(packet(1, 2, 3), 0x01, 0xFE)
	bad[3] ^= 0x40
	if hasDataPacket(bad) {
		t.Error("packet with a bad checksum detected")
	}
	if hasDataPacket([]byte{0x01, 0xFE, 0xEF, 0x10, 0x01, 0xFE}) {
		t.Error("too-short packet detected")
	}
}

func TestPacketEnds(t *testing.T) {
	p := packet(1, 2, 0x01, 0xFE, 3)
	if !packetEnds(append([]byte{0x55, 0xEF, 0x10}, p...)) {
		t.Error("complete packet not detected")
	}
	if packetEnds(p[:len(p)-1]) {
		t.Error("partial packet detected")
	}
	if packetEnds(append(p, 0x01)) {
		t.Error("packet followed by more data detected")
	}
}

// halfDuplex models an RS-485 link: writes are lost while the firmware transmits.
type halfDuplex struct {
	chunks   [][]byte
	busy     bool
	received [][]byte
}

func (h *halfDuplex) Read(p []byte) (int, error) {
	if len(h.chunks) == 0 {
		time.Sleep(time.Millisecond)
		return 0, nil
	}
	n := copy(p, h.chunks[0])
	h.busy = !bytes.HasSuffix(h.chunks[0], []byte{0xEF, 0x10})
	h.chunks = h.chunks[1:]
	return n, nil
}

func (h *halfDuplex) Write(p []byte) (int, error) {
	if !h.busy {
		h.received = append(h.received, append([]byte(nil), p...))
	}
	return len(p), nil
}

func (h *halfDuplex) SetReadTimeout(time.Duration) error { return nil }
func (h *halfDuplex) ResetInputBuffer() error            { return nil }

func TestSendBetweenPackets(t *testing.T) {
	p := packet(9, 8, 7, 6, 5, 4)
	h := &halfDuplex{chunks: [][]byte{p[3:], p[:4], p[4:], p[:6], p[6:]}}
	frame := []byte{0xA5, 0x06, 0x00, 0x00}
	if err := SendBetweenPackets(h, frame, 57600, 3); err != nil {
		t.Fatal(err)
	}
	if len(h.received) != 2 {
		t.Fatalf("frames received while idle: %d, want 2", len(h.received))
	}
}
