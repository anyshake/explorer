## Which firmware should I download?

All firmware binaries in this release support **both E-C111G and E-C121G**. The firmware automatically detects the IMU and GNSS baud rate.

1. **Identify the GNSS module.** Read the model marking printed on the GNSS module. Prefer the firmware for that model or model family. If there is no model marking, or you are unsure, choose `generic`.
2. **Check the PCB hardware revision.** Read the hardware revision printed on the PCB and choose the filename prefix below.

| PCB hardware revision     | Filename prefix |
| ------------------------- | --------------- |
| **20250804 or later**     | `current_`      |
| **Earlier than 20250804** | `legacy_`       |

Choose the `.bin` file with that prefix and the filename ending matching your GNSS module:

| GNSS module marking                                                   | Filename ending             |
| --------------------------------------------------------------------- | --------------------------- |
| Unknown, unmarked, or unsure                                          | `gnss_generic.bin`          |
| Quectel **LC260Z**, **LC261Z**, **LC760Z**, or **LC761Z**             | `gnss_quectel_lc760z.bin`   |
| Zhongkewei **ATGM332D** or **ATGM336H**; Quectel **L26K** or **L76K** | `gnss_quectel_l76k.bin`     |
| Allystar **TAU812S**, **TAU1113**, or **TAU1114**                     | `gnss_allystar_tau1114.bin` |

For example:

- PCB revision **20250804** with a Allystar **TAU1114**: download `current_E-C1x1G_gnss_allystar_tau1114.bin`.
- PCB revision **20250111** with an unknown GNSS module: download `legacy_E-C1x1G_gnss_generic.bin`.
