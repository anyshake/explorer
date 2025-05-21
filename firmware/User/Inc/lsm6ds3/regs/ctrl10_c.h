#ifndef __LSM6DS3_REGS_CTRL10_C_H
#define __LSM6DS3_REGS_CTRL10_C_H

#include <stdint.h>

#include "User/Inc/lsm6ds3/reg_map.h"
#include "User/Inc/lsm6ds3/utils.h"

#define __LSM6DS3_REG_CTRL10_C_DEFAULT_VALUE 0x38

#define LSM6DS3_REG_CTRL10_C_ZEN_G_OUTPUT_DISABLE 0x00
#define LSM6DS3_REG_CTRL10_C_ZEN_G_OUTPUT_ENABLE 0x01

#define LSM6DS3_REG_CTRL10_C_YEN_G_OUTPUT_DISABLE 0x00
#define LSM6DS3_REG_CTRL10_C_YEN_G_OUTPUT_ENABLE 0x01

#define LSM6DS3_REG_CTRL10_C_XEN_G_OUTPUT_DISABLE 0x00
#define LSM6DS3_REG_CTRL10_C_XEN_G_OUTPUT_ENABLE 0x01

#define LSM6DS3_REG_CTRL10_C_FUNC_EN_DISABLE 0x00
#define LSM6DS3_REG_CTRL10_C_FUNC_EN_ENABLE 0x01

#define LSM6DS3_REG_CTRL10_C_PEDO_RST_STEP_DISABLE 0x00
#define LSM6DS3_REG_CTRL10_C_PEDO_RST_STEP_ENABLE 0x01

#define LSM6DS3_REG_CTRL10_C_SIGN_MOTION_EN_DISABLE 0x00
#define LSM6DS3_REG_CTRL10_C_SIGN_MOTION_EN_ENABLE 0x01

typedef struct {
    // Gyroscope yaw axis (Z) output enable
    // 0x00: Z-axis output disabled
    // 0x01: Z-axis output enabled (default)
    uint8_t zen_g;
    // Gyroscope roll axis (Y) output enable
    // 0x00: Y-axis output disabled
    // 0x01: Y-axis output enabled (default)
    uint8_t yen_g;
    // All Gyroscope pitch axis (X) output enable
    // 0x00: X-axis output disabled
    // 0x01: X-axis output enabled (default)
    uint8_t xen_g;
    // Enable embedded functionalities (pedometer, tilt, significant motion, sensor hub
    // and ironing) and accelerometer HP and LPF2 filters (refer to Figure 6)
    // 0x00: disable functionalities of embedded functions and accelerometer filters (default)
    // 0x01: enable functionalities of embedded functions and accelerometer filters
    uint8_t func_en;
    // Reset pedometer step counter
    // 0x00: disable (default)
    // 0x01: enable
    uint8_t pedo_rst_step;
    // Enable significant motion function
    // 0x00: disable (default)
    // 0x01: enable
    uint8_t sign_motion_en;
} lsm6ds3_reg_ctrl10_c_t;

lsm6ds3_reg_ctrl10_c_t lsm6ds3_reg_new_ctrl10_c(void);
void lsm6ds3_reg_set_ctrl10_c(lsm6ds3_reg_ctrl10_c_t* ctrl10_c);
void lsm6ds3_reg_get_ctrl10_c(lsm6ds3_reg_ctrl10_c_t* ctrl10_c);

#endif
