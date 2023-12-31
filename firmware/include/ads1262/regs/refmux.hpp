#ifndef __ADS1262_REGS_REFMUX_HPP__
#define __ADS1262_REGS_REFMUX_HPP__

#include <stdint.h>

#include "ads1262/regs/reg_map.hpp"
#include "ads1262/utils.hpp"

#define __ADC_REFMUX_DEFAULT_VALUE 0x00

#define ADC_REFMUX_INTREF 0x00
#define ADC_REFMUX_EXTAIN0P 0x01
#define ADC_REFMUX_EXTAIN1N 0x01
#define ADC_REFMUX_EXTAIN2P 0x02
#define ADC_REFMUX_EXTAIN3N 0x02
#define ADC_REFMUX_EXTAIN4P 0x03
#define ADC_REFMUX_EXTAIN5N 0x03
#define ADC_REFMUX_INTAVDD 0x04
#define ADC_REFMUX_INTAVSS 0x04

typedef struct {
    // Reference positive input, selects the positive reference input
    // 0x00: Internal 2.5 V reference (default)
    // 0x01: External reference on AIN0P
    // 0x02: External reference on AIN2P
    // 0x03: External reference on AIN4P
    // 0x04: Internal AVDD
    uint8_t refmux_p = 0x00;
    // Reference negative input, selects the negative reference input
    // 0x00: Internal 2.5 V reference (default)
    // 0x01: External reference on AIN1N
    // 0x02: External reference on AIN3N
    // 0x03: External reference on AIN5N
    // 0x04: Internal AVSS
    uint8_t refmux_n = 0x00;
} adc_reg_refmux_t;

void adc_reg_set_refmux(adc_reg_refmux_t* refmux);
void adc_reg_get_refmux(adc_reg_refmux_t* refmux);

#endif
