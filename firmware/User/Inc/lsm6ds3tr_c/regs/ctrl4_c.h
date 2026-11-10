#ifndef __LSM6DS3TR_C_REGS_CTRL4_C_H
#define __LSM6DS3TR_C_REGS_CTRL4_C_H

#include <stdint.h>

#include "User/Inc/lsm6ds3tr_c/reg_map.h"
#include "User/Inc/lsm6ds3tr_c/utils.h"

// Register reset value
#define __LSM6DS3TR_C_REG_CTRL4_C_DEFAULT_VALUE 0x00

#define LSM6DS3TR_C_REG_CTRL4_C_DEN_XL_EN_DISABLED 0x00
#define LSM6DS3TR_C_REG_CTRL4_C_DEN_XL_EN_ENABLED 0x01

#define LSM6DS3TR_C_REG_CTRL4_C_SLEEP_DISABLED 0x00
#define LSM6DS3TR_C_REG_CTRL4_C_SLEEP_ENABLED 0x01

#define LSM6DS3TR_C_REG_CTRL4_C_INT2_ON_INT1_OFF 0x00
#define LSM6DS3TR_C_REG_CTRL4_C_INT2_ON_INT1_ON 0x01

#define LSM6DS3TR_C_REG_CTRL4_C_DEN_DRDY_INT1_DISABLED 0x00
#define LSM6DS3TR_C_REG_CTRL4_C_DEN_DRDY_INT1_ENABLED 0x01

#define LSM6DS3TR_C_REG_CTRL4_C_DRDY_MASK_DISABLED 0x00
#define LSM6DS3TR_C_REG_CTRL4_C_DRDY_MASK_ENABLED 0x01

#define LSM6DS3TR_C_REG_CTRL4_C_I2C_DISABLE_BYPASS 0x00
#define LSM6DS3TR_C_REG_CTRL4_C_I2C_DISABLE_ENABLED 0x01

#define LSM6DS3TR_C_REG_CTRL4_C_LPF1_SEL_G_DISABLED 0x00
#define LSM6DS3TR_C_REG_CTRL4_C_LPF1_SEL_G_ENABLED 0x01

typedef struct {
    // [7] Extend DEN functionality to accelerometer
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t den_xl_en;

    // [6] Gyroscope sleep mode enable
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t sleep;

    // [5] All interrupt signals available on INT1 pad enable
    // 0x00: Interrupt signals divided between INT1 and INT2 pads (default)
    // 0x01: All interrupt signals in logic or on INT1 pad
    uint8_t int2_on_int1;

    // [4] Route DEN data-ready to INT1
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t den_drdy_int1;

    // [3] Data-ready mask enable. If enabled, when switching from Power-Down to an
    // active mode, the accelerometer and gyroscope data-ready signals are masked
    // until the settling of the sensor filters is completed
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t drdy_mask;

    // [2] Disable I2C interface
    // 0x00: Both I2C and SPI enabled (default)
    // 0x01: I2C disabled, SPI only
    uint8_t i2c_disable;

    // [1] Enable gyroscope LPF1; bit 0 is reserved and must remain zero
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t lpf1_sel_g;
} lsm6ds3tr_c_reg_ctrl4_c_t;

lsm6ds3tr_c_reg_ctrl4_c_t lsm6ds3tr_c_reg_new_ctrl4_c(void);
void lsm6ds3tr_c_reg_set_ctrl4_c(lsm6ds3tr_c_reg_ctrl4_c_t* ctrl4_c);
void lsm6ds3tr_c_reg_get_ctrl4_c(lsm6ds3tr_c_reg_ctrl4_c_t* ctrl4_c);

#endif
