## Which file do I need?

Every firmware comes in two builds:

| Build                        | File ends in      | How you flash it                                                                           |
| ---------------------------- | ----------------- | ------------------------------------------------------------------------------------------ |
| **Bootloader (recommended)** | `_bootloader.bin` | Over the serial port with `dfu-tool`. Needs the bootloader, installed once with an ST-Link |
| Standalone                   | `.bin`            | With an ST-Link, every time                                                                |

**Devices shipped before the end of 2026 do not have the bootloader preinstalled.**

- Install it once with an ST-Link: **[bootloader and dfu-tool guide](https://github.com/anyshake/explorer/blob/master/docs/bootloader-guide.md)**.
- After that, you never need to disassemble the device for a firmware update.

Installing the bootloader is an extra step, so the standalone build is still provided. See [Flashing with an ST-Link](https://github.com/anyshake/explorer/blob/master/docs/flashing-with-st-link.md) for flashing it.

## Pick your firmware

All files support **both E-C111G and E-C121G**. The firmware detects the IMU and the GNSS baud rate automatically.

**1. PCB hardware revision** (printed on the PCB) → filename prefix:

| PCB hardware revision     | Prefix     |
| ------------------------- | ---------- |
| **20250804 or later**     | `current_` |
| **Earlier than 20250804** | `legacy_`  |

**2. GNSS module** (model printed on the module) → filename part. If it has no marking, or you are unsure, choose `gnss_generic`.

| GNSS module marking                                                   | Filename part           |
| --------------------------------------------------------------------- | ----------------------- |
| Unknown, unmarked, or unsure                                          | `gnss_generic`          |
| Quectel **LC260Z**, **LC261Z**, **LC760Z**, or **LC761Z**             | `gnss_quectel_lc760z`   |
| Zhongkewei **ATGM332D** or **ATGM336H**; Quectel **L26K** or **L76K** | `gnss_quectel_l76k`     |
| Allystar **TAU812S**, **TAU1113**, or **TAU1114**                     | `gnss_allystar_tau1114` |

For example, a PCB with revision **20250804** and an Allystar **TAU1114**:

- bootloader build: `current_E-C1x1G_gnss_allystar_tau1114_bootloader.bin`;
- standalone build: `current_E-C1x1G_gnss_allystar_tau1114.bin`.

## Other files

| File                         | What it is                                                    |
| ---------------------------- | ------------------------------------------------------------- |
| `bootloader_E-C1x1G.bin`     | The bootloader. Flash it once to `0x08000000` with an ST-Link |
| `dfu-tool_windows_amd64.exe` | Update tool for Windows                                       |
| `dfu-tool_darwin_arm64`      | Update tool for macOS, Apple silicon                          |
| `dfu-tool_darwin_amd64`      | Update tool for macOS, Intel                                  |
| `dfu-tool_linux_amd64`       | Update tool for Linux, x86-64                                 |
| `dfu-tool_linux_arm64`       | Update tool for Linux, ARM64 (e.g. Raspberry Pi)              |

Once the bootloader is installed, an update is one command:

```sh
dfu-tool_windows_amd64.exe -port COM3 current_E-C1x1G_gnss_allystar_tau1114_bootloader.bin
```
