#ifndef __ADS1262_REGS_MODE_2_HPP
#define __ADS1262_REGS_MODE_2_HPP

#include <stdint.h>

#include "ads1262/regs/reg_map.hpp"
#include "ads1262/utils.hpp"

#define __ADC_MODE_2_DEFAULT_VALUE 0x04

#define ADC_MODE_2_BYPASS_ENABLED 0x00
#define ADC_MODE_2_BYPASS_BYPASSED 0x01

#define ADC_MODE_2_GAIN_1 0x00
#define ADC_MODE_2_GAIN_2 0x01
#define ADC_MODE_2_GAIN_4 0x02
#define ADC_MODE_2_GAIN_8 0x03
#define ADC_MODE_2_GAIN_16 0x04
#define ADC_MODE_2_GAIN_32 0x05

#define ADC_MODE_2_DR_2_5 0x00
#define ADC_MODE_2_DR_5 0x01
#define ADC_MODE_2_DR_10 0x02
#define ADC_MODE_2_DR_16_6 0x03
#define ADC_MODE_2_DR_20 0x04
#define ADC_MODE_2_DR_50 0x05
#define ADC_MODE_2_DR_60 0x06
#define ADC_MODE_2_DR_100 0x07
#define ADC_MODE_2_DR_400 0x08
#define ADC_MODE_2_DR_1200 0x09
#define ADC_MODE_2_DR_2400 0x0A
#define ADC_MODE_2_DR_4800 0x0B
#define ADC_MODE_2_DR_7200 0x0C
#define ADC_MODE_2_DR_14400 0x0D
#define ADC_MODE_2_DR_19200 0x0E
#define ADC_MODE_2_DR_38400 0x0F

typedef struct {
    // PGA bypass mode, selects PGA bypass mode
    // 0x00: PGA enabled (default)
    // 0x01: PGA bypassed
    uint8_t bypass = 0x00;
    // PGA gain, selects the PGA gain
    // 0x00: 1 V/V (default)
    // 0x01: 2 V/V
    // 0x02: 4 V/V
    // 0x03: 8 V/V
    // 0x04: 16 V/V
    // 0x05: 32 V/V
    uint8_t gain = 0x00;
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
    uint8_t dr = 0x04;
} adc_reg_mode_2_t;

void adc_reg_set_mode_2(adc_reg_mode_2_t* mode_2);
void adc_reg_get_mode_2(adc_reg_mode_2_t* mode_2);

#endif
