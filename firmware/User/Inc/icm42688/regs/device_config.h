#ifndef __ICM42688_REGS_DEVICE_CONFIG_H
#define __ICM42688_REGS_DEVICE_CONFIG_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

#define __ICM42688_REG_DEVICE_CONFIG_DEFAULT_VALUE 0x00

#define ICM42688_REG_DEVICE_CONFIG_SPI_MODE_0_3 0x00
#define ICM42688_REG_DEVICE_CONFIG_SPI_MODE_1_2 0x01

#define ICM42688_REG_DEVICE_CONFIG_SOFT_RESET_CONFIG_NORMAL 0x00
#define ICM42688_REG_DEVICE_CONFIG_SOFT_RESET_CONFIG_ENABLE 0x01

typedef struct {
    // SPI mode selection
    // 0x00: Mode 0 and Mode 3 (default)
    // 0x01: Mode 1 and Mode 2
    uint8_t spi_mode;
    // Accelerometer full-scale selection
    // After writing 1 to this bitfield, wait 1ms for soft reset
    // to be effective, before attempting any other register access
    // 0x00: Normal (default)
    // 0x01: Enable reset
    uint8_t soft_reset_config;
} icm42688_reg_device_config_t;

icm42688_reg_device_config_t icm42688_reg_new_device_config(void);
void icm42688_reg_set_device_config(icm42688_reg_device_config_t* device_config);
void icm42688_reg_get_device_config(icm42688_reg_device_config_t* device_config);

#endif
