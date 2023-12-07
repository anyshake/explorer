#ifndef __ADS1262_REGS_IDACMUX_HPP__
#define __ADS1262_REGS_IDACMUX_HPP__

#include <stdint.h>

#include "ads1262/regs/reg_map.hpp"
#include "ads1262/utils.hpp"

#define __ADC_IDACMUX_DEFAULT_VALUE 0xBB

#define ADC_IDACMUX_AIN0 0x00
#define ADC_IDACMUX_AIN1 0x01
#define ADC_IDACMUX_AIN2 0x02
#define ADC_IDACMUX_AIN3 0x03
#define ADC_IDACMUX_AIN4 0x04
#define ADC_IDACMUX_AIN5 0x05
#define ADC_IDACMUX_AIN6 0x06
#define ADC_IDACMUX_AIN7 0x07
#define ADC_IDACMUX_AIN8 0x08
#define ADC_IDACMUX_AIN9 0x09
#define ADC_IDACMUX_AINCOM 0x0A
#define ADC_IDACMUX_NOCONN 0x0B

typedef struct {
    // IDAC2 Output Multiplexer, Selects the analog input pin to connect IDAC2
    // 0x00: AIN0
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
    // 0x0B: No connection (default)
    uint8_t mux_2 = 0x0B;
    // IDAC1 Output Multiplexer, Selects the analog input pin to connect IDAC1
    // 0x00: AIN0
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
    // 0x0B: No connection (default)
    uint8_t mux_1 = 0x0B;
} adc_reg_idacmux_t;

void adc_reg_set_idacmux(adc_reg_idacmux_t* idacmux);
void adc_reg_get_idacmux(adc_reg_idacmux_t* idacmux);

#endif
