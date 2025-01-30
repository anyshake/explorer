#ifndef __ICM42688_REGS_TEMP_DATA_H
#define __ICM42688_REGS_TEMP_DATA_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

typedef struct {
    // Lower byte of temperature data
    uint8_t temp_data_l;
    // Upper byte of temperature data
    uint8_t temp_data_h;
} icm42688_reg_temp_data_t;

void icm42688_reg_get_temp_data(icm42688_reg_temp_data_t* temp_data);

#endif
