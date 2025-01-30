#include "User/Inc/icm42688/regs/drive_config.h"

icm42688_reg_drive_config_t icm42688_reg_new_drive_config(void) {
    icm42688_reg_drive_config_t drive_config;
    drive_config.i2c_slew_rate = ICM42688_REG_DRIVE_CONFIG_I2C_SLEW_RATE_20_60_NS;
    drive_config.spi_slew_rate = ICM42688_REG_DRIVE_CONFIG_SPI_SLEW_RATE_0_2_NS;
    return drive_config;
}

void icm42688_reg_set_drive_config(icm42688_reg_drive_config_t* drive_config) {
    uint8_t drive_config_data = __ICM42688_REG_DRIVE_CONFIG_DEFAULT_VALUE;
    drive_config_data &= ~(0x07 << 3);
    drive_config_data |= (drive_config->i2c_slew_rate & 0x07) << 3;
    drive_config_data &= ~0x07;
    drive_config_data |= (drive_config->spi_slew_rate & 0x07);
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    icm42688_write_reg(ICM42688_REG_DRIVE_CONFIG, drive_config_data);
}

void icm42688_reg_get_drive_config(icm42688_reg_drive_config_t* drive_config) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    uint8_t drive_config_data = 0;
    icm42688_read_reg(ICM42688_REG_DRIVE_CONFIG, &drive_config_data);
    drive_config->i2c_slew_rate = (drive_config_data >> 3) & 0x07;
    drive_config->spi_slew_rate = drive_config_data & 0x07;
}
