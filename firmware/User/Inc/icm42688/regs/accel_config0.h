#ifndef __ICM42688_REGS_ACCEL_CONFIG0_H
#define __ICM42688_REGS_ACCEL_CONFIG0_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

#define __ICM42688_REG_ACCEL_CONFIG0_DEFAULT_VALUE 0x06

#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_FS_SEL_16G 0x00
#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_FS_SEL_8G 0x01
#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_FS_SEL_4G 0x02
#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_FS_SEL_2G 0x03

#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_32_KHZ 0x01
#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_16_KHZ 0x02
#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_8_KHZ 0x03
#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_4_KHZ 0x04
#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_2_KHZ 0x05
#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_1_KHZ 0x06
#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_200_HZ 0x07
#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_100_HZ 0x08
#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_50_HZ 0x09
#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_25_HZ 0x0A
#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_12_5_HZ 0x0B
#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_6_25_HZ 0x0C
#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_3_125_HZ 0x0D
#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_1_5625_HZ 0x0E
#define ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_500_HZ 0x0F

typedef struct {
    // Full scale select for accelerometer UI interface output
    // 0x00: ±16g (default)
    // 0x01: ±8g
    // 0x02: ±4g
    // 0x03: ±2g
    uint8_t accel_fs_sel;
    // Accelerometer ODR selection for UI interface output
    // 0x01: 32kHz (LN mode)
    // 0x02: 16kHz (LN mode)
    // 0x03: 8kHz (LN mode)
    // 0x04: 4kHz (LN mode)
    // 0x05: 2kHz (LN mode)
    // 0x06: 1kHz (LN mode) (default)
    // 0x07: 200Hz (LP or LN mode)
    // 0x08: 100Hz (LP or LN mode)
    // 0x09: 50Hz (LP or LN mode)
    // 0x0A: 25Hz (LP or LN mode)
    // 0x0B: 12.5Hz (LP or LN mode)
    // 0x0C: 6.25Hz (LP mode)
    // 0x0D: 3.125Hz (LP mode)
    // 0x0E: 1.5625Hz (LP mode)
    // 0x0F: 500Hz (LP or LN mode)
    uint8_t accel_odr;
} icm42688_reg_accel_config0_t;

icm42688_reg_accel_config0_t icm42688_reg_new_accel_config0(void);
void icm42688_reg_set_accel_config0(icm42688_reg_accel_config0_t* accel_config0);
void icm42688_reg_get_accel_config0(icm42688_reg_accel_config0_t* accel_config0);

#endif
