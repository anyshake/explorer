#include "User/Inc/icm42688/utils.h"

void icm42688_init(icm42688_ints_pin_t pin) {
    mcu_utils_gpio_mode(pin.int_1, GPIO_MODE_INPUT);
    mcu_utils_gpio_mode(pin.int_2, GPIO_MODE_INPUT);
}

void icm42688_reset(bool is_rtos) {
    icm42688_reg_device_config_t device_config = icm42688_reg_new_device_config();
    device_config.soft_reset_config = ICM42688_REG_DEVICE_CONFIG_SOFT_RESET_CONFIG_ENABLE;
    icm42688_reg_set_device_config(&device_config);
    mcu_utils_delay_ms(100, is_rtos);
}

void icm42688_wait(void) {
    icm42688_reg_int_status_t int_status;
    do {
        icm42688_reg_get_int_status(&int_status);
    } while (!int_status.data_rdy_int);
}

void icm42688_read_reg(uint8_t reg, uint8_t* rx_data) {
    mcu_utils_i2c_read_mem(ICM42688_I2C_ADDRES, reg, rx_data, 1,
                           MCU_UTILS_I2C_REG_WIDTH_8);
}

void icm42688_write_reg(uint8_t reg, uint8_t tx_data) {
    mcu_utils_i2c_write_mem(ICM42688_I2C_ADDRES, reg, &tx_data, sizeof(tx_data),
                            MCU_UTILS_I2C_REG_WIDTH_8);
}
