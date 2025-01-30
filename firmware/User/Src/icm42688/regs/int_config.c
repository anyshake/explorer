#include "User/Inc/icm42688/regs/int_config.h"

icm42688_reg_int_config_t icm42688_reg_new_int_config(void) {
    icm42688_reg_int_config_t int_config;
    int_config.int2_mode = ICM42688_REG_INT_CONFIG_INT2_MODE_PULSED;
    int_config.int2_drive_circuit = ICM42688_REG_INT_CONFIG_INT2_DRIVE_CIRCUIT_OPEN_DRAIN;
    int_config.int2_polarity = ICM42688_REG_INT_CONFIG_INT2_POLARITY_ACTIVE_LOW;
    int_config.int1_mode = ICM42688_REG_INT_CONFIG_INT1_MODE_PULSED;
    int_config.int1_drive_circuit = ICM42688_REG_INT_CONFIG_INT1_DRIVE_CIRCUIT_OPEN_DRAIN;
    int_config.int1_polarity = ICM42688_REG_INT_CONFIG_INT1_POLARITY_ACTIVE_LOW;
    return int_config;
}

void icm42688_reg_set_int_config(icm42688_reg_int_config_t* int_config) {
    uint8_t int_config_data = __ICM42688_REG_INT_CONFIG_DEFAULT_VALUE;
    int_config_data &= ~(0x01 << 5);
    int_config_data |= (int_config->int2_mode & 0x01) << 5;
    int_config_data &= ~(0x01 << 4);
    int_config_data |= (int_config->int2_drive_circuit & 0x01) << 4;
    int_config_data &= ~(0x01 << 3);
    int_config_data |= (int_config->int2_polarity & 0x01) << 3;
    int_config_data &= ~(0x01 << 2);
    int_config_data |= (int_config->int1_mode & 0x01) << 2;
    int_config_data &= ~(0x01 << 1);
    int_config_data |= (int_config->int1_drive_circuit & 0x01) << 1;
    int_config_data &= ~0x01;
    int_config_data |= (int_config->int1_polarity & 0x01);
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    icm42688_write_reg(ICM42688_REG_INT_CONFIG, int_config_data);
}

void icm42688_reg_get_int_config(icm42688_reg_int_config_t* int_config) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    uint8_t int_config_data = 0;
    icm42688_read_reg(ICM42688_REG_INT_CONFIG, &int_config_data);
    int_config->int2_mode = (int_config_data >> 5) & 0x01;
    int_config->int2_drive_circuit = (int_config_data >> 4) & 0x01;
    int_config->int2_polarity = (int_config_data >> 3) & 0x01;
    int_config->int1_mode = (int_config_data >> 2) & 0x01;
    int_config->int1_drive_circuit = (int_config_data >> 1) & 0x01;
    int_config->int1_polarity = int_config_data & 0x01;
}
