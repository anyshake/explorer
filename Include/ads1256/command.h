#ifndef APP_ADS1256_COMMAND_H
#define APP_ADS1256_COMMAND_H

#define DEVICE_COMMAND_WAKEUP 0x00
#define DEVICE_COMMAND_RDATA 0x01
#define DEVICE_COMMAND_RDATAC 0x03
#define DEVICE_COMMAND_SDATAC 0x0F
#define DEVICE_COMMAND_RREG 0x10
#define DEVICE_COMMAND_WREG 0x50
#define DEVICE_COMMAND_SELFCAL 0xF0
#define DEVICE_COMMAND_SELFOCAL 0xF1
#define DEVICE_COMMAND_SELFGCAL 0xF2
#define DEVICE_COMMAND_SYSOCAL 0xF3
#define DEVICE_COMMAND_SYSGCAL 0xF4
#define DEVICE_COMMAND_SYNC 0xFC
#define DEVICE_COMMAND_STANDBY 0xFD
#define DEVICE_COMMAND_RESET 0xFE

#endif