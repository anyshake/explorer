#include "User/Inc/lsm6ds3tr_c/utils.h"

void lsm6ds3tr_c_init(lsm6ds3tr_c_ints_pin_t pin) {
    mcu_utils_gpio_mode(pin.int_1, GPIO_MODE_INPUT);
    mcu_utils_gpio_mode(pin.int_2, GPIO_MODE_INPUT);
}

void lsm6ds3tr_c_reset(bool is_rtos) {
    // Restore the user bank after an MCU-only reset.
    lsm6ds3tr_c_write_reg(LSM6DS3TR_C_REG_FUNC_CFG_ACCESS, 0x00);
    lsm6ds3tr_c_reg_ctrl3_c_t ctrl3_c = lsm6ds3tr_c_reg_new_ctrl3_c();
    ctrl3_c.sw_reset = LSM6DS3TR_C_REG_CTRL3_C_SW_RESET_RESET_DEVICE;
    lsm6ds3tr_c_reg_set_ctrl3_c(&ctrl3_c);
    mcu_utils_delay_ms(100, is_rtos);
}

void lsm6ds3tr_c_wait(void) {
    lsm6ds3tr_c_reg_status_reg_t status_reg;
    do {
        lsm6ds3tr_c_reg_get_status_reg(&status_reg);
    } while (!status_reg.xlda);
}

void lsm6ds3tr_c_read_reg(uint8_t reg, uint8_t* rx_data) {
    lsm6ds3tr_c_read_regs(reg, rx_data, 1);
}

void lsm6ds3tr_c_read_regs(uint8_t reg, uint8_t* rx_data, uint16_t rx_len) {
    mcu_utils_i2c1_read_mem(LSM6DS3TR_C_I2C_ADDRESS, reg, rx_data, rx_len, MCU_UTILS_I2C1_REG_WIDTH_8);
}

void lsm6ds3tr_c_write_reg(uint8_t reg, uint8_t tx_data) {
    mcu_utils_i2c1_write_mem(LSM6DS3TR_C_I2C_ADDRESS, reg, &tx_data, sizeof(tx_data), MCU_UTILS_I2C1_REG_WIDTH_8);
}
