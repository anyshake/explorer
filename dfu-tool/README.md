# dfu-tool

Updates the AnyShake Explorer firmware over its serial port, through the [bootloader](../bootloader).

## Quick Start

1. The bootloader must be installed on the device. Devices shipped before the end of 2026 need this done once, with an ST-Link: see the [bootloader and dfu-tool guide](../docs/bootloader-guide.md).
2. Download two files from the [latest release](https://github.com/anyshake/explorer/releases/latest):
   - `dfu-tool` for your computer;
   - the firmware ending in `_bootloader.bin`.
3. Close AnyShake Observer, then run:

   ```sh
   dfu-tool -port <port> <firmware_bootloader.bin>
   ```

   Port names look like:
   - `COM3` on Windows;
   - `/dev/cu.usbserial-110` on macOS;
   - `/dev/ttyUSB0` on Linux.

   Run `dfu-tool -list` to see them.

No baud rate setting is needed:

- the tool reads the firmware's rate from its data stream (57600 to 460800);
- the bootloader always uses 57600.

### Options

| Option      | Default | Description                                                                                                 |
| ----------- | ------- | ----------------------------------------------------------------------------------------------------------- |
| `-port`     |         | Serial port connected to the Explorer                                                                       |
| `-list`     |         | List serial ports and exit                                                                                  |
| `-no-enter` | off     | Do not ask the firmware to restart; wait for the bootloader. Power-cycle the device while it waits          |
| `-wait`     | `10s`   | How long to wait for the bootloader                                                                         |
| `-no-boot`  | off     | Stay in the bootloader afterwards (also with `-check`)                                                      |
| `-check`    | off     | Only check that the device enters the bootloader, then restart the firmware. Flashes nothing; takes no file |
| `-no-color` | off     | Plain output, e.g. for logs                                                                                 |
| `-debug`    | off     | Print every byte sent and received                                                                          |

Use `-no-enter` when:

- the firmware is broken or does not respond;
- a previous update was interrupted;
- only the bootloader is installed.

The tool rejects standalone firmware before it writes anything.

To check that a device has the bootloader and can enter it, without changing the firmware:

```sh
dfu-tool -port <port> -check
```

It prints the bootloader information, then starts the installed firmware again.

## Build on Your Own

Requires [Go](https://go.dev/dl/) 1.26 or newer.

```sh
cd dfu-tool
go run . -port /dev/ttyUSB0 ../firmware/.pio/build/genericSTM32F103C8_bootloader/firmware.bin
go build -o dfu-tool .
go test ./...
```

Cross-compiling needs no C compiler. The release binaries are built like this:

```sh
CGO_ENABLED=0 GOOS=windows GOARCH=amd64 go build -trimpath -ldflags="-s -w" -o dfu-tool_windows_amd64.exe .
```

| Package               | Contents                                         |
| --------------------- | ------------------------------------------------ |
| `main.go`             | Options and the update sequence                  |
| `internal/protocol`   | Frame format, commands, statuses, CRC            |
| `internal/client`     | Requests, retries, flashing                      |
| `internal/autobaud`   | Firmware baud rate detection                     |
| `internal/image`      | Image padding and checks                         |
| `internal/ui`         | Terminal output                                  |
| `internal/serialport` | Serial port access (`go.bug.st/serial` on macOS) |

The protocol must match the bootloader and the firmware. See [DFU internals](../docs/dfu-internals.md).
