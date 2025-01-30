#ifndef __ICM42688_REGS_GYRO_ACCEL_CONFIG0_H
#define __ICM42688_REGS_GYRO_ACCEL_CONFIG0_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

#define __ICM42688_REG_GYRO_ACCEL_CONFIG0_DEFAULT_VALUE 0x11

#define ICM42688_REG_GYRO_ACCEL_CONFIG0_ACCEL_UI_FILT_BW_ODR_2 0x00
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_ACCEL_UI_FILT_BW_MAX_400HZ_ODR_4 0x01
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_ACCEL_UI_FILT_BW_MAX_400HZ_ODR_5 0x02
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_ACCEL_UI_FILT_BW_MAX_400HZ_ODR_8 0x03
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_ACCEL_UI_FILT_BW_MAX_400HZ_ODR_10 0x04
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_ACCEL_UI_FILT_BW_MAX_400HZ_ODR_16 0x05
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_ACCEL_UI_FILT_BW_MAX_400HZ_ODR_20 0x06
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_ACCEL_UI_FILT_BW_MAX_400HZ_ODR_40 0x07
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_ACCEL_UI_FILT_BW_LOW_LATENCY_ODR_DEC2 0x14
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_ACCEL_UI_FILT_BW_LOW_LATENCY_ODR_DEC2_8 0x15

#define ICM42688_REG_GYRO_ACCEL_CONFIG0_GYRO_UI_FILT_BW_ODR_2 0x00
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_GYRO_UI_FILT_BW_MAX_400HZ_ODR_4 0x01
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_GYRO_UI_FILT_BW_MAX_400HZ_ODR_5 0x02
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_GYRO_UI_FILT_BW_MAX_400HZ_ODR_8 0x03
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_GYRO_UI_FILT_BW_MAX_400HZ_ODR_10 0x04
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_GYRO_UI_FILT_BW_MAX_400HZ_ODR_16 0x05
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_GYRO_UI_FILT_BW_MAX_400HZ_ODR_20 0x06
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_GYRO_UI_FILT_BW_MAX_400HZ_ODR_40 0x07
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_GYRO_UI_FILT_BW_LOW_LATENCY_ODR_DEC2 0x14
#define ICM42688_REG_GYRO_ACCEL_CONFIG0_GYRO_UI_FILT_BW_LOW_LATENCY_ODR_DEC2_8 0x15

typedef struct {
    // Bandwidth for Accel LPF
    // 0x00: BW=ODR/2 in LN mode
    // 0x01: BW=max(400Hz, ODR)/4 in LN mode, 1x AVG filter in LP mode (default)
    // 0x02: BW=max(400Hz, ODR)/5 in LN mode
    // 0x03: BW=max(400Hz, ODR)/8 in LN mode
    // 0x04: BW=max(400Hz, ODR)/10 in LN mode
    // 0x05: BW=max(400Hz, ODR)/16 in LN mode
    // 0x06: BW=max(400Hz, ODR)/20 in LN mode, 16x AVG filter in LP mode
    // 0x07: BW=max(400Hz, ODR)/40 in LN mode
    // 0x14: Low Latency option: Trivial decimation @ ODR of Dec2 filter output. Dec2 runs at max(400Hz, ODR) in LN mode
    // 0x15: Low Latency option: Trivial decimation @ ODR of Dec2 filter output. Dec2 runs at max(400Hz, 8*ODR) in LN mode
    uint8_t accel_ui_filt_bw;
    // Bandwidth for Gyro LPF
    // 0x00: BW=ODR/2 in LN mode
    // 0x01: BW=max(400Hz, ODR)/4 in LN mode
    // 0x02: BW=max(400Hz, ODR)/5 in LN mode
    // 0x03: BW=max(400Hz, ODR)/8 in LN mode
    // 0x04: BW=max(400Hz, ODR)/10 in LN mode
    // 0x05: BW=max(400Hz, ODR)/16 in LN mode
    // 0x06: BW=max(400Hz, ODR)/20 in LN mode
    // 0x07: BW=max(400Hz, ODR)/40 in LN mode
    // 0x14: Low Latency option: Trivial decimation @ ODR of Dec2 filter output. Dec2 runs at max(400Hz, ODR) in LN mode
    // 0x15: Low Latency option: Trivial decimation @ ODR of Dec2 filter output. Dec2 runs at max(400Hz, 8*ODR) in LN mode
    uint8_t gyro_ui_filt_bw;
} icm42688_reg_gyro_accel_config0_t;

icm42688_reg_gyro_accel_config0_t icm42688_reg_new_gyro_accel_config0(void);
void icm42688_reg_set_gyro_accel_config0(icm42688_reg_gyro_accel_config0_t* gyro_accel_config0);
void icm42688_reg_get_gyro_accel_config0(icm42688_reg_gyro_accel_config0_t* gyro_accel_config0);

#endif
