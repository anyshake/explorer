#ifndef __LSM6DS3TR_C_REGS_INT1_CTRL_H
#define __LSM6DS3TR_C_REGS_INT1_CTRL_H

#include <stdint.h>

#include "User/Inc/lsm6ds3tr_c/reg_map.h"
#include "User/Inc/lsm6ds3tr_c/utils.h"

// Register reset value
#define __LSM6DS3TR_C_REG_INT1_CTRL_DEFAULT_VALUE 0x00

#define LSM6DS3TR_C_REG_INT1_CTRL_STEP_DETECTOR_DISABLED 0x00
#define LSM6DS3TR_C_REG_INT1_CTRL_STEP_DETECTOR_ENABLED 0x01

#define LSM6DS3TR_C_REG_INT1_CTRL_SIGN_MOTION_DISABLED 0x00
#define LSM6DS3TR_C_REG_INT1_CTRL_SIGN_MOTION_ENABLED 0x01

#define LSM6DS3TR_C_REG_INT1_CTRL_FULL_FLAG_DISABLED 0x00
#define LSM6DS3TR_C_REG_INT1_CTRL_FULL_FLAG_ENABLED 0x01

#define LSM6DS3TR_C_REG_INT1_CTRL_FIFO_OVERRUN_DISABLED 0x00
#define LSM6DS3TR_C_REG_INT1_CTRL_FIFO_OVERRUN_ENABLED 0x01

#define LSM6DS3TR_C_REG_INT1_CTRL_FTH_DISABLED 0x00
#define LSM6DS3TR_C_REG_INT1_CTRL_FTH_ENABLED 0x01

#define LSM6DS3TR_C_REG_INT1_CTRL_BOOT_DISABLED 0x00
#define LSM6DS3TR_C_REG_INT1_CTRL_BOOT_ENABLED 0x01

#define LSM6DS3TR_C_REG_INT1_CTRL_DRDY_G_DISABLED 0x00
#define LSM6DS3TR_C_REG_INT1_CTRL_DRDY_G_ENABLED 0x01

#define LSM6DS3TR_C_REG_INT1_CTRL_DRDY_XL_DISABLED 0x00
#define LSM6DS3TR_C_REG_INT1_CTRL_DRDY_XL_ENABLED 0x01

typedef struct {
    // [7] Pedometer step recognition interrupt enable on INT1 pad
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t step_detector;

    // [6] Significant motion interrupt enable on INT1 pad
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t sign_mot;

    // [5] FIFO full flag interrupt enable on INT1 pad
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t full_flag;

    // [4] FIFO overrun interrupt on INT1 pad
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t fifo_ovr;

    // [3] FIFO threshold interrupt on INT1 pad
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t fth;

    // [2] Boot status available on INT1 pad
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t boot;

    // [1] Gyroscope Data Ready on INT1 pad
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t drdy_g;

    // [0] Accelerometer Data Ready on INT1 pad
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t drdy_xl;
} lsm6ds3tr_c_reg_int1_ctrl_t;

lsm6ds3tr_c_reg_int1_ctrl_t lsm6ds3tr_c_reg_new_int1_ctrl(void);
void lsm6ds3tr_c_reg_set_int1_ctrl(lsm6ds3tr_c_reg_int1_ctrl_t* int1_ctrl);
void lsm6ds3tr_c_reg_get_int1_ctrl(lsm6ds3tr_c_reg_int1_ctrl_t* int1_ctrl);

#endif
