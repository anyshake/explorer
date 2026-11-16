package ui

import (
	"fmt"
	"time"

	"github.com/pterm/pterm"

	"github.com/anyshake/explorer/dfu-tool/internal/protocol"
)

func Banner() {
	pterm.DefaultHeader.
		WithFullWidth().
		WithBackgroundStyle(pterm.NewStyle(pterm.BgDarkGray)).
		WithTextStyle(pterm.NewStyle(pterm.FgLightWhite, pterm.Bold)).
		Println("AnyShake Explorer DFU")
	pterm.Println()
}

type Step struct {
	spinner *pterm.SpinnerPrinter
}

func Start(format string, a ...any) *Step {
	if pterm.RawOutput {
		pterm.Println(fmt.Sprintf(format, a...) + "...")
		return &Step{}
	}
	s, _ := pterm.DefaultSpinner.WithRemoveWhenDone(false).Start(fmt.Sprintf(format, a...))
	return &Step{spinner: s}
}

func (s *Step) Done(format string, a ...any) {
	s.finish(s.spinnerSuccess, &pterm.Success, format, a...)
}

func (s *Step) Warn(format string, a ...any) {
	s.finish(s.spinnerWarning, &pterm.Warning, format, a...)
}

func (s *Step) Fail(format string, a ...any) {
	s.finish(s.spinnerFail, &pterm.Error, format, a...)
}

func (s *Step) spinnerSuccess(m ...any) { s.spinner.Success(m...) }
func (s *Step) spinnerWarning(m ...any) { s.spinner.Warning(m...) }
func (s *Step) spinnerFail(m ...any)    { s.spinner.Fail(m...) }

func (s *Step) finish(spin func(...any), plain *pterm.PrefixPrinter, format string, a ...any) {
	msg := fmt.Sprintf(format, a...)
	if s.spinner == nil {
		plain.Println(msg)
		return
	}
	spin(msg)
}

func Info(format string, a ...any) {
	pterm.Info.Printfln(format, a...)
}

func Warn(format string, a ...any) {
	pterm.Warning.Printfln(format, a...)
}

func Success(format string, a ...any) {
	pterm.Success.Printfln(format, a...)
}

func Error(err error) {
	pterm.Println()
	pterm.Error.Println(err.Error())
}

func Device(info *protocol.DeviceInfo, imageSize int) {
	valid := pterm.FgRed.Sprint("no")
	if info.AppValid {
		valid = pterm.FgGreen.Sprint("yes")
	}
	used := float64(imageSize) * 100 / float64(info.AppMaxSize)
	rows := [][]string{
		{"Bootloader", info.BootloaderRev},
		{"Entered", info.Reason.String()},
		{"App area", fmt.Sprintf("0x%08x, %d KiB", info.AppBase, info.AppMaxSize/1024)},
		{"Current app", valid},
		{"New image", fmt.Sprintf("%d bytes (%.1f%% of the app area)", imageSize, used)},
	}
	for i := range rows {
		rows[i][0] = pterm.Gray(rows[i][0])
	}
	pterm.Println()
	_ = pterm.DefaultTable.WithData(rows).WithLeftAlignment().Render()
	pterm.Println()
}

type Progress struct {
	bar *pterm.ProgressbarPrinter
}

func NewProgress(total int) *Progress {
	if pterm.RawOutput {
		pterm.Println("Flashing...")
		return &Progress{}
	}
	bar, _ := pterm.DefaultProgressbar.
		WithTotal(total).
		WithTitle("Flashing").
		WithShowCount(false).
		WithRemoveWhenDone(true).
		Start()
	return &Progress{bar: bar}
}

func (p *Progress) Set(done, _ int) {
	if p.bar == nil {
		return
	}
	if d := done - p.bar.Current; d != 0 {
		p.bar.Add(d)
	}
}

func (p *Progress) Stop() {
	if p.bar != nil {
		_, _ = p.bar.Stop()
	}
}

func Ports(ports []string) {
	if len(ports) == 0 {
		Warn("No serial ports found")
		return
	}
	items := make([]pterm.BulletListItem, len(ports))
	for i, p := range ports {
		items[i] = pterm.BulletListItem{Level: 0, Text: p}
	}
	_ = pterm.DefaultBulletList.WithItems(items).Render()
}

func Trace(dir string, p []byte) {
	pterm.Debug.Printfln("%s % x", dir, p)
}

func EnableTrace() {
	pterm.EnableDebugMessages()
}

func Elapsed(d time.Duration) string {
	return fmt.Sprintf("%.1fs", d.Seconds())
}
