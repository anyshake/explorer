# Flashing with an ST-Link

An ST-Link is needed for two things:

- installing the bootloader, once;
- flashing the standalone firmware, every time.

With the bootloader installed, firmware updates no longer need it. Use [dfu-tool](bootloader-guide.md) instead.

## What You Need

- An ST-Link V2.
- Three jumper wires.
- A flashing tool:
  - [stlink](https://github.com/stlink-org/stlink) (`st-flash`). The examples below use it.
  - Or [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html), with a graphical interface.

## Connect

1. Power off the Explorer and disassemble it to reach the PCB.
2. Find the 3-pin `STLINK` header (J5) on the PCB.
3. Wire it to the ST-Link. The header pins are, in order, GND, CLK, DIO:

   | ST-Link | Explorer |
   | ------- | -------- |
   | GND     | GND      |
   | SWCLK   | CLK      |
   | SWDIO   | DIO      |

4. Power the Explorer from its normal supply, then plug the ST-Link into your computer.

## Write the Files

Every file goes to a fixed address:

| File                            | Address      |
| ------------------------------- | ------------ |
| `bootloader_E-C1x1G.bin`        | `0x08000000` |
| `*_gnss_<model>_bootloader.bin` | `0x08001000` |
| `*_gnss_<model>.bin`            | `0x08000000` |

Notes:

- The `*_gnss_<model>_bootloader.bin` firmware needs the bootloader.
- The standalone `*_gnss_<model>.bin` firmware overwrites the bootloader.

**Bootloader setup (recommended):** bootloader first, then the firmware.

```sh
st-flash --reset write bootloader_E-C1x1G.bin 0x08000000
st-flash --reset write current_E-C1x1G_gnss_allystar_tau1114_bootloader.bin 0x08001000
```

**Standalone:**

```sh
st-flash --reset write current_E-C1x1G_gnss_allystar_tau1114.bin 0x08000000
```

In STM32CubeProgrammer: connect over **ST-LINK / SWD**, open the file, set the address from the table, and click **Download**.

A file written to the wrong address does not start. Write it again to the right address.

## Troubleshooting

- **`st-flash` cannot find the chip.**
  - Check the three wires, and that the Explorer is powered.
- **The chip is read-protected.** In STM32CubeProgrammer, set **RDP** to **Level 0** under **Option bytes**. This erases the whole chip. Then flash again.
