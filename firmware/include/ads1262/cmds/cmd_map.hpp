#ifndef __ADS1262_CMDS_CMD_MAP_HPP
#define __ADS1262_CMDS_CMD_MAP_HPP

// NOP
#define ADC_CMD_NOP 0x00
// Control
#define ADC_CMD_RESET 0x06
#define ADC_CMD_START1 0x08
#define ADC_CMD_STOP1 0x0A
// Conversion data read
#define ADC_CMD_RDATA1 0x12
// Calibration
#define ADC_CMD_SYOCAL1 0x16
#define ADC_CMD_SYGCAL1 0x17
#define ADC_CMD_SFOCAL1 0x19
// Register read & write
#define ADC_CMD_RREG 0x20
#define ADC_CMD_WREG 0x40

#endif
