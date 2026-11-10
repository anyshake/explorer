#ifndef __LSM6DS3TR_C_REGS_CTRL10_C_H
#define __LSM6DS3TR_C_REGS_CTRL10_C_H

#include <stdint.h>

#include "User/Inc/lsm6ds3tr_c/reg_map.h"
#include "User/Inc/lsm6ds3tr_c/utils.h"

// Register reset value
#define __LSM6DS3TR_C_REG_CTRL10_C_DEFAULT_VALUE 0x00

#define LSM6DS3TR_C_REG_CTRL10_C_WRIST_TILT_EN_DISABLED 0x00
#define LSM6DS3TR_C_REG_CTRL10_C_WRIST_TILT_EN_ENABLED 0x01

#define LSM6DS3TR_C_REG_CTRL10_C_TIMER_EN_DISABLED 0x00
#define LSM6DS3TR_C_REG_CTRL10_C_TIMER_EN_ENABLED 0x01

#define LSM6DS3TR_C_REG_CTRL10_C_PEDO_EN_DISABLED 0x00
#define LSM6DS3TR_C_REG_CTRL10_C_PEDO_EN_ENABLED 0x01

#define LSM6DS3TR_C_REG_CTRL10_C_TILT_EN_DISABLED 0x00
#define LSM6DS3TR_C_REG_CTRL10_C_TILT_EN_ENABLED 0x01

#define LSM6DS3TR_C_REG_CTRL10_C_FUNC_EN_DISABLED 0x00
#define LSM6DS3TR_C_REG_CTRL10_C_FUNC_EN_ENABLED 0x01

#define LSM6DS3TR_C_REG_CTRL10_C_PEDO_RST_STEP_DISABLED 0x00
#define LSM6DS3TR_C_REG_CTRL10_C_PEDO_RST_STEP_ENABLED 0x01

#define LSM6DS3TR_C_REG_CTRL10_C_SIGN_MOTION_EN_DISABLED 0x00
#define LSM6DS3TR_C_REG_CTRL10_C_SIGN_MOTION_EN_ENABLED 0x01

typedef struct {
    // [7] Wrist tilt enable; bit 6 is reserved
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t wrist_tilt_en;

    // [5] Timestamp enable
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t timer_en;

    // [4] Pedometer enable
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t pedo_en;

    // [3] Tilt calculation enable
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t tilt_en;

    // [2] Enable embedded functions. TR-C LPF2 does not require this bit.
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t func_en;

    // [1] Reset pedometer step counter
    // 0x00: Disable (default)
    // 0x01: Enable
    uint8_t pedo_rst_step;

    // [0] Enable significant motion function
    // 0x00: Disable (default)
    // 0x01: Enable
    uint8_t sign_motion_en;
} lsm6ds3tr_c_reg_ctrl10_c_t;

lsm6ds3tr_c_reg_ctrl10_c_t lsm6ds3tr_c_reg_new_ctrl10_c(void);
void lsm6ds3tr_c_reg_set_ctrl10_c(lsm6ds3tr_c_reg_ctrl10_c_t* ctrl10_c);
void lsm6ds3tr_c_reg_get_ctrl10_c(lsm6ds3tr_c_reg_ctrl10_c_t* ctrl10_c);

#endif
