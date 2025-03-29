#ifndef __ADS1262_REGS_MODE_2_H
#define __ADS1262_REGS_MODE_2_H

#include <stdint.h>

#include "ads1262/reg_map.h"
#include "ads1262/utils.h"

#define __ADS1262_REG_MODE_2_DEFAULT_VALUE 0x04

#define ADS1262_REG_MODE_2_BYPASS_ENABLED 0x00
#define ADS1262_REG_MODE_2_BYPASS_BYPASSED 0x01

#define ADS1262_REG_MODE_2_GAIN_1 0x00
#define ADS1262_REG_MODE_2_GAIN_2 0x01
#define ADS1262_REG_MODE_2_GAIN_4 0x02
#define ADS1262_REG_MODE_2_GAIN_8 0x03
#define ADS1262_REG_MODE_2_GAIN_16 0x04
#define ADS1262_REG_MODE_2_GAIN_32 0x05

#define ADS1262_REG_MODE_2_DR_2_5 0x00
#define ADS1262_REG_MODE_2_DR_5 0x01
#define ADS1262_REG_MODE_2_DR_10 0x02
#define ADS1262_REG_MODE_2_DR_16_6 0x03
#define ADS1262_REG_MODE_2_DR_20 0x04
#define ADS1262_REG_MODE_2_DR_50 0x05
#define ADS1262_REG_MODE_2_DR_60 0x06
#define ADS1262_REG_MODE_2_DR_100 0x07
#define ADS1262_REG_MODE_2_DR_400 0x08
#define ADS1262_REG_MODE_2_DR_1200 0x09
#define ADS1262_REG_MODE_2_DR_2400 0x0A
#define ADS1262_REG_MODE_2_DR_4800 0x0B
#define ADS1262_REG_MODE_2_DR_7200 0x0C
#define ADS1262_REG_MODE_2_DR_14400 0x0D
#define ADS1262_REG_MODE_2_DR_19200 0x0E
#define ADS1262_REG_MODE_2_DR_38400 0x0F

typedef struct {
    // PGA bypass mode, selects PGA bypass mode
    // 0x00: PGA enabled (default)
    // 0x01: PGA bypassed
    uint8_t bypass;
    // PGA gain, selects the PGA gain
    // 0x00: 1 V/V (default)
    // 0x01: 2 V/V
    // 0x02: 4 V/V
    // 0x03: 8 V/V
    // 0x04: 16 V/V
    // 0x05: 32 V/V
    uint8_t gain;
    // Data rate, selects the ADC data rate, in FIR filter mode, the available
    // data rates are limited to 2.5, 5, 10 and 20 SPS
    // 0x00: 2.5 SPS
    // 0x01: 5 SPS
    // 0x02: 10 SPS
    // 0x03: 16.6 SPS
    // 0x04: 20 SPS (default)
    // 0x05: 50 SPS
    // 0x06: 60 SPS
    // 0x07: 100 SPS
    // 0x08: 400 SPS
    // 0x09: 1200 SPS
    // 0x0A: 2400 SPS
    // 0x0B: 4800 SPS
    // 0x0C: 7200 SPS
    // 0x0D: 14400 SPS
    // 0x0E: 19200 SPS
    // 0x0F: 38400 SPS
    uint8_t dr;
} ads1262_reg_mode_2_t;

ads1262_reg_mode_2_t ads1262_reg_new_mode_2(void);
void ads1262_reg_set_mode_2(ads1262_reg_mode_2_t* mode_2);
void ads1262_reg_get_mode_2(ads1262_reg_mode_2_t* mode_2);

#endif
