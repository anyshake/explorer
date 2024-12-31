#ifndef __LSM6DS3_REGS_CTRL1_XL_H
#define __LSM6DS3_REGS_CTRL1_XL_H

#include <stdint.h>

#include "User/Inc/lsm6ds3/reg_map.h"
#include "User/Inc/lsm6ds3/utils.h"

#define __LSM6DS3_CTRL1_XL_DEFAULT_VALUE 0x00

#define LSM6DS3_CTRL1_XL_ODR_XL_POWER_DOWN 0x00
#define LSM6DS3_CTRL1_XL_ODR_XL_1_6HZ 0x0B
#define LSM6DS3_CTRL1_XL_ODR_XL_12_5HZ 0x01
#define LSM6DS3_CTRL1_XL_ODR_XL_26HZ 0x02
#define LSM6DS3_CTRL1_XL_ODR_XL_52HZ 0x03
#define LSM6DS3_CTRL1_XL_ODR_XL_104HZ 0x04
#define LSM6DS3_CTRL1_XL_ODR_XL_208HZ 0x05
#define LSM6DS3_CTRL1_XL_ODR_XL_416HZ 0x06
#define LSM6DS3_CTRL1_XL_ODR_XL_833HZ 0x07
#define LSM6DS3_CTRL1_XL_ODR_XL_1660HZ 0x08
#define LSM6DS3_CTRL1_XL_ODR_XL_3330HZ 0x09
#define LSM6DS3_CTRL1_XL_ODR_XL_6660HZ 0x0A

#define LSM6DS3_CTRL1_XL_FS_XL_2G 0x00
#define LSM6DS3_CTRL1_XL_FS_XL_16G 0x01
#define LSM6DS3_CTRL1_XL_FS_XL_4G 0x02
#define LSM6DS3_CTRL1_XL_FS_XL_8G 0x03

#define LSM6DS3_CTRL1_XL_LPF1_BW_SEL 0x00

#define LSM6DS3_CTRL1_XL_BW0_XL_1500HZ 0x00
#define LSM6DS3_CTRL1_XL_BW0_XL_400HZ 0x01

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
    // Accelerometer digital LPF (LPF1) bandwidth selection
    // For bandwidth selection, refer to CTRL8_XL
    uint8_t lpf1_bw_sel;
    // Accelerometer analog chain bandwidth selection (only for accelerometer
    // ODR ≥ 1670 Hz)
    // 0x00: 1500 Hz (default)
    // 0x01: 400 Hz
    uint8_t bw0_xl;
} lsm6ds3_reg_ctrl1_xl_t;

void lsm6ds3_reg_set_ctrl1_xl(lsm6ds3_reg_ctrl1_xl_t* ctrl1_xl);
void lsm6ds3_reg_get_ctrl1_xl(lsm6ds3_reg_ctrl1_xl_t* ctrl1_xl);

#endif
