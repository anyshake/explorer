#ifndef __LSM6DS3_REGS_INT1_CTRL_H
#define __LSM6DS3_REGS_INT1_CTRL_H

#include <stdint.h>

#include "User/Inc/lsm6ds3/reg_map.h"
#include "User/Inc/lsm6ds3/utils.h"

#define __LSM6DS3_REG_INT1_CTRL_DEFAULT_VALUE 0x00

#define LSM6DS3_REG_INT1_CTRL_STEP_DETECTOR_DISABLED 0x00
#define LSM6DS3_REG_INT1_CTRL_STEP_DETECTOR_ENABLED 0x01

#define LSM6DS3_REG_INT1_CTRL_SIGN_MOTION_DISABLED 0x00
#define LSM6DS3_REG_INT1_CTRL_SIGN_MOTION_ENABLED 0x01

#define LSM6DS3_REG_INT1_CTRL_FULL_FLAG_DISABLED 0x00
#define LSM6DS3_REG_INT1_CTRL_FULL_FLAG_ENABLED 0x01

#define LSM6DS3_REG_INT1_CTRL_FIFO_OVERRUN_DISABLED 0x00
#define LSM6DS3_REG_INT1_CTRL_FIFO_OVERRUN_ENABLED 0x01

#define LSM6DS3_REG_INT1_CTRL_FTH_DISABLED 0x00
#define LSM6DS3_REG_INT1_CTRL_FTH_ENABLED 0x01

#define LSM6DS3_REG_INT1_CTRL_BOOT_DISABLED 0x00
#define LSM6DS3_REG_INT1_CTRL_BOOT_ENABLED 0x01

#define LSM6DS3_REG_INT1_CTRL_DRDY_G_DISABLED 0x00
#define LSM6DS3_REG_INT1_CTRL_DRDY_G_ENABLED 0x01

#define LSM6DS3_REG_INT1_CTRL_DRDY_XL_DISABLED 0x00
#define LSM6DS3_REG_INT1_CTRL_DRDY_XL_ENABLED 0x01

typedef struct {
    // Pedometer step recognition interrupt enable on INT1 pad
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t step_detector;
    // Significant motion interrupt enable on INT1 pad
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t sign_mot;
    // FIFO full flag interrupt enable on INT1 pad
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t full_flag;
    // FIFO overrun interrupt on INT1 pad
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t fifo_ovr;
    // FIFO threshold interrupt on INT1 pad
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t fth;
    // Boot status available on INT1 pad
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t boot;
    // Gyroscope Data Ready on INT1 pad
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t drdy_g;
    // Accelerometer Data Ready on INT1 pad
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t drdy_xl;
} lsm6ds3_reg_int1_ctrl_t;

lsm6ds3_reg_int1_ctrl_t lsm6ds3_reg_new_int1_ctrl(void);
void lsm6ds3_reg_set_int1_ctrl(lsm6ds3_reg_int1_ctrl_t* int1_ctrl);
void lsm6ds3_reg_get_int1_ctrl(lsm6ds3_reg_int1_ctrl_t* int1_ctrl);

#endif
