<p align="center">
  <img src="https://raw.githubusercontent.com/anyshake/logotype/master/banner_explorer.png" width="500"/>
</p>

AnyShake Explorer consists of ADC, MCU, and RS232 serial port. The ADC is configured as differential input. It is used to read the voltage count on the geophone, convert it into a digital value and send it to [AnyShake Observer](https://github.com/anyshake/observer) via RS232 serial port.

## Features

 - 5V USB power supply
 - ADS1262 10-channel, 32-bit ADC
 - Differential geophone input
 - 3-component geophone support
 - RS232 / TTL dual serial port
 - Variable channel packet length
 - Variable serial port baud rate
 - Seismic data checksum verification
 - Support reset from AnyShake Observer
 - LED indicators for power, ADC, and serial port
 - Easy to port to other MCUs / ADCs with existing code

## Hardware

See [README.md](https://github.com/anyshake/explorer/tree/master/hardware/README.md) in hardware directory.

## Firmware

See [README.md](https://github.com/anyshake/explorer/tree/master/firmware/README.md) in firmware directory.

## Contributing

TO BE DONE

## Preview

![PCB Front](https://raw.githubusercontent.com/anyshake/explorer/master/preview/pcb_front.jpg)
![PCB Back](https://raw.githubusercontent.com/anyshake/explorer/master/preview/pcb_back.jpg)

## License

[The MIT License (MIT)](https://raw.githubusercontent.com/anyshake/explorer/master/LICENSE)
