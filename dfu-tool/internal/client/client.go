package client

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"time"

	"github.com/anyshake/explorer/dfu-tool/internal/protocol"
)

var ErrTimeout = errors.New("timed out waiting for the device")

type Port interface {
	io.ReadWriter
	SetReadTimeout(t time.Duration) error
	ResetInputBuffer() error
}

type Client struct {
	port Port
	buf  []byte
}

func New(port Port) *Client {
	_ = port.SetReadTimeout(20 * time.Millisecond)
	return &Client{port: port}
}

func (c *Client) fill(deadline time.Time) error {
	var tmp [256]byte
	for {
		n, err := c.port.Read(tmp[:])
		if n > 0 {
			c.buf = append(c.buf, tmp[:n]...)
			return nil
		}
		if err != nil && !errors.Is(err, io.EOF) {
			return err
		}
		if time.Now().After(deadline) {
			return ErrTimeout
		}
	}
}

func (c *Client) readResponse(deadline time.Time) (*protocol.Response, error) {
	for {
		r, n := protocol.FindResponse(c.buf)
		c.buf = append(c.buf[:0], c.buf[n:]...)
		if r != nil {
			return r, nil
		}
		if err := c.fill(deadline); err != nil {
			return nil, err
		}
	}
}

func (c *Client) transact(cmd uint8, payload []byte, timeout time.Duration) (*protocol.Response, error) {
	if _, err := c.port.Write(protocol.EncodeRequest(cmd, payload)); err != nil {
		return nil, err
	}
	deadline := time.Now().Add(timeout)
	for {
		r, err := c.readResponse(deadline)
		if err != nil {
			return nil, err
		}
		if r.Cmd != cmd {
			continue
		}
		return r, nil
	}
}

func (c *Client) transactOK(cmd uint8, payload []byte, timeout time.Duration) (*protocol.Response, error) {
	r, err := c.transact(cmd, payload, timeout)
	if err != nil {
		return nil, err
	}
	if r.Status != protocol.StatusOK {
		return r, &protocol.StatusError{Cmd: cmd, Status: r.Status}
	}
	return r, nil
}

// Hello polls the bootloader; poke (if set) is called every second without an answer.
func (c *Client) Hello(timeout time.Duration, poke func() error) (*protocol.DeviceInfo, error) {
	deadline := time.Now().Add(timeout)
	lastPoke := time.Now()
	for time.Now().Before(deadline) {
		if poke != nil && time.Since(lastPoke) > time.Second {
			if err := poke(); err != nil {
				return nil, err
			}
			lastPoke = time.Now()
			time.Sleep(50 * time.Millisecond)
		}
		_ = c.port.ResetInputBuffer()
		c.buf = c.buf[:0]
		r, err := c.transact(protocol.CmdHello, nil, 100*time.Millisecond)
		if errors.Is(err, ErrTimeout) {
			continue
		}
		if err != nil {
			return nil, err
		}
		if r.Status != protocol.StatusOK {
			return nil, &protocol.StatusError{Cmd: protocol.CmdHello, Status: r.Status}
		}
		return protocol.ParseDeviceInfo(r.Payload)
	}
	return nil, ErrTimeout
}

func (c *Client) Flash(info *protocol.DeviceInfo, image []byte, progress func(done, total int)) error {
	begin := binary.LittleEndian.AppendUint32(nil, uint32(len(image)))
	begin = binary.LittleEndian.AppendUint32(begin, protocol.CRC(image))
	if _, err := c.transactOK(protocol.CmdBegin, begin, 10*time.Second); err != nil {
		return fmt.Errorf("begin: %w", err)
	}

	chunk := int(info.MaxChunk)
	if chunk <= 0 || chunk > protocol.MaxChunk {
		chunk = protocol.MaxChunk
	}
	chunk &^= 3
	const maxRetries = 5
	for offset, retries := 0, 0; offset < len(image); {
		end := min(offset+chunk, len(image))
		payload := binary.LittleEndian.AppendUint32(nil, uint32(offset))
		payload = append(payload, image[offset:end]...)

		r, err := c.transact(protocol.CmdData, payload, 2*time.Second)
		if err == nil && r.Status == protocol.StatusOK && len(r.Payload) == 4 {
			offset = int(binary.LittleEndian.Uint32(r.Payload))
			retries = 0
			if progress != nil {
				progress(offset, len(image))
			}
			continue
		}
		if err == nil && r.Status == protocol.StatusBadOffset && len(r.Payload) == 4 {
			next := int(binary.LittleEndian.Uint32(r.Payload))
			if next%4 == 0 && next <= len(image) && next != offset {
				offset = next
				retries = 0
				continue
			}
		}
		if err == nil && r.Status != protocol.StatusOK && r.Status != protocol.StatusBadCRC {
			return fmt.Errorf("data at 0x%x: %w", offset, &protocol.StatusError{Cmd: protocol.CmdData, Status: r.Status})
		}
		if err != nil && !errors.Is(err, ErrTimeout) {
			return fmt.Errorf("data at 0x%x: %w", offset, err)
		}
		retries++
		if retries > maxRetries {
			if err == nil {
				err = &protocol.StatusError{Cmd: protocol.CmdData, Status: r.Status}
			}
			return fmt.Errorf("data at 0x%x: giving up after %d retries: %w", offset, maxRetries, err)
		}
	}

	if _, err := c.transactOK(protocol.CmdEnd, nil, 5*time.Second); err != nil {
		return fmt.Errorf("end: %w", err)
	}
	return nil
}

func (c *Client) Boot() error {
	_, err := c.transactOK(protocol.CmdBoot, nil, 2*time.Second)
	return err
}

// Leave starts the installed firmware if boot is set and the firmware is valid; it never writes flash.
func (c *Client) Leave(info *protocol.DeviceInfo, boot bool) (bool, error) {
	if !boot || !info.AppValid {
		return false, nil
	}
	return true, c.Boot()
}
