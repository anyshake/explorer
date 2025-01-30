#ifndef __ICM42688_REGS_ACCEL_DATA_Y_H
#define __ICM42688_REGS_ACCEL_DATA_Y_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

typedef struct {
    // Lower byte of Accel Y-axis data
    uint8_t accel_data_y_l;
    // Upper byte of Accel Y-axis data
    uint8_t accel_data_y_h;
} icm42688_reg_accel_data_y_t;

void icm42688_reg_get_accel_data_y(icm42688_reg_accel_data_y_t* accel_data_y);

#endif
