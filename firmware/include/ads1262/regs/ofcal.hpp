#ifndef __ADS1262_REGS_OFCAL_HPP
#define __ADS1262_REGS_OFCAL_HPP

#include <stdint.h>

#include "ads1262/regs/reg_map.hpp"
#include "ads1262/utils.hpp"

#define __ADC_OFCAL_DEFAULT_VALUE 0x000000

typedef struct {
    // Offset calibration register 0
    // 0x00: Default value
    uint8_t ofcal_0 = 0x00;
    // Offset calibration register 1
    // 0x00: Default value
    uint8_t ofcal_1 = 0x00;
    // Offset calibration register 2
    // 0x00: Default value
    uint8_t ofcal_2 = 0x00;
} adc_reg_ofcal_t;

void adc_reg_set_ofcal(adc_reg_ofcal_t* ofcal);
void adc_reg_get_ofcal(adc_reg_ofcal_t* ofcal);

#endif
