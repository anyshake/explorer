#ifndef __ICM42688_REGS_PWR_MGMT0_H
#define __ICM42688_REGS_PWR_MGMT0_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

#define __ICM42688_REG_PWR_MGMT0_DEFAULT_VALUE 0x00

#define ICM42688_REG_PWR_MGMT0_TEMP_DIS_ENABLED 0x00
#define ICM42688_REG_PWR_MGMT0_TEMP_DIS_DISABLED 0x01

#define ICM42688_REG_PWR_MGMT0_IDLE_ENABLED 0x00
#define ICM42688_REG_PWR_MGMT0_IDLE_DISABLED 0x01

#define ICM42688_REG_PWR_MGMT0_GYRO_MODE_OFF 0x00
#define ICM42688_REG_PWR_MGMT0_GYRO_MODE_STANDBY 0x01
#define ICM42688_REG_PWR_MGMT0_GYRO_MODE_LOW_NOISE 0x03

#define ICM42688_REG_PWR_MGMT0_ACCEL_MODE_OFF 0x00
#define ICM42688_REG_PWR_MGMT0_ACCEL_MODE_LOW_POWER 0x02
#define ICM42688_REG_PWR_MGMT0_ACCEL_MODE_LOW_NOISE 0x03

typedef struct {
    // Temperature sensor enable / disable
    // 0x00: Temperature sensor is enabled (default)
    // 0x01: Temperature sensor is disabled
    uint8_t temp_dis;
    // 0x00: the RC oscillator is powered on even if Accel and Gyro are powered off (default)
    // 0x01: when Accel and Gyro are powered off, the chip will go to OFF state, since the RC oscillator will also be powered off
    uint8_t idle;
    // Gyroscope needs to be kept ON for a minimum of 45ms. When transitioning from OFF to any of the other modes, do not issue any register writes for 200μs
    // 0x00: Turns gyroscope off (default)
    // 0x01: Places gyroscope in Standby Mode
    // 0x03: Places gyroscope in Low Noise Mode
    uint8_t gyro_mode;
    // When transitioning from OFF to any of the other modes, do not issue any register writes for 200μs
    // 0x00: Turns accelerometer off (default)
    // 0x01: Turns accelerometer off
    // 0x02: Places accelerometer in Low Power (LP) Mode
    // 0x03: Places accelerometer in Low Noise (LN) Mode
    uint8_t accel_mode;
} icm42688_reg_pwr_mgmt0_t;

icm42688_reg_pwr_mgmt0_t icm42688_reg_new_pwr_mgmt0(void);
void icm42688_reg_set_pwr_mgmt0(icm42688_reg_pwr_mgmt0_t* pwr_mgmt0);
void icm42688_reg_get_pwr_mgmt0(icm42688_reg_pwr_mgmt0_t* pwr_mgmt0);

#endif
