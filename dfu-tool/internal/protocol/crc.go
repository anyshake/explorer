package protocol

var crcTable = func() (t [256]uint32) {
	for i := range t {
		c := uint32(i) << 24
		for range 8 {
			if c&0x80000000 != 0 {
				c = c<<1 ^ 0x04C11DB7
			} else {
				c <<= 1
			}
		}
		t[i] = c
	}
	return
}()

// CRC-32/MPEG-2 over little-endian 32-bit words, as the bootloader computes it.
func CRC(data []byte) uint32 {
	crc := uint32(0xFFFFFFFF)
	for i := 0; i+4 <= len(data); i += 4 {
		for j := 3; j >= 0; j-- {
			crc = crc<<8 ^ crcTable[byte(crc>>24)^data[i+j]]
		}
	}
	return crc
}
