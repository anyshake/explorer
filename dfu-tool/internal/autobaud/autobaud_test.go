package autobaud

import "testing"

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
