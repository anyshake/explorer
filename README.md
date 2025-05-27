<p align="center">
  <img src="https://raw.githubusercontent.com/anyshake/explorer/master/images/header.png" width="500"/>
</p>

## Overview

**AnyShake Explorer** is an open-source hardware project that includes complete schematics and PCB layout files, allowing anyone to build their own high-sensitivity seismograph. It uses three 4.5 Hz velocity geophones (EHZ, EHE, and EHN) to detect seismic waves. A geophone is a transducer that converts ground motion into an electrical signal. Compared to most DIY seismographs, this solution offers significantly improved sensitivity and accuracy.

This version features a firmware-based compensation algorithm that effectively extends the natural frequency response down to approximately **0.8 Hz**, enabling the detection of lower-frequency seismic events.

## Hardware

For hardware schematics, layout files, and additional details, see the [hardware README](https://github.com/anyshake/explorer/tree/v1/hardware/README.md).

## Firmware

For firmware source code and build instructions, refer to the [firmware README](https://github.com/anyshake/explorer/tree/v1/firmware/README.md).

## Documentation

Visit the [official documentation](https://anyshake.org/docs/project-overview/) for a quick start guide, usage instructions, and in-depth technical details.

## Preview

![PCB Front](https://raw.githubusercontent.com/anyshake/explorer/v1/preview/pcb_front.jpg)
![PCB Back](https://raw.githubusercontent.com/anyshake/explorer/v1/preview/pcb_back.jpg)

## Credits

**AnyShake Explorer** is designed and developed by [@bclswl0827](https://github.com/bclswl0827). Testing and validation were carried out by [@TenkyuChimata](https://github.com/TenkyuChimata).

We would also like to thank the following tools and platforms that made this project possible:

* [KiCad EDA](https://kicad.org/)
* [PlatformIO](https://platformio.org/)

> *Why not use the Arduino IDE?*
> While Arduino IDE is beginner-friendly, it lacks proper support for larger and more complex projects. Dependency management and multi-file structure are cumbersome. In contrast, PlatformIO offers a modern, cross-platform development environment with full support for Arduino frameworks and efficient package management—making it ideal for serious embedded development.
