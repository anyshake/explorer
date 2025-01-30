#ifndef __ICM42688_REGS_GYRO_DATA_Z_H
#define __ICM42688_REGS_GYRO_DATA_Z_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

typedef struct {
    // Lower byte of Gyro Z-axis data
    uint8_t gyro_data_z_l;
    // Upper byte of Gyro Z-axis data
    uint8_t gyro_data_z_h;
} icm42688_reg_gyro_data_z_t;

void icm42688_reg_get_gyro_data_z(icm42688_reg_gyro_data_z_t* gyro_data_z);

#endif
