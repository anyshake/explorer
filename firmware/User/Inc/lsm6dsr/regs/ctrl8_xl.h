#ifndef __LSM6DSR_REGS_CTRL8_XL_H
#define __LSM6DSR_REGS_CTRL8_XL_H

#include <stdint.h>

#include "User/Inc/lsm6dsr/reg_map.h"
#include "User/Inc/lsm6dsr/utils.h"

// Register reset value
#define __LSM6DSR_REG_CTRL8_XL_DEFAULT_VALUE 0x00

#define LSM6DSR_REG_CTRL8_XL_HPCF_XL_LPF2_ODR_4 0x00
#define LSM6DSR_REG_CTRL8_XL_HPCF_XL_LPF2_ODR_10 0x01
#define LSM6DSR_REG_CTRL8_XL_HPCF_XL_LPF2_ODR_20 0x02
#define LSM6DSR_REG_CTRL8_XL_HPCF_XL_LPF2_ODR_45 0x03
#define LSM6DSR_REG_CTRL8_XL_HPCF_XL_LPF2_ODR_100 0x04
#define LSM6DSR_REG_CTRL8_XL_HPCF_XL_LPF2_ODR_200 0x05
#define LSM6DSR_REG_CTRL8_XL_HPCF_XL_LPF2_ODR_400 0x06
#define LSM6DSR_REG_CTRL8_XL_HPCF_XL_LPF2_ODR_800 0x07

#define LSM6DSR_REG_CTRL8_XL_HPCF_XL_HPF_ODR_4 0x00
#define LSM6DSR_REG_CTRL8_XL_HPCF_XL_HPF_ODR_10 0x01
#define LSM6DSR_REG_CTRL8_XL_HPCF_XL_HPF_ODR_20 0x02
#define LSM6DSR_REG_CTRL8_XL_HPCF_XL_HPF_ODR_45 0x03
#define LSM6DSR_REG_CTRL8_XL_HPCF_XL_HPF_ODR_100 0x04
#define LSM6DSR_REG_CTRL8_XL_HPCF_XL_HPF_ODR_200 0x05
#define LSM6DSR_REG_CTRL8_XL_HPCF_XL_HPF_ODR_400 0x06
#define LSM6DSR_REG_CTRL8_XL_HPCF_XL_HPF_ODR_800 0x07

#define LSM6DSR_REG_CTRL8_XL_HP_REF_MODE_XL_DISABLED 0x00
#define LSM6DSR_REG_CTRL8_XL_HP_REF_MODE_XL_ENABLED 0x01

#define LSM6DSR_REG_CTRL8_XL_FASTSETTL_MODE_XL_DISABLED 0x00
#define LSM6DSR_REG_CTRL8_XL_FASTSETTL_MODE_XL_ENABLED 0x01

#define LSM6DSR_REG_CTRL8_XL_HP_SLOPE_XL_EN_DISABLED 0x00
#define LSM6DSR_REG_CTRL8_XL_HP_SLOPE_XL_EN_ENABLED 0x01

#define LSM6DSR_REG_CTRL8_XL_LOW_PASS_ON_6D_DISABLED 0x00
#define LSM6DSR_REG_CTRL8_XL_LOW_PASS_ON_6D_ENABLED 0x01

typedef struct {
    // [7:5] Accelerometer LPF2 and HP filter configuration and cutoff setting, see the datasheet filter bandwidth table
    // 0x00: Low-pass filter bandwidth: ODR/4; High-pass filter bandwidth: SLOPE (ODR/4) (default)
    // 0x01: Low-pass filter bandwidth: ODR/10; High-pass filter bandwidth: ODR/10
    // 0x02: Low-pass filter bandwidth: ODR/20; High-pass filter bandwidth: ODR/20
    // 0x03: Low-pass filter bandwidth: ODR/45; High-pass filter bandwidth: ODR/45
    // 0x04: Low-pass filter bandwidth: ODR/100; High-pass filter bandwidth: ODR/100
    // 0x05: Low-pass filter bandwidth: ODR/200; High-pass filter bandwidth: ODR/200
    // 0x06: Low-pass filter bandwidth: ODR/400; High-pass filter bandwidth: ODR/400
    // 0x07: Low-pass filter bandwidth: ODR/800; High-pass filter bandwidth: ODR/800
    uint8_t hpcf_xl;

    // [4] Enables accelerometer high-pass filter reference mode (valid for high-pass path - HP_SLOPE_XL_EN bit
    // must be '1').
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t hp_ref_mode_xl;

    // [3] Enables accelerometer LPF2 and HPF fast-settling mode. The filter settles by the second sample after
    // writing this bit. Active only when exiting power-down mode
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t fastsettl_mode_xl;

    // [2] Accelerometer slope filter / high-pass filter selection
    // 0x00: Low-pass path (default)
    // 0x01: Slope/high-pass path
    uint8_t hp_slope_xl_en;

    // [0] LPF2 on 6D function selection
    // 0x00: ODR/2 low-pass filtered data sent to 6D interrupt function (default)
    // 0x01: LPF2 output data sent to 6D interrupt function
    uint8_t low_pass_on_6d;
} lsm6dsr_reg_ctrl8_xl_t;

lsm6dsr_reg_ctrl8_xl_t lsm6dsr_reg_new_ctrl8_xl(void);
void lsm6dsr_reg_set_ctrl8_xl(lsm6dsr_reg_ctrl8_xl_t* ctrl8_xl);
void lsm6dsr_reg_get_ctrl8_xl(lsm6dsr_reg_ctrl8_xl_t* ctrl8_xl);

#endif
