#ifndef __LSM6DSR_REGS_CTRL4_C_H
#define __LSM6DSR_REGS_CTRL4_C_H

#include <stdint.h>

#include "User/Inc/lsm6dsr/reg_map.h"
#include "User/Inc/lsm6dsr/utils.h"

#define __LSM6DSR_REG_CTRL4_C_DEFAULT_VALUE 0x00

#define LSM6DSR_REG_CTRL4_C_SLEEP_G_DISABLE 0x00
#define LSM6DSR_REG_CTRL4_C_SLEEP_G_ENABLE 0x01

#define LSM6DSR_REG_CTRL4_C_INT2_ON_INT1_OFF 0x00
#define LSM6DSR_REG_CTRL4_C_INT2_ON_INT1_ON 0x01

#define LSM6DSR_REG_CTRL4_C_DRDY_MASK_DISABLE 0x00
#define LSM6DSR_REG_CTRL4_C_DRDY_MASK_ENABLE 0x01

#define LSM6DSR_REG_CTRL4_C_I2C_DISABLE_BYPASS 0x00
#define LSM6DSR_REG_CTRL4_C_I2C_DISABLE_ENABLE 0x01

#define LSM6DSR_REG_CTRL4_C_LPF1_SEL_G_DISABLE 0x00
#define LSM6DSR_REG_CTRL4_C_LPF1_SEL_G_ENABLE 0x01

typedef struct {
    // Gyroscope sleep mode enable
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t sleep_g;
    // All interrupt signals available on INT1 pad enable
    // 0x00: interrupt signals divided between INT1 and INT2 pads (default)
    // 0x01: all interrupt signals in logic or on INT1 pad
    uint8_t int2_on_int1;
    // Data-ready mask enable. If enabled, when switching from Power-Down to an
    // active mode, the accelerometer and gyroscope data-ready signals are masked
    // until the settling of the sensor filters is completed
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t drdy_mask;
    // Disable I2C interface
    // 0x00: SPI, I2C and MIPI I3CSM interfaces enabled (default)
    // 0x01: I2C disabled
    uint8_t i2c_disable;
    // Enables gyroscope digital LPF1
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t lpf1_sel_g;
} lsm6dsr_reg_ctrl4_c_t;

lsm6dsr_reg_ctrl4_c_t lsm6dsr_reg_new_ctrl4_c(void);
void lsm6dsr_reg_set_ctrl4_c(lsm6dsr_reg_ctrl4_c_t* ctrl4_c);
void lsm6dsr_reg_get_ctrl4_c(lsm6dsr_reg_ctrl4_c_t* ctrl4_c);

#endif
