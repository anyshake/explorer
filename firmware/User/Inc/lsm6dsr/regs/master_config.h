#ifndef __LSM6DSR_REGS_MASTER_CONFIG_H
#define __LSM6DSR_REGS_MASTER_CONFIG_H

#include <stdint.h>

#include "User/Inc/lsm6dsr/reg_map.h"
#include "User/Inc/lsm6dsr/utils.h"

// Register reset value
#define __LSM6DSR_REG_MASTER_CONFIG_DEFAULT_VALUE 0x00

#define LSM6DSR_REG_MASTER_CONFIG_RST_MASTER_REGS_NOP 0x00
#define LSM6DSR_REG_MASTER_CONFIG_RST_MASTER_REGS_RESET 0x01

#define LSM6DSR_REG_MASTER_CONFIG_WRITE_ONCE_EACH_CYCLE 0x00
#define LSM6DSR_REG_MASTER_CONFIG_WRITE_ONCE_1ST_CYCLE 0x01

#define LSM6DSR_REG_MASTER_CONFIG_START_CONFIG_SENSOR_DRDY 0x00
#define LSM6DSR_REG_MASTER_CONFIG_START_CONFIG_INT2_PIN 0x01

#define LSM6DSR_REG_MASTER_CONFIG_PASS_THROUGH_MODE_DISABLED 0x00
#define LSM6DSR_REG_MASTER_CONFIG_PASS_THROUGH_MODE_ENABLED 0x01

#define LSM6DSR_REG_MASTER_CONFIG_SHUB_PU_EN_DISABLED 0x00
#define LSM6DSR_REG_MASTER_CONFIG_SHUB_PU_EN_ENABLED 0x01

#define LSM6DSR_REG_MASTER_CONFIG_MASTER_ON_DISABLED 0x00
#define LSM6DSR_REG_MASTER_CONFIG_MASTER_ON_ENABLED 0x01

#define LSM6DSR_REG_MASTER_CONFIG_AUX_SENS_ON_1_SENSOR 0x00
#define LSM6DSR_REG_MASTER_CONFIG_AUX_SENS_ON_2_SENSORS 0x01
#define LSM6DSR_REG_MASTER_CONFIG_AUX_SENS_ON_3_SENSORS 0x02
#define LSM6DSR_REG_MASTER_CONFIG_AUX_SENS_ON_4_SENSORS 0x03

typedef struct {
    // [7] Reset sensor-hub master logic and output registers; write 0x01, then 0x00
    // 0x00: Nop (default)
    // 0x01: Reset
    uint8_t rst_master_regs;

    // [6] Slave 0 write operation timing
    // 0x00: Write operation for each sensor hub cycle (default)
    // 0x01: Write operation only for the first sensor hub cycle
    uint8_t write_once;

    // [5] Sensor hub trigger signal selection.
    // 0x00: Sensor hub trigger signal is the accelerometer/gyro data-ready (default)
    // 0x01: Sensor hub trigger signal external from INT2 pin
    uint8_t start_config;

    // [4] I2C interface pass-through.
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t pass_through_mode;

    // [3] Master I2C pull-up enable.
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t shub_pu_en;

    // [2] Sensor hub I2C master enable.
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t master_on;

    // [1:0] Number of external sensors to be read by the sensor hub
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
