package client

import (
	"bytes"
	"encoding/binary"
	"sync"
	"testing"
	"time"

	"github.com/anyshake/explorer/dfu-tool/internal/image"
	"github.com/anyshake/explorer/dfu-tool/internal/protocol"
)

const (
	simAppBase = 0x08001000
	simAppSize = 60 * 1024
)

type simDevice struct {
	mu      sync.Mutex
	toHost  []byte
	rx      []byte
	flash   []byte
	begun   bool
	size    int
	crc     uint32
	next    int
	first   []byte
	booted  bool
	dropAck map[int]bool
	garbage []byte
}

func newSimDevice() *simDevice {
	d := &simDevice{flash: bytes.Repeat([]byte{0xFF}, simAppSize), dropAck: map[int]bool{}}
	return d
}

func (d *simDevice) Read(p []byte) (int, error) {
	d.mu.Lock()
	n := copy(p, d.toHost)
	d.toHost = d.toHost[n:]
	d.mu.Unlock()
	if n == 0 {
		time.Sleep(time.Millisecond)
	}
	return n, nil
}

func (d *simDevice) SetReadTimeout(time.Duration) error { return nil }
func (d *simDevice) ResetInputBuffer() error {
	d.mu.Lock()
	d.toHost = nil
	d.mu.Unlock()
	return nil
}

func (d *simDevice) Write(p []byte) (int, error) {
	d.mu.Lock()
	defer d.mu.Unlock()
	d.rx = append(d.rx, p...)
	for {
		i := 0
		for ; i+4 <= len(d.rx); i++ {
			h := binary.LittleEndian.Uint32(d.rx[i:])
			n, cmd := h>>16, h>>8&0xFF
			if h&0xFF == protocol.HostSync && cmd >= 1 && cmd <= 6 && n <= 1028 && n%4 == 0 {
				break
			}
		}
		if i+4 > len(d.rx) {
			return len(p), nil
		}
		f := d.rx[i:]
		n := int(binary.LittleEndian.Uint16(f[2:]))
		if len(f) < 4+n+4 {
			return len(p), nil
		}
		d.rx = f[4+n+4:]
		if protocol.CRC(f[:4+n]) != binary.LittleEndian.Uint32(f[4+n:]) {
			d.reply(f[1], protocol.StatusBadCRC, nil)
			continue
		}
		d.handle(f[1], f[4:4+n])
	}
}

func (d *simDevice) reply(cmd uint8, st protocol.Status, payload []byte) {
	if d.garbage != nil {
		d.toHost = append(d.toHost, d.garbage...)
		d.garbage = nil
	}
	d.toHost = append(d.toHost, protocol.EncodeResponse(cmd, st, payload)...)
}

func (d *simDevice) handle(cmd uint8, p []byte) {
	u32 := func(v int) []byte { return binary.LittleEndian.AppendUint32(nil, uint32(v)) }
	switch cmd {
	case protocol.CmdHello:
		out := make([]byte, 32)
		out[0], out[1] = 1, 1
		binary.LittleEndian.PutUint32(out[4:], simAppBase)
		binary.LittleEndian.PutUint32(out[8:], simAppSize)
		binary.LittleEndian.PutUint16(out[12:], 1024)
		binary.LittleEndian.PutUint16(out[14:], 1024)
		copy(out[16:], "git-test")
		d.reply(cmd, protocol.StatusOK, out)
	case protocol.CmdBegin:
		d.size = int(binary.LittleEndian.Uint32(p))
		d.crc = binary.LittleEndian.Uint32(p[4:])
		for i := range d.flash {
			d.flash[i] = 0xFF
		}
		d.begun, d.next = true, 0
		d.reply(cmd, protocol.StatusOK, nil)
	case protocol.CmdData:
		off, data := int(binary.LittleEndian.Uint32(p)), p[4:]
		switch {
		case !d.begun:
			d.reply(cmd, protocol.StatusBadState, nil)
			return
		case off < d.next && off+len(data) == d.next:
		case off != d.next:
			d.reply(cmd, protocol.StatusBadOffset, u32(d.next))
			return
		default:
			if off == 0 {
				d.first = append([]byte(nil), data[:4]...)
				copy(d.flash[4:], data[4:])
			} else {
				copy(d.flash[off:], data)
			}
			d.next = off + len(data)
		}
		if d.dropAck[off] {
			delete(d.dropAck, off)
			return
		}
		d.reply(cmd, protocol.StatusOK, u32(d.next))
	case protocol.CmdEnd:
		img := append(append([]byte(nil), d.first...), d.flash[4:d.size]...)
		if protocol.CRC(img) != d.crc {
			d.reply(cmd, protocol.StatusVerifyError, nil)
			return
		}
		copy(d.flash, d.first)
		d.begun = false
		d.reply(cmd, protocol.StatusOK, nil)
	case protocol.CmdEnter:
		d.reply(cmd, protocol.StatusOK, nil)
	case protocol.CmdBoot:
		d.booted = true
		d.reply(cmd, protocol.StatusOK, nil)
	default:
		d.reply(cmd, protocol.StatusBadCmd, nil)
	}
}

func testImage(size int) []byte {
	img := make([]byte, size)
	for i := range img {
		img[i] = byte(i*7 + i>>8)
	}
	binary.LittleEndian.PutUint32(img[0:], 0x20004FF0)
	binary.LittleEndian.PutUint32(img[4:], simAppBase+0x1235)
	return img
}

func TestFlashRoundTrip(t *testing.T) {
	dev := newSimDevice()
	dev.garbage = []byte{0x01, 0xFE, 0x5A, 0x00, 0xEF, 0x10}
	dev.dropAck[2048] = true

	c := New(dev)
	info, err := c.Hello(time.Second, nil)
	if err != nil {
		t.Fatal(err)
	}
	if info.AppBase != simAppBase || info.BootloaderRev != "git-test" || info.MaxChunk != 1024 {
		t.Fatalf("unexpected info %+v", info)
	}

	img := testImage(53472)
	if err := image.Check(info, img); err != nil {
		t.Fatal(err)
	}
	if err := c.Flash(info, img, nil); err != nil {
		t.Fatal(err)
	}
	if !bytes.Equal(dev.flash[:len(img)], img) {
		t.Fatal("flash content mismatch")
	}
	if err := c.Boot(); err != nil || !dev.booted {
		t.Fatalf("boot: %v", err)
	}
}

func TestOddSizedImageIsPadded(t *testing.T) {
	dev := newSimDevice()
	c := New(dev)
	info, err := c.Hello(time.Second, nil)
	if err != nil {
		t.Fatal(err)
	}
	img := image.Pad(testImage(1027))
	if len(img) != 1028 || img[1027] != 0xFF {
		t.Fatalf("bad padding: %d", len(img))
	}
	if err := c.Flash(info, img, nil); err != nil {
		t.Fatal(err)
	}
}
