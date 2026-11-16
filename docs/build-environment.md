# Build Environment

You only need this to build the firmware or bootloader from source. Prebuilt files are attached to every [release](https://github.com/anyshake/explorer/releases).

## Tools

| Project                  | Tools                                                                                                                                       |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------- |
| `firmware`, `bootloader` | [VS Code](https://code.visualstudio.com/) + the [PlatformIO](https://platformio.org/install/ide?install=vscode) extension, or the `pio` CLI |
| `dfu-tool`               | [Go](https://go.dev/dl/) 1.26 or newer                                                                                                      |
| Flashing over SWD        | An ST-Link. See [Flashing with an ST-Link](flashing-with-st-link.md)                                                                        |

PlatformIO downloads the ARM toolchain and the STM32 framework the first time you build.

## Source

```sh
git clone --depth=1 https://github.com/anyshake/explorer.git
```

You can also download a [`.zip`](https://github.com/anyshake/explorer/archive/refs/heads/master.zip) or [`.tar.gz`](https://github.com/anyshake/explorer/archive/refs/heads/master.tar.gz) archive. With an archive, you cannot update it with `git pull`.

Open `firmware` or `bootloader` as a folder in VS Code, not the repository root. PlatformIO looks for `platformio.ini` in the opened folder.

## Next

- [`firmware`](../firmware): set `GNSS_MODEL` and `HARDWARE_REV`, choose an environment, build and upload.
- [`bootloader`](../bootloader): build and upload.
- [`dfu-tool`](../dfu-tool): build, run, cross-compile.
