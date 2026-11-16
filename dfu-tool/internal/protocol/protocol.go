package protocol

import (
	"encoding/binary"
	"fmt"
	"strings"
)

const (
	HostSync   = 0xA5
	DeviceSync = 0x5A

	CmdHello = 0x01
	CmdBegin = 0x02
	CmdData  = 0x03
	CmdEnd   = 0x04
	CmdBoot  = 0x05
	CmdEnter = 0x06

	MaxChunk           = 1024
	MaxRequestPayload  = 4 + MaxChunk
	MaxResponsePayload = 252

	BootloaderBaudRate = 57600
)

type Status uint8

const (
	StatusOK          Status = 0x00
	StatusBadCRC      Status = 0x01
	StatusBadCmd      Status = 0x02
	StatusBadLength   Status = 0x03
	StatusBadState    Status = 0x04
	StatusBadOffset   Status = 0x05
	StatusFlashError  Status = 0x06
	StatusVerifyError Status = 0x07
	StatusBadImage    Status = 0x08
)

func (s Status) String() string {
	switch s {
	case StatusOK:
		return "ok"
	case StatusBadCRC:
		return "frame CRC mismatch"
	case StatusBadCmd:
		return "unknown command"
	case StatusBadLength:
		return "bad length"
	case StatusBadState:
		return "command not allowed in current state"
	case StatusBadOffset:
		return "unexpected offset"
	case StatusFlashError:
		return "flash erase/program failed"
	case StatusVerifyError:
		return "image CRC verification failed"
	case StatusBadImage:
		return "invalid application image"
	}
	return fmt.Sprintf("status 0x%02x", uint8(s))
}

type StatusError struct {
	Cmd    uint8
	Status Status
}

func (e *StatusError) Error() string {
	return fmt.Sprintf("device rejected command 0x%02x: %s", e.Cmd, e.Status)
}

type BootReason uint8

func (r BootReason) String() string {
	switch r {
	case 1:
		return "requested by firmware"
	case 2:
		return "no valid application"
	case 3:
		return "caught after reset"
	}
	return "unknown"
}

type DeviceInfo struct {
	Reason        BootReason
	AppValid      bool
	AppBase       uint32
	AppMaxSize    uint32
	PageSize      uint16
	MaxChunk      uint16
	BootloaderRev string
}

func ParseDeviceInfo(p []byte) (*DeviceInfo, error) {
	if len(p) < 32 {
		return nil, fmt.Errorf("short HELLO response (%d bytes)", len(p))
	}
	return &DeviceInfo{
		Reason:        BootReason(p[0]),
		AppValid:      p[1] != 0,
		AppBase:       binary.LittleEndian.Uint32(p[4:]),
		AppMaxSize:    binary.LittleEndian.Uint32(p[8:]),
		PageSize:      binary.LittleEndian.Uint16(p[12:]),
		MaxChunk:      binary.LittleEndian.Uint16(p[14:]),
		BootloaderRev: strings.TrimRight(string(p[16:32]), "\x00"),
	}, nil
}

func EncodeRequest(cmd uint8, payload []byte) []byte {
	buf := make([]byte, 0, 4+len(payload)+4)
	buf = binary.LittleEndian.AppendUint32(buf, HostSync|uint32(cmd)<<8|uint32(len(payload))<<16)
	buf = append(buf, payload...)
	return binary.LittleEndian.AppendUint32(buf, CRC(buf))
}

func EncodeResponse(cmd uint8, status Status, payload []byte) []byte {
	buf := make([]byte, 0, 4+len(payload)+4)
	buf = binary.LittleEndian.AppendUint32(buf, DeviceSync|uint32(cmd)<<8|uint32(status)<<16|uint32(len(payload))<<24)
	buf = append(buf, payload...)
	return binary.LittleEndian.AppendUint32(buf, CRC(buf))
}

type Response struct {
	Cmd     uint8
	Status  Status
	Payload []byte
}

// FindResponse returns the first valid frame in buf and the number of bytes
// consumed, or nil and the number of leading bytes that can be dropped.
func FindResponse(buf []byte) (*Response, int) {
	for i := 0; i < len(buf); i++ {
		if buf[i] != DeviceSync {
			continue
		}
		rest := buf[i:]
		if len(rest) < 4 {
			return nil, i
		}
		n := int(rest[3])
		if n%4 != 0 || n > MaxResponsePayload {
			continue
		}
		if len(rest) < 4+n+4 {
			return nil, i
		}
		if CRC(rest[:4+n]) != binary.LittleEndian.Uint32(rest[4+n:]) {
			continue
		}
		r := &Response{Cmd: rest[1], Status: Status(rest[2]), Payload: append([]byte(nil), rest[4:4+n]...)}
		return r, i + 4 + n + 4
	}
	return nil, len(buf)
}
