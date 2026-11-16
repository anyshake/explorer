# AnyShake Explorer Docs

Your Explorer ships with working firmware. To record data, connect it to [AnyShake Observer](https://github.com/anyshake/observer) and see the [product docs](https://anyshake.org/docs/anyshake-explorer/product-overview/). You do not need anything here.

Read on when you want to update the firmware.

## Updating the Firmware

| Guide                                                | Read it when                                                                             |
| ---------------------------------------------------- | ---------------------------------------------------------------------------------------- |
| [Bootloader and dfu-tool](bootloader-guide.md)       | **Recommended.** Use an ST-Link once; after that, every update goes over the serial port |
| [Flashing with an ST-Link](flashing-with-st-link.md) | You install the bootloader, or flash without it                                          |

Devices shipped before the end of 2026 do not have the bootloader preinstalled. If you never install it, you can keep flashing the standalone firmware with an ST-Link.

## Building from Source

| Guide                                     | Contents                                                                          |
| ----------------------------------------- | --------------------------------------------------------------------------------- |
| [Build environment](build-environment.md) | Tools and source code for building the firmware and bootloader                    |
| [`firmware`](../firmware)                 | `platformio.ini` settings and build environments                                  |
| [`bootloader`](../bootloader)             | Building the bootloader                                                           |
| [`dfu-tool`](../dfu-tool)                 | All options; building the tool                                                    |
| [DFU internals](dfu-internals.md)         | Flash layout, boot flow, protocol, and which constants must match across projects |
