#ifndef __ADS1262_CMD_MAP_H
#define __ADS1262_CMD_MAP_H

// NOP
#define ADS1262_CMD_NOP 0x00
// Control
#define ADS1262_CMD_RESET 0x06
#define ADS1262_CMD_START1 0x08
#define ADS1262_CMD_STOP1 0x0A
// Conversion data read
#define ADS1262_CMD_RDATA1 0x12
// Calibration
#define ADS1262_CMD_SYOCAL1 0x16
#define ADS1262_CMD_SYGCAL1 0x17
#define ADS1262_CMD_SFOCAL1 0x19
// Register read & write
#define ADS1262_CMD_RREG 0x20
#define ADS1262_CMD_WREG 0x40

#endif
