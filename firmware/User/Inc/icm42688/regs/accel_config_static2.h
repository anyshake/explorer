#ifndef __ICM42688_REGS_ACCEL_CONFIG_STATIC2_H
#define __ICM42688_REGS_ACCEL_CONFIG_STATIC2_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

#define __ICM42688_REG_ACCEL_CONFIG_STATIC2_DEFAULT_VALUE 0x30

#define ICM42688_REG_ACCEL_CONFIG_STATIC2_ACCEL_AAF_DIS_ENABLED 0x00
#define ICM42688_REG_ACCEL_CONFIG_STATIC2_ACCEL_AAF_DIS_DISABLED 0x01

typedef struct {
    // Controls bandwidth of the accelerometer anti-alias filter
    // 0x01 to 0x3F: See section 5.2 for details
    uint8_t accel_aaf_delt;
    // Disables the Accelerometer Anti Alias Filter
    // 0x00: Enable accelerometer anti-aliasing filter (default)
    // 0x01: Disable accelerometer anti-aliasing filter
    uint8_t accel_aaf_dis;
} icm42688_reg_accel_config_static2_t;

icm42688_reg_accel_config_static2_t icm42688_reg_new_accel_config_static2(void);
void icm42688_reg_set_accel_config_static2(icm42688_reg_accel_config_static2_t* accel_config_static2);
void icm42688_reg_get_accel_config_static2(icm42688_reg_accel_config_static2_t* accel_config_static2);

#endif
