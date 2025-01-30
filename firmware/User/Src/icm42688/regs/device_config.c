#include "User/Inc/icm42688/regs/device_config.h"

icm42688_reg_device_config_t icm42688_reg_new_device_config(void) {
    icm42688_reg_device_config_t device_config;
    device_config.spi_mode = ICM42688_REG_DEVICE_CONFIG_SPI_MODE_0_3;
    device_config.soft_reset_config = ICM42688_REG_DEVICE_CONFIG_SOFT_RESET_CONFIG_NORMAL;
    return device_config;
}

void icm42688_reg_set_device_config(icm42688_reg_device_config_t* device_config) {
    uint8_t device_config_data = __ICM42688_REG_DEVICE_CONFIG_DEFAULT_VALUE;
    device_config_data &= ~(0x01 << 4);
    device_config_data |= (device_config->spi_mode & 0x01) << 4;
    device_config_data &= ~0x01;
    device_config_data |= (device_config->soft_reset_config & 0x01);
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    icm42688_write_reg(ICM42688_REG_DEVICE_CONFIG, device_config_data);
}

void icm42688_reg_get_device_config(icm42688_reg_device_config_t* device_config) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    uint8_t device_config_data = 0;
    icm42688_read_reg(ICM42688_REG_DEVICE_CONFIG, &device_config_data);
    device_config->spi_mode = (device_config_data >> 4) & 0x01;
    device_config->soft_reset_config = device_config_data & 0x01;
}
