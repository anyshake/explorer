#ifndef __ADS1262_REGS_FSCAL_H
#define __ADS1262_REGS_FSCAL_H

#include <stdint.h>

#include "ads1262/reg_map.h"
#include "ads1262/utils.h"

#define __ADS1262_REG_FSCAL_DEFAULT_VALUE 0x400000

typedef struct {
    // Full-scale calibration register 0
    // 0x00: Default value
    uint8_t fscal_0;
    // Full-scale calibration register 1
    // 0x00: Default value
    uint8_t fscal_1;
    // Full-scale calibration register 2
    // 0x40: Default value
    uint8_t fscal_2;
} ads1262_reg_fscal_t;

ads1262_reg_fscal_t ads1262_reg_new_fscal(void);
void ads1262_reg_set_fscal(ads1262_reg_fscal_t* fscal);
void ads1262_reg_get_fscal(ads1262_reg_fscal_t* fscal);

#endif
