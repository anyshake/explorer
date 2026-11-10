#ifndef __ICM42688_REGS_DRIVE_CONFIG_H
#define __ICM42688_REGS_DRIVE_CONFIG_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

// Register reset value
#define __ICM42688_REG_DRIVE_CONFIG_DEFAULT_VALUE 0x05

#define ICM42688_REG_DRIVE_CONFIG_I2C_SLEW_RATE_20_60_NS 0x00
#define ICM42688_REG_DRIVE_CONFIG_I2C_SLEW_RATE_12_36_NS 0x01
#define ICM42688_REG_DRIVE_CONFIG_I2C_SLEW_RATE_6_18_NS 0x02
#define ICM42688_REG_DRIVE_CONFIG_I2C_SLEW_RATE_4_12_NS 0x03
#define ICM42688_REG_DRIVE_CONFIG_I2C_SLEW_RATE_2_6_NS 0x04
#define ICM42688_REG_DRIVE_CONFIG_I2C_SLEW_RATE_0_2_NS 0x05

#define ICM42688_REG_DRIVE_CONFIG_SPI_SLEW_RATE_20_60_NS 0x00
#define ICM42688_REG_DRIVE_CONFIG_SPI_SLEW_RATE_12_36_NS 0x01
#define ICM42688_REG_DRIVE_CONFIG_SPI_SLEW_RATE_6_18_NS 0x02
#define ICM42688_REG_DRIVE_CONFIG_SPI_SLEW_RATE_4_12_NS 0x03
#define ICM42688_REG_DRIVE_CONFIG_SPI_SLEW_RATE_2_6_NS 0x04
#define ICM42688_REG_DRIVE_CONFIG_SPI_SLEW_RATE_0_2_NS 0x05

typedef struct {
    // [5:3] Controls slew rate for output pin 14 in I2C mode only
    // 0x00: 20 ns-60 ns (default)
    // 0x01: 12 ns-36 ns
    // 0x02: 6 ns-18 ns
    // 0x03: 4 ns-12 ns
    // 0x04: 2 ns-6 ns
    // 0x05: < 2 ns
    uint8_t i2c_slew_rate;

    // [2:0] Controls slew rate for output pin 14 in SPI or I3CSM mode,
    // and for all other output pins
    // 0x00: 20 ns-60 ns
    // 0x01: 12 ns-36 ns
    // 0x02: 6 ns-18 ns
    // 0x03: 4 ns-12 ns
    // 0x04: 2 ns-6 ns
    // 0x05: < 2 ns (default)
    uint8_t spi_slew_rate;
} icm42688_reg_drive_config_t;

icm42688_reg_drive_config_t icm42688_reg_new_drive_config(void);
void icm42688_reg_set_drive_config(icm42688_reg_drive_config_t* drive_config);
void icm42688_reg_get_drive_config(icm42688_reg_drive_config_t* drive_config);

#endif
