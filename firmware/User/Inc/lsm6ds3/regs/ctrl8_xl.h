#ifndef __LSM6DS3_REGS_CTRL8_XL_H
#define __LSM6DS3_REGS_CTRL8_XL_H

#include <stdint.h>

#include "User/Inc/lsm6ds3/reg_map.h"
#include "User/Inc/lsm6ds3/utils.h"

#define __LSM6DS3_REG_CTRL8_XL_DEFAULT_VALUE 0x00

#define LSM6DS3_REG_CTRL8_XL_LPF2_XL_EN_DISABLE 0x00
#define LSM6DS3_REG_CTRL8_XL_LPF2_XL_EN_ENABLE 0x01

#define LSM6DS3_REG_CTRL8_XL_HPCF_XL_SLOPE_HPF_ODR_4 0x00
#define LSM6DS3_REG_CTRL8_XL_HPCF_XL_SLOPE_HPF_ODR_100 0x01
#define LSM6DS3_REG_CTRL8_XL_HPCF_XL_SLOPE_HPF_ODR_9 0x02
#define LSM6DS3_REG_CTRL8_XL_HPCF_XL_SLOPE_HPF_ODR_400 0x03

#define LSM6DS3_REG_CTRL8_XL_HPCF_XL_LPF2_ODR_50 0x00
#define LSM6DS3_REG_CTRL8_XL_HPCF_XL_LPF2_ODR_100 0x01
#define LSM6DS3_REG_CTRL8_XL_HPCF_XL_LPF2_ODR_9 0x02
#define LSM6DS3_REG_CTRL8_XL_HPCF_XL_LPF2_ODR_400 0x03

#define LSM6DS3_REG_CTRL8_XL_HP_SLOPE_XL_EN_DISABLE 0x00
#define LSM6DS3_REG_CTRL8_XL_HP_SLOPE_XL_EN_ENABLE 0x01

#define LSM6DS3_REG_CTRL8_XL_LOW_PASS_ON_6D_DISABLE 0x00
#define LSM6DS3_REG_CTRL8_XL_LOW_PASS_ON_6D_ENABLE 0x01

typedef struct {
    // Accelerometer low-pass filter LPF2 selection
    // 0x00: LPF2 off (default)
    // 0x01: LPF2 on
    uint8_t lpf2_xl_en;
    // Accelerometer slope filter and high-pass filter configuration and cutoff
    // setting. It is also used to select the cutoff frequency of the
    // LPF2 filter. This low-pass filter can also be used in the
    // 6D/4D functionality by setting the LOW_PASS_ON_6D bit of CTRL8_XL
    // (17h) to 1
    // 0x00: Slope filter and high-pass filter: ODR_XL/4; Low-pass filter: ODR_XL/50 (default)
    // 0x01: Slope filter and high-pass filter: ODR_XL/100; Low-pass filter: ODR_XL/100
    // 0x02: Slope filter and high-pass filter: ODR_XL/9; Low-pass filter: ODR_XL/9
    // 0x03: Slope filter and high-pass filter: ODR_XL/400; Low-pass filter: ODR_XL/400
    uint8_t hpcf_xl;
    // Accelerometer slope filter / high-pass filter selection
    // Refer to Figure 6
    uint8_t hp_slope_xl_en;
    // Low-pass filter on 6D function selection
    // Refer to Figure 6
    uint8_t low_pass_on_6d;
} lsm6ds3_reg_ctrl8_xl_t;

lsm6ds3_reg_ctrl8_xl_t lsm6ds3_reg_new_ctrl8_xl(void);
void lsm6ds3_reg_set_ctrl8_xl(lsm6ds3_reg_ctrl8_xl_t* ctrl8_xl);
void lsm6ds3_reg_get_ctrl8_xl(lsm6ds3_reg_ctrl8_xl_t* ctrl8_xl);

#endif
