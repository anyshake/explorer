#ifndef __ADS1262_REGS_OFCAL_H
#define __ADS1262_REGS_OFCAL_H

#include <stdint.h>

#include "ads1262/reg_map.h"
#include "ads1262/utils.h"

#define __ADS1262_REG_OFCAL_DEFAULT_VALUE 0x000000

typedef struct {
    // Offset calibration register 0
    // 0x00: Default value
    uint8_t ofcal_0;
    // Offset calibration register 1
    // 0x00: Default value
    uint8_t ofcal_1;
    // Offset calibration register 2
    // 0x00: Default value
    uint8_t ofcal_2;
} ads1262_reg_ofcal_t;

ads1262_reg_ofcal_t ads1262_reg_new_ofcal(void);
void ads1262_reg_set_ofcal(ads1262_reg_ofcal_t* ofcal);
void ads1262_reg_get_ofcal(ads1262_reg_ofcal_t* ofcal);

#endif
