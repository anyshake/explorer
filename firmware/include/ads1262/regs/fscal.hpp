#ifndef __ADS1262_REGS_FSCAL_HPP
#define __ADS1262_REGS_FSCAL_HPP

#include <stdint.h>

#include "ads1262/regs/reg_map.hpp"
#include "ads1262/utils.hpp"

#define __ADC_FSCAL_DEFAULT_VALUE 0x400000

typedef struct {
    // Full-scale calibration register 0
    // 0x40: Default value
    uint8_t fscal_0 = 0x40;
    // Full-scale calibration register 1
    // 0x00: Default value
    uint8_t fscal_1 = 0x00;
    // Full-scale calibration register 2
    // 0x00: Default value
    uint8_t fscal_2 = 0x00;
} adc_reg_fscal_t;

void adc_reg_set_fscal(adc_reg_fscal_t* fscal);
void adc_reg_get_fscal(adc_reg_fscal_t* fscal);

#endif
