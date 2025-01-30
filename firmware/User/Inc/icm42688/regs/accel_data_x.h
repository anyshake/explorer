#ifndef __ICM42688_REGS_ACCEL_DATA_X_H
#define __ICM42688_REGS_ACCEL_DATA_X_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

typedef struct {
    // Lower byte of Accel X-axis data
    uint8_t accel_data_x_l;
    // Upper byte of Accel X-axis data
    uint8_t accel_data_x_h;
} icm42688_reg_accel_data_x_t;

void icm42688_reg_get_accel_data_x(icm42688_reg_accel_data_x_t* accel_data_x);

#endif
