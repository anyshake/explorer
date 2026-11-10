#ifndef __ICM42688_REGS_ACCEL_CONFIG_STATIC4_H
#define __ICM42688_REGS_ACCEL_CONFIG_STATIC4_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

// Register reset value
#define __ICM42688_REG_ACCEL_CONFIG_STATIC4_DEFAULT_VALUE 0x62

typedef struct {
    // [7:4] Controls bandwidth of the accelerometer anti-alias filter
    // See the anti-alias filter bandwidth table in DS-000347, section 5.3
    uint8_t accel_aaf_bitshift;

    // [3:0] Controls bandwidth of the accelerometer anti-alias filter
    // See the anti-alias filter bandwidth table in DS-000347, section 5.3
    uint8_t accel_aaf_deltsqr_bit_8_to_11;
} icm42688_reg_accel_config_static4_t;

icm42688_reg_accel_config_static4_t icm42688_reg_new_accel_config_static4(void);
void icm42688_reg_set_accel_config_static4(icm42688_reg_accel_config_static4_t* accel_config_static4);
void icm42688_reg_get_accel_config_static4(icm42688_reg_accel_config_static4_t* accel_config_static4);

#endif
