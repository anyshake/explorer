#ifndef __ADS1262_REGS_INPMUX_HPP
#define __ADS1262_REGS_INPMUX_HPP

#include <stdint.h>

#include "ads1262/regs/reg_map.hpp"
#include "ads1262/utils.hpp"

#define __ADC_INPMUX_DEFAULT_VALUE 0x01

#define ADC_INPMUX_AIN0 0x00
#define ADC_INPMUX_AIN1 0x01
#define ADC_INPMUX_AIN2 0x02
#define ADC_INPMUX_AIN3 0x03
#define ADC_INPMUX_AIN4 0x04
#define ADC_INPMUX_AIN5 0x05
#define ADC_INPMUX_AIN6 0x06
#define ADC_INPMUX_AIN7 0x07
#define ADC_INPMUX_AIN8 0x08
#define ADC_INPMUX_AIN9 0x09
#define ADC_INPMUX_AINCOM 0x0A
#define ADC_INPMUX_TEMP 0x0B
#define ADC_INPMUX_AVDD 0x0C
#define ADC_INPMUX_DVDD 0x0D
#define ADC_INPMUX_TDAC 0x0E
#define ADC_INPMUX_FLOAT 0x0F

typedef struct {
    // Positive input multiplexer, selects the positive input channel
    // 0x00: AIN0 (default)
    // 0x01: AIN1
    // 0x02: AIN2
    // 0x03: AIN3
    // 0x04: AIN4
    // 0x05: AIN5
    // 0x06: AIN6
    // 0x07: AIN7
    // 0x08: AIN8
    // 0x09: AIN9
    // 0x0A: AINCOM
    // 0x0B: Temperature sensor monitor positive
    // 0x0C: Analog power supply monitor positive
    // 0x0D: Digital power supply monitor positive
    // 0x0E: TDAC test signal positive
    // 0x0F: Float (open connection)
    uint8_t mux_p = 0x00;
    // Negative input multiplexer, selects the negative input channel
    // 0x00: AIN0
    // 0x01: AIN1 (default)
    // 0x02: AIN2
    // 0x03: AIN3
    // 0x04: AIN4
    // 0x05: AIN5
    // 0x06: AIN6
    // 0x07: AIN7
    // 0x08: AIN8
    // 0x09: AIN9
    // 0x0A: AINCOM
    // 0x0B: Temperature sensor monitor negative
    // 0x0C: Analog power supply monitor negative
    // 0x0D: Digital power supply monitor negative
    // 0x0E: TDAC test signal negative
    // 0x0F: Float (open connection, default)
    uint8_t mux_n = 0x01;
} adc_reg_inpmux_t;

void adc_reg_set_inpmux(adc_reg_inpmux_t* inpmux);
void adc_reg_get_inpmux(adc_reg_inpmux_t* inpmux);

#endif
