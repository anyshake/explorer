# Bootloader and dfu-tool

With the bootloader installed, a firmware update is one command over the serial cable you already use for AnyShake Observer. You do not need to disassemble the device.

Devices shipped before the end of 2026 do not have the bootloader preinstalled. To install it, you need an ST-Link **once**.

## 1. Download

From the [latest release](https://github.com/anyshake/explorer/releases/latest), download:

1. **`bootloader_E-C1x1G.bin`.**
2. **The firmware for your device, ending in `_bootloader.bin`.** The release notes explain how to pick it from:
   - the PCB revision;
   - the GNSS module.
3. **`dfu-tool` for your computer:**

   | Computer                         | File                         |
   | -------------------------------- | ---------------------------- |
   | Windows                          | `dfu-tool_windows_amd64.exe` |
   | macOS, Apple silicon             | `dfu-tool_darwin_arm64`      |
   | macOS, Intel                     | `dfu-tool_darwin_amd64`      |
   | Linux, x86-64                    | `dfu-tool_linux_amd64`       |
   | Linux, ARM64 (e.g. Raspberry Pi) | `dfu-tool_linux_arm64`       |

## 2. Install the Bootloader (once)

Connect an ST-Link as described in [Flashing with an ST-Link](flashing-with-st-link.md). Then write the bootloader first and the firmware second:

```sh
st-flash --reset write bootloader_E-C1x1G.bin 0x08000000
st-flash --reset write current_E-C1x1G_gnss_allystar_tau1114_bootloader.bin 0x08001000
```

In both commands, use your own file names. Disconnect the ST-Link and reassemble the device.

## 3. Update the Firmware

1. Close AnyShake Observer, so that the serial port is free.
2. Run `dfu-tool` with the serial port and the firmware file:

   ```sh
   # Windows
   dfu-tool_windows_amd64.exe -port COM3 current_E-C1x1G_gnss_allystar_tau1114_bootloader.bin

   # macOS (allow it to run once: xattr -d com.apple.quarantine dfu-tool_darwin_arm64)
   chmod +x dfu-tool_darwin_arm64
   ./dfu-tool_darwin_arm64 -port /dev/cu.usbserial-110 current_E-C1x1G_gnss_allystar_tau1114_bootloader.bin

   # Linux
   chmod +x dfu-tool_linux_amd64
   ./dfu-tool_linux_amd64 -port /dev/ttyUSB0 current_E-C1x1G_gnss_allystar_tau1114_bootloader.bin
   ```

   If you do not know the port name, run the tool with `-list`.

The tool:

1. finds the baud rate on its own;
2. restarts the device into the bootloader;
3. writes and verifies the firmware;
4. starts the new firmware.

It takes well under a minute. Leave the baud rate switches as they are.

For every later update, only step 3 is needed.

## If Something Goes Wrong

- **"No answer from the bootloader."**
  - Check the port name, and make sure no other program has the port open.
  - If the firmware does not respond, run the tool with `-no-enter`, then power-cycle the device while the tool waits.
- **The update was interrupted** (cable pulled, power lost).
  - Nothing is lost: an incomplete firmware never starts, and the device waits in update mode. Run the tool again, with `-no-enter` if needed.
- **"image reset vector … is outside the application area."**
  - The file is a standalone build. Use the file ending in `_bootloader.bin`.
- **Linux: permission denied on the port.**
  - Add yourself to the `dialout` group (`uucp` on Arch), then log in again.

All options are listed in the [dfu-tool README](../dfu-tool).

## Going Back to Standalone Firmware

Write a standalone firmware (without `_bootloader`) to `0x08000000` with an ST-Link. This removes the bootloader.
