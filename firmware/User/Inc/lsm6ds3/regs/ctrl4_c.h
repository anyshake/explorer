#ifndef __LSM6DS3_REGS_CTRL4_C_H
#define __LSM6DS3_REGS_CTRL4_C_H

#include <stdint.h>

#include "User/Inc/lsm6ds3/reg_map.h"
#include "User/Inc/lsm6ds3/utils.h"

#define __LSM6DS3_REG_CTRL4_C_DEFAULT_VALUE 0x00

#define LSM6DS3_REG_CTRL4_C_XL_BW_SCAL_ODR_BYPASS 0x00
#define LSM6DS3_REG_CTRL4_C_XL_BW_SCAL_ODR_BW_XL 0x01

#define LSM6DS3_REG_CTRL4_C_SLEEP_G_DISABLE 0x00
#define LSM6DS3_REG_CTRL4_C_SLEEP_G_ENABLE 0x01

#define LSM6DS3_REG_CTRL4_C_INT2_ON_INT1_OFF 0x00
#define LSM6DS3_REG_CTRL4_C_INT2_ON_INT1_ON 0x01

#define LSM6DS3_REG_CTRL4_C_FIFO_TEMP_EN_DISABLE 0x00
#define LSM6DS3_REG_CTRL4_C_FIFO_TEMP_EN_ENABLE 0x01

#define LSM6DS3_REG_CTRL4_C_DRDY_MASK_DISABLE 0x00
#define LSM6DS3_REG_CTRL4_C_DRDY_MASK_ENABLE 0x01

#define LSM6DS3_REG_CTRL4_C_I2C_DISABLE_BYPASS 0x00
#define LSM6DS3_REG_CTRL4_C_I2C_DISABLE_ENABLE 0x01

#define LSM6DS3_REG_CTRL4_C_STOP_ON_FTH_DISABLE 0x00
#define LSM6DS3_REG_CTRL4_C_STOP_ON_FTH_ENABLE 0x01

typedef struct {
    // Accelerometer bandwidth selection
    // 0x00: bandwidth determined by ODR selection (default)
    // 0x01: bandwidth determined by setting BW_XL[1:0] in CTRL1_XL (10h) register
    uint8_t xl_bw_scal_odr;
    // Gyroscope sleep mode enable
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t sleep_g;
    // All interrupt signals available on INT1 pad enable
    // 0x00: interrupt signals divided between INT1 and INT2 pads (default)
    // 0x01: all interrupt signals in logic or on INT1 pad
    uint8_t int2_on_int1;
    // Enable temperature data as 4 th FIFO data set
    // 0x00: disable temperature data as 4th FIFO data set (default)
    // 0x01: enable temperature data as 4th FIFO data set
    uint8_t fifo_temp_en;
    // Data-ready mask enable. If enabled, when switching from Power-Down to an
    // active mode, the accelerometer and gyroscope data-ready signals are masked
    // until the settling of the sensor filters is completed
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t drdy_mask;
    // Disable I2C interface
    // 0x00: both I2C and SPI enabled (default)
    // 0x01: I2C disabled, SPI only
    uint8_t i2c_disable;
    // Enable FIFO threshold level use
    // 0x00: FIFO depth is not limited (default)
    // 0x01: FIFO depth is limited to threshold level
    uint8_t stop_on_fth;
} lsm6ds3_reg_ctrl4_c_t;

lsm6ds3_reg_ctrl4_c_t lsm6ds3_reg_new_ctrl4_c(void);
void lsm6ds3_reg_set_ctrl4_c(lsm6ds3_reg_ctrl4_c_t* ctrl4_c);
void lsm6ds3_reg_get_ctrl4_c(lsm6ds3_reg_ctrl4_c_t* ctrl4_c);

#endif
