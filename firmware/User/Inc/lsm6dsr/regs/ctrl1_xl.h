#ifndef __LSM6DSR_REGS_CTRL1_XL_H
#define __LSM6DSR_REGS_CTRL1_XL_H

#include <stdint.h>

#include "User/Inc/lsm6dsr/reg_map.h"
#include "User/Inc/lsm6dsr/utils.h"

#define __LSM6DSR_REG_CTRL1_XL_DEFAULT_VALUE 0x00

#define LSM6DSR_REG_CTRL1_XL_ODR_XL_POWER_DOWN 0x00
#define LSM6DSR_REG_CTRL1_XL_ODR_XL_1_6HZ 0x0B
#define LSM6DSR_REG_CTRL1_XL_ODR_XL_12_5HZ 0x01
#define LSM6DSR_REG_CTRL1_XL_ODR_XL_26HZ 0x02
#define LSM6DSR_REG_CTRL1_XL_ODR_XL_52HZ 0x03
#define LSM6DSR_REG_CTRL1_XL_ODR_XL_104HZ 0x04
#define LSM6DSR_REG_CTRL1_XL_ODR_XL_208HZ 0x05
#define LSM6DSR_REG_CTRL1_XL_ODR_XL_416HZ 0x06
#define LSM6DSR_REG_CTRL1_XL_ODR_XL_833HZ 0x07
#define LSM6DSR_REG_CTRL1_XL_ODR_XL_1660HZ 0x08
#define LSM6DSR_REG_CTRL1_XL_ODR_XL_3330HZ 0x09
#define LSM6DSR_REG_CTRL1_XL_ODR_XL_6660HZ 0x0A

#define LSM6DSR_REG_CTRL1_XL_FS_XL_2G 0x00
#define LSM6DSR_REG_CTRL1_XL_FS_XL_16G 0x01
#define LSM6DSR_REG_CTRL1_XL_FS_XL_4G 0x02
#define LSM6DSR_REG_CTRL1_XL_FS_XL_8G 0x03

#define LSM6DSR_REG_CTRL1_XL_LPF2_XL_EN_1ST_STAGE 0x00
#define LSM6DSR_REG_CTRL1_XL_LPF2_XL_EN_2ND_STAGE 0x01

typedef struct {
    // Output data rate and power mode selection
    // 0x00: Power-down mode (default)
    // 0x0B: 1.6 Hz low-power mode, 12.5 Hz high performance mode
    // 0x01: 12.5 Hz low-power mode, 12.5 Hz high performance mode
    // 0x02: 26 Hz low-power mode, 26 Hz high performance mode
    // 0x03: 52 Hz low-power mode, 52 Hz high performance mode
    // 0x04: 104 Hz low-power mode, 104 Hz high performance mode
    // 0x05: 208 Hz low-power mode, 208 Hz high performance mode
    // 0x06: 416 Hz low-power mode, 416 Hz high performance mode
    // 0x07: 833 Hz low-power mode, 833 Hz high performance mode
    // 0x08: 1660 Hz low-power mode, 1660 Hz high performance mode
    // 0x09: 3330 Hz low-power mode, 3330 Hz high performance mode
    // 0x0A: 6660 Hz low-power mode, 6660 Hz high performance mode
    uint8_t odr_xl;
    // Accelerometer full-scale selection
    // 0x00: +-2 g (default)
    // 0x01: +-16 g
    // 0x02: +-4 g
    // 0x03: +-8 g
    uint8_t fs_xl;
    // Accelerometer high-resolution selection
    // 0x00: output from first stage digital filtering selected (default)
    // 0x01: output from LPF2 second filtering stage selected
    uint8_t lpf2_xl_en;
} lsm6dsr_reg_ctrl1_xl_t;

lsm6dsr_reg_ctrl1_xl_t lsm6dsr_reg_new_ctrl1_xl(void);
void lsm6dsr_reg_set_ctrl1_xl(lsm6dsr_reg_ctrl1_xl_t* ctrl1_xl);
void lsm6dsr_reg_get_ctrl1_xl(lsm6dsr_reg_ctrl1_xl_t* ctrl1_xl);

#endif
