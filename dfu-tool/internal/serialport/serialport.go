//go:build !darwin

package serialport

import (
	"time"

	"github.com/bclswl0827/go-serial"
)

type Port struct {
	conn *serial.Port
}

func Open(name string, baud int) (*Port, error) {
	conn, err := serial.Open(name,
		serial.WithBaudrate(baud),
		serial.WithDataBits(8),
		serial.WithParity(serial.NoParity),
		serial.WithStopBits(serial.OneStopBit),
	)
	if err != nil {
		return nil, err
	}
	return &Port{conn: conn}, nil
}

func List() ([]string, error) { return serial.GetPortsList() }

func (p *Port) Read(b []byte) (int, error)  { return p.conn.Read(b) }
func (p *Port) Write(b []byte) (int, error) { return p.conn.Write(b) }
func (p *Port) Close() error                { return p.conn.Close() }
func (p *Port) ResetInputBuffer() error     { return p.conn.ResetInputBuffer() }

func (p *Port) SetReadTimeout(t time.Duration) error {
	return p.conn.SetReadTimeout(int(t.Milliseconds()))
}

func (p *Port) SetBaud(baud int) error {
	return p.conn.Reconfigure(serial.WithBaudrate(baud))
}
