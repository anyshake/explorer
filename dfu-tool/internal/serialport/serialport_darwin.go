package serialport

import (
	"time"

	"go.bug.st/serial"
)

// bclswl0827/go-serial needs cgo on darwin, so macOS uses go.bug.st/serial (as AnyShake Observer does).
type Port struct {
	conn serial.Port
}

func Open(name string, baud int) (*Port, error) {
	conn, err := serial.Open(name, &serial.Mode{
		BaudRate: baud,
		DataBits: 8,
		Parity:   serial.NoParity,
		StopBits: serial.OneStopBit,
	})
	if err != nil {
		return nil, err
	}
	return &Port{conn: conn}, nil
}

func List() ([]string, error) { return serial.GetPortsList() }

func (p *Port) Read(b []byte) (int, error)           { return p.conn.Read(b) }
func (p *Port) Write(b []byte) (int, error)          { return p.conn.Write(b) }
func (p *Port) Close() error                         { return p.conn.Close() }
func (p *Port) ResetInputBuffer() error              { return p.conn.ResetInputBuffer() }
func (p *Port) SetReadTimeout(t time.Duration) error { return p.conn.SetReadTimeout(t) }

func (p *Port) SetBaud(baud int) error {
	return p.conn.SetMode(&serial.Mode{
		BaudRate: baud,
		DataBits: 8,
		Parity:   serial.NoParity,
		StopBits: serial.OneStopBit,
	})
}
