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

// Read returns as soon as any data arrives, like go.bug.st/serial on darwin;
// waiting out the timeout would make SendBetweenPackets miss the idle gap.
func (p *Port) SetReadTimeout(t time.Duration) error {
	if ms := t.Milliseconds(); ms > 0 {
		return p.conn.SetFirstByteReadTimeout(uint32(ms))
	}
	return p.conn.SetReadTimeout(0)
}

func (p *Port) SetBaud(baud int) error {
	return p.conn.Reconfigure(serial.WithBaudrate(baud))
}
