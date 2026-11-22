# DFU Internals

How the bootloader, the firmware and dfu-tool work together. Read this before you change any of them.

The three projects share no code. They agree only on the constants below.

## Flash and RAM Layout

| Region      | Address range             | Size          |
| ----------- | ------------------------- | ------------- |
| Bootloader  | `0x08000000 - 0x08000FFF` | 4 KiB         |
| Application | `0x08001000 - 0x0800FFFF` | 60 KiB        |
| RAM         | `0x20000000 - 0x20004FEF` | 20 KiB − 16 B |
| Mailbox     | `0x20004FF0 - 0x20004FFF` | 16 B          |

The mailbox is excluded from both linker scripts, so it survives a system reset. It holds:

- `DFU!` (`0x21554644`): set by the firmware, meaning "stay in the bootloader";
- `APP!` (`0x21505041`): set by the bootloader, meaning "start the application".

## Boot Flow

1. **Mailbox is `APP!`:**
   - clear it, and jump to the application before any peripheral is initialised;
   - set `SCB->VTOR` to `0x08001000`, then the MSP, then call the reset vector.
2. **Otherwise,** start USART1 at a fixed **57600 8N1**. The baud rate switches are ignored.
   - **Mailbox is `DFU!`:** stay in update mode. After 60 s with no session, start the application.
   - **No valid application** (bad initial SP or reset vector): stay in update mode until one is loaded.
   - **Otherwise:** wait 500 ms for a `HELLO`, then start the application.

The application is always started through a reset with `APP!` in the mailbox, so it starts with the peripherals in their reset state.

The PC13 LED blinks every 250 ms while waiting, and every 50 ms during a session.

If the firmware started the independent watchdog, it cannot be stopped, so the bootloader keeps refreshing it.

### Entering DFU from the Firmware

The firmware is built with `USE_BOOTLOADER` (environment `genericSTM32F103C8_bootloader`). It watches UART1 for the 8-byte frame:

```
A5 06 00 00 B8 5F 38 C4
```

This is the bootloader's `ENTER` request. When the firmware sees it, it writes `DFU!` to the mailbox and resets. The bootloader then answers the same `ENTER` frame with `OK`.

The firmware's baud rate is set by switches (57600 / 115200 / 230400 / 460800). dfu-tool detects it from the `01 FE … EF 10` data packets. If it sees no stream, it sends `ENTER` at every rate.

## Protocol

All fields are little-endian 32-bit words. Every frame length is a multiple of 4.

| Direction     | Header word                                | Then         |
| ------------- | ------------------------------------------ | ------------ |
| Host → device | `0xA5 \| cmd<<8 \| len16<<16`              | payload, CRC |
| Device → host | `0x5A \| cmd<<8 \| status<<16 \| len8<<24` | payload, CRC |

The CRC is CRC-32/MPEG-2 over the header and the payload, word by word:

- init `0xFFFFFFFF`;
- polynomial `0x04C11DB7`;
- no reflection, no final XOR.

That is the algorithm of the STM32 CRC unit. The bootloader computes it in software.

| Cmd    | Name    | Payload                    | Reply payload / action                                   |
| ------ | ------- | -------------------------- | -------------------------------------------------------- |
| `0x01` | `HELLO` |                            | 32 bytes, listed below                                   |
| `0x02` | `BEGIN` | `size`, `crc`              | Erases the pages, starting with the vector page          |
| `0x03` | `DATA`  | `offset`, up to 1024 bytes | `next_offset`                                            |
| `0x04` | `END`   |                            | Verifies the CRC and vectors, then writes the first word |
| `0x05` | `BOOT`  |                            | Replies, then resets into the application                |
| `0x06` | `ENTER` |                            | `OK`. In the firmware, it triggers the reset into DFU    |

The `HELLO` reply contains, in order:

- `reason`;
- `app_valid`;
- `app_base`;
- `app_max_size`;
- `page_size`;
- `max_chunk`;
- `bl_rev[16]`.

`reason` is one of: `0` none, `1` requested by the firmware, `2` no valid application, `3` host caught the listen window.

| Status | Name         | Status | Name           |
| ------ | ------------ | ------ | -------------- |
| `0`    | `OK`         | `5`    | `BAD_OFFSET`   |
| `1`    | `BAD_CRC`    | `6`    | `FLASH_ERROR`  |
| `2`    | `BAD_CMD`    | `7`    | `VERIFY_ERROR` |
| `3`    | `BAD_LENGTH` | `8`    | `BAD_IMAGE`    |
| `4`    | `BAD_STATE`  |        |                |

Timeouts:

- 200 ms between bytes of a frame;
- 60 s of session inactivity.

### Atomic Update

`BEGIN` erases the vector page first, so the old image is invalid from that moment.

The first word of the image, the initial SP, is held back until `END` has verified the CRC of the whole image and the vector table. An interrupted update therefore leaves no valid application, and the bootloader stays in update mode.

Both dfu-tool (before `BEGIN`) and the bootloader (at `END`) check the initial SP and all 58 handler slots through USBWakeUp. The SP must be aligned and within application RAM (at most `0x20004FF0`). Nonzero handlers must be Thumb addresses after the vector table and inside the uploaded image; the reset handler must not be zero. Reserved zero slots are allowed. The reserved tail and BootRAM signature at `0x108` are not handler slots. Checking only the reset handler is insufficient: a standalone image can have its reset handler above `0x08001000` while early exception handlers still point into the bootloader region.

Before starting an installed image, the bootloader repeats the vector check against the application partition (the uploaded length is not persisted). These checks reject incorrectly linked images, but do not prove firmware compatibility or successful application startup. CRC verifies transfer integrity, not application health. A `BOOT` acknowledgement only means the start request was accepted. Automatic recovery from a hung application would additionally require a watchdog and a startup-confirmation contract with the firmware; this is not currently implemented.

## Constants That Must Match

If you change any of these, change every listed file:

| Constant                         | Files                                                                                                                                                                                                                     |
| -------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Application base / size          | `bootloader/User/Inc/boot.h`, `bootloader/STM32F103C8Tx_FLASH_BOOT.ld`, `bootloader/platformio.ini` (`maximum_size`), `firmware/STM32F103C8Tx_FLASH_APP.ld`, `firmware/platformio.ini` (`offset_address`, `maximum_size`) |
| Mailbox address / `DFU!` value   | `bootloader/User/Inc/boot.h`, `firmware/Utils/Inc/bootloader.h`, both linker scripts (`RAM LENGTH = 20K - 16`), both `platformio.ini` (`maximum_ram_size`)                                                                |
| `ENTER` frame                    | `firmware/Utils/Inc/bootloader.h`, `dfu-tool/internal/protocol`                                                                                                                                                           |
| Frame format, commands, statuses | `bootloader/User/Inc/dfu/protocol.h`, `dfu-tool/internal/protocol`                                                                                                                                                        |
| Bootloader baud rate (57600)     | `bootloader/Bootloader.ioc` → `bootloader/Core/Src/usart.c`, `dfu-tool/internal/protocol` (`BootloaderBaudRate`)                                                                                                          |

`dfu-tool/internal/protocol` has tests that pin the CRC reference values and the `ENTER` bytes.
