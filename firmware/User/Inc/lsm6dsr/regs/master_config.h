#ifndef __LSM6DSR_REGS_MASTER_CONFIG_H
#define __LSM6DSR_REGS_MASTER_CONFIG_H

#include <stdint.h>

#include "User/Inc/lsm6dsr/reg_map.h"
#include "User/Inc/lsm6dsr/utils.h"

#define __LSM6DSR_REG_MASTER_CONFIG_DEFAULT_VALUE 0x00

#define LSM6DSR_REG_MASTER_CONFIG_RST_MASTER_REGS_NOP 0
#define LSM6DSR_REG_MASTER_CONFIG_RST_MASTER_REGS_RESET 1

#define LSM6DSR_REG_MASTER_CONFIG_WRITE_ONCE_EACH_CYCLE 0
#define LSM6DSR_REG_MASTER_CONFIG_WRITE_ONCE_1ST_CYCLE 1

#define LSM6DSR_REG_MASTER_CONFIG_START_CONFIG_SENSOR_DRDY 0
#define LSM6DSR_REG_MASTER_CONFIG_START_CONFIG_INT2_PIN 1

#define LSM6DSR_REG_MASTER_CONFIG_PASS_THROUGH_MODE_DISABLED 0
#define LSM6DSR_REG_MASTER_CONFIG_PASS_THROUGH_MODE_ENABLED 1

#define LSM6DSR_REG_MASTER_CONFIG_SHUB_PU_EN_DISABLED 0
#define LSM6DSR_REG_MASTER_CONFIG_SHUB_PU_EN_ENABLED 1

#define LSM6DSR_REG_MASTER_CONFIG_MASTER_ON_DISABLED 0
#define LSM6DSR_REG_MASTER_CONFIG_MASTER_ON_ENABLED 1

#define LSM6DSR_REG_MASTER_CONFIG_AUX_SENS_ON_1_SENSOR 0
#define LSM6DSR_REG_MASTER_CONFIG_AUX_SENS_ON_2_SENSORS 1
#define LSM6DSR_REG_MASTER_CONFIG_AUX_SENS_ON_3_SENSORS 2
#define LSM6DSR_REG_MASTER_CONFIG_AUX_SENS_ON_4_SENSORS 3

typedef struct {
    // Reset Master logic and output registers. Must be set to ‘1’ and then set it to ‘0’. Default value: 0
    // 0x00: nop (default)
    // 0x01: reset
    uint8_t rst_master_regs;
    // Slave 0 write operation is performed only at the first sensor hub cycle.
    // 0x00: write operation for each sensor hub cycle (default)
    // 0x01: write operation only for the first sensor hub cycle
    uint8_t write_once;
    // Sensor hub trigger signal selection.
    // 0x00: sensor hub trigger signal is the accelerometer/gyro data-ready (default)
    // 0x01: sensor hub trigger signal external from INT2 pin
    uint8_t start_config;
    // I2C interface pass-through.
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t pass_through_mode;
    // Master I2C pull-up enable.
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t shub_pu_en;
    // Sensor hub I2C master enable.
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t master_on;
    // Number of external sensors to be read by the sensor hub
    // 0x00: 1 sensor (default)
    // 0x01: 2 sensors
    // 0x02: 3 sensors
    // 0x03: 4 sensors
    uint8_t aux_sens_on;
} lsm6dsr_reg_master_config_t;

lsm6dsr_reg_master_config_t lsm6dsr_reg_new_master_config(void);
void lsm6dsr_reg_set_master_config(lsm6dsr_reg_master_config_t* master_config);
void lsm6dsr_reg_get_master_config(lsm6dsr_reg_master_config_t* master_config);

#endif
