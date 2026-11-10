#ifndef __LSM6DS3TR_C_REGS_CTRL1_XL_H
#define __LSM6DS3TR_C_REGS_CTRL1_XL_H

#include <stdint.h>

#include "User/Inc/lsm6ds3tr_c/reg_map.h"
#include "User/Inc/lsm6ds3tr_c/utils.h"

// Register reset value
#define __LSM6DS3TR_C_REG_CTRL1_XL_DEFAULT_VALUE 0x00

#define LSM6DS3TR_C_REG_CTRL1_XL_ODR_XL_POWER_DOWN 0x00
#define LSM6DS3TR_C_REG_CTRL1_XL_ODR_XL_1_6HZ 0x0B
#define LSM6DS3TR_C_REG_CTRL1_XL_ODR_XL_12_5HZ 0x01
#define LSM6DS3TR_C_REG_CTRL1_XL_ODR_XL_26HZ 0x02
#define LSM6DS3TR_C_REG_CTRL1_XL_ODR_XL_52HZ 0x03
#define LSM6DS3TR_C_REG_CTRL1_XL_ODR_XL_104HZ 0x04
#define LSM6DS3TR_C_REG_CTRL1_XL_ODR_XL_208HZ 0x05
#define LSM6DS3TR_C_REG_CTRL1_XL_ODR_XL_416HZ 0x06
#define LSM6DS3TR_C_REG_CTRL1_XL_ODR_XL_833HZ 0x07
#define LSM6DS3TR_C_REG_CTRL1_XL_ODR_XL_1660HZ 0x08
#define LSM6DS3TR_C_REG_CTRL1_XL_ODR_XL_3330HZ 0x09
#define LSM6DS3TR_C_REG_CTRL1_XL_ODR_XL_6660HZ 0x0A

#define LSM6DS3TR_C_REG_CTRL1_XL_FS_XL_2G 0x00
#define LSM6DS3TR_C_REG_CTRL1_XL_FS_XL_16G 0x01
#define LSM6DS3TR_C_REG_CTRL1_XL_FS_XL_4G 0x02
#define LSM6DS3TR_C_REG_CTRL1_XL_FS_XL_8G 0x03

#define LSM6DS3TR_C_REG_CTRL1_XL_LPF1_BW_SEL_ODR_2 0x00
#define LSM6DS3TR_C_REG_CTRL1_XL_LPF1_BW_SEL_ODR_4 0x01

#define LSM6DS3TR_C_REG_CTRL1_XL_BW0_XL_1500HZ 0x00
#define LSM6DS3TR_C_REG_CTRL1_XL_BW0_XL_400HZ 0x01

typedef struct {
    // [7:4] Output data rate; the power mode also depends on CTRL6_C.XL_HM_MODE
    // 0x00: Power-down mode (default)
    // 0x01: 12.5 Hz
    // 0x02: 26 Hz
    // 0x03: 52 Hz
    // 0x04: 104 Hz
    // 0x05: 208 Hz
    // 0x06: 416 Hz
    // 0x07: 833 Hz
    // 0x08: 1.66 kHz
    // 0x09: 3.33 kHz
    // 0x0A: 6.66 kHz
    // 0x0B: 1.6 Hz in low-power mode; 12.5 Hz in high-performance mode
    uint8_t odr_xl;

    // [3:2] Accelerometer full-scale selection
    // 0x00: ±2 g (default)
    // 0x01: ±16 g
    // 0x02: ±4 g
    // 0x03: ±8 g
    uint8_t fs_xl;

    // [1] LPF1 bandwidth selection; unused when LPF2 is selected
    // 0x00: ODR/2 (default)
    // 0x01: ODR/4
    uint8_t lpf1_bw_sel;

    // [0] Analog bandwidth selection, effective only at ODR >= 1.67 kHz
    // 0x00: 1500 Hz (default)
    // 0x01: 400 Hz
    uint8_t bw0_xl;
} lsm6ds3tr_c_reg_ctrl1_xl_t;

lsm6ds3tr_c_reg_ctrl1_xl_t lsm6ds3tr_c_reg_new_ctrl1_xl(void);
void lsm6ds3tr_c_reg_set_ctrl1_xl(lsm6ds3tr_c_reg_ctrl1_xl_t* ctrl1_xl);
void lsm6ds3tr_c_reg_get_ctrl1_xl(lsm6ds3tr_c_reg_ctrl1_xl_t* ctrl1_xl);

#endif
