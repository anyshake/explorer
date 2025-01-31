#include "User/Inc/lsm6ds3/utils.h"

void lsm6ds3_init(lsm6ds3_ints_pin_t pin) {
    mcu_utils_gpio_mode(pin.int_1, GPIO_MODE_INPUT);
    mcu_utils_gpio_mode(pin.int_2, GPIO_MODE_INPUT);
}

void lsm6ds3_reset(bool is_rtos) {
    lsm6ds3_reg_ctrl3_c_t ctrl3_c = lsm6ds3_reg_new_ctrl3_c();
    ctrl3_c.sw_reset = LSM6DS3_REG_CTRL3_C_SW_RESET_NORMAL_MODE;
    lsm6ds3_reg_set_ctrl3_c(&ctrl3_c);
    mcu_utils_delay_ms(100, is_rtos);
}

void lsm6ds3_wait(lsm6ds3_ints_pin_t pin) {
    while (!mcu_utils_gpio_read(pin.int_1)) {
        ;
    }
}

void lsm6ds3_read_reg(uint8_t reg, uint8_t* rx_data) {
    mcu_utils_i2c_read(LSM6DS3_I2C_ADDRES, reg, rx_data, 1, MCU_UTILS_I2C_REG_WIDTH_8);
}

void lsm6ds3_write_reg(uint8_t reg, uint8_t tx_data) {
    mcu_utils_i2c_write(LSM6DS3_I2C_ADDRES, reg, &tx_data, sizeof(tx_data), MCU_UTILS_I2C_REG_WIDTH_8);
}
