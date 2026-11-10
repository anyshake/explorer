#ifndef __LSM6DS3_REGS_CTRL10_C_H
#define __LSM6DS3_REGS_CTRL10_C_H

#include <stdint.h>

#include "User/Inc/lsm6ds3/reg_map.h"
#include "User/Inc/lsm6ds3/utils.h"

// Register reset value
#define __LSM6DS3_REG_CTRL10_C_DEFAULT_VALUE 0x38

#define LSM6DS3_REG_CTRL10_C_ZEN_G_OUTPUT_DISABLED 0x00
#define LSM6DS3_REG_CTRL10_C_ZEN_G_OUTPUT_ENABLED 0x01

#define LSM6DS3_REG_CTRL10_C_YEN_G_OUTPUT_DISABLED 0x00
#define LSM6DS3_REG_CTRL10_C_YEN_G_OUTPUT_ENABLED 0x01

#define LSM6DS3_REG_CTRL10_C_XEN_G_OUTPUT_DISABLED 0x00
#define LSM6DS3_REG_CTRL10_C_XEN_G_OUTPUT_ENABLED 0x01

#define LSM6DS3_REG_CTRL10_C_FUNC_EN_DISABLED 0x00
#define LSM6DS3_REG_CTRL10_C_FUNC_EN_ENABLED 0x01

#define LSM6DS3_REG_CTRL10_C_PEDO_RST_STEP_DISABLED 0x00
#define LSM6DS3_REG_CTRL10_C_PEDO_RST_STEP_ENABLED 0x01

#define LSM6DS3_REG_CTRL10_C_SIGN_MOTION_EN_DISABLED 0x00
#define LSM6DS3_REG_CTRL10_C_SIGN_MOTION_EN_ENABLED 0x01

typedef struct {
    // [5] Gyroscope yaw axis (Z) output enable
    // 0x00: Z-axis output disabled
    // 0x01: Z-axis output enabled (default)
    uint8_t zen_g;

    // [4] Gyroscope roll axis (Y) output enable
    // 0x00: Y-axis output disabled
    // 0x01: Y-axis output enabled (default)
    uint8_t yen_g;

    // [3] All Gyroscope pitch axis (X) output enable
    // 0x00: X-axis output disabled
    // 0x01: X-axis output enabled (default)
    uint8_t xen_g;

    // [2] Enable embedded functionalities (pedometer, tilt, significant motion, sensor hub
    // and ironing) and accelerometer HP and LPF2 filters (refer to Figure 6)
    // 0x00: Disable functionalities of embedded functions and accelerometer filters (default)
    // 0x01: Enable functionalities of embedded functions and accelerometer filters
    uint8_t func_en;

    // [1] Reset pedometer step counter
    // 0x00: Disable (default)
    // 0x01: Enable
    uint8_t pedo_rst_step;

    // [0] Enable significant motion function
    // 0x00: Disable (default)
    // 0x01: Enable
    uint8_t sign_motion_en;
} lsm6ds3_reg_ctrl10_c_t;

lsm6ds3_reg_ctrl10_c_t lsm6ds3_reg_new_ctrl10_c(void);
void lsm6ds3_reg_set_ctrl10_c(lsm6ds3_reg_ctrl10_c_t* ctrl10_c);
void lsm6ds3_reg_get_ctrl10_c(lsm6ds3_reg_ctrl10_c_t* ctrl10_c);

#endif
