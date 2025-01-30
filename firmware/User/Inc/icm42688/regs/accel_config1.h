#ifndef __ICM42688_REGS_ACCEL_CONFIG1_H
#define __ICM42688_REGS_ACCEL_CONFIG1_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

#define __ICM42688_REG_ACCEL_CONFIG1_DEFAULT_VALUE 0x0D

#define ICM42688_REG_ACCEL_CONFIG1_ACCEL_UI_FILT_ORD_1 0x00
#define ICM42688_REG_ACCEL_CONFIG1_ACCEL_UI_FILT_ORD_2 0x01
#define ICM42688_REG_ACCEL_CONFIG1_ACCEL_UI_FILT_ORD_3 0x02

#define ICM42688_REG_ACCEL_CONFIG1_ACCEL_DEC2_M2_ORD_3 0x02

typedef struct {
    // Selects order of ACCEL UI filter
    // 0x00: 1st order
    // 0x01: 2nd order (default)
    // 0x02: 3rd order
    uint8_t accel_ui_filt_ord;
    // Order of Accelerometer DEC2_M2 filter
    // 0x02: 3rd order (default)
    uint8_t accel_dec2_m2_ord;
} icm42688_reg_accel_config1_t;

icm42688_reg_accel_config1_t icm42688_reg_new_accel_config1(void);
void icm42688_reg_set_accel_config1(icm42688_reg_accel_config1_t* accel_config1);
void icm42688_reg_get_accel_config1(icm42688_reg_accel_config1_t* accel_config1);

#endif
