# Quick Start Guide

## Install VSCode

Download and install [VSCode](https://code.visualstudio.com/).

## Install PlatformIO

Install [PlatformIO](https://platformio.org/) extension for VSCode.

You can checkout [Install PlatformIO](https://platformio.org/install/ide?install=vscode) for more information.

## Clone Repository

Clone this repository to your local machine with `--depth=1` option to reduce download size.

```bash
$ git clone --depth=1 https://github.com/anyshake/explorer.git
```

If you don't have git installed, you can download the source code as a `.zip` or `.tar.gz` archive, extract it and `cd` into the directory. However, you will not be able to update the source code with `git pull`.

- [github.com/anyshake/explorer/archive/refs/heads/master.zip](https://github.com/anyshake/explorer/archive/refs/heads/master.zip)
- [github.com/anyshake/explorer/archive/refs/heads/master.tar.gz](https://github.com/anyshake/explorer/archive/refs/heads/master.tar.gz)

## Build Firmware

Open the `firmware` directory in VSCode, wait for PlatformIO to install dependencies.

After dependencies are installed, you can build the firmware with PlatformIO.

- Click PlatformIO icon on the left sidebar
- Click Default > General > Build

## Flash Firmware

### Burn with ST-Link

Ensure the ST-Link is connected to the device correctly (SWCLK->CLK, SWDIO->DIO, GND->GND), then connect the ST-Link to your PC with USB.

Make sure the ST-Link is recognized by your PC, then you can flash the firmware with PlatformIO.

- Click PlatformIO icon on the left sidebar
- Click Default > General > Upload

The device will be reset after the firmware is uploaded.

### Burn with Serial Port

Toggling the BOOT0 jumper to right side, connect the board to your PC by RS232 serial port or USB to TTL converter, then power on the device.

Make sure the serial port is recognized by your PC, modify the `upload_protocol` field in `platformio.ini` to the correct serial port.

```ini
upload_protocol = serial
```

After saving the configuration, you can flash the firmware with PlatformIO.

- Click PlatformIO icon on the left sidebar
- Click Default > General > Upload

After the firmware is uploaded, toggle the BOOT0 jumper left size, then reset the device manually.
