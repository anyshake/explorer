#include "Utils/Inc/i2c.h"

void mcu_utils_i2c_init(bool is_rtos) {
    MX_I2C1_Init();
    mcu_utils_delay_ms(100, is_rtos);
}

void mcu_utils_i2c_end(void) {
    HAL_I2C_DeInit(&hi2c1);
}

void mcu_utils_i2c_read(uint8_t address,
                        uint16_t reg,
                        uint8_t* rx_data,
                        uint16_t rx_len,
                        uint8_t reg_width) {
    if (rx_data == NULL || !rx_len) {
        return;
    }

    HAL_I2C_Mem_Read(&hi2c1, address << 1, reg, reg_width, rx_data, rx_len,
                     1000);
}

void mcu_utils_i2c_write(uint8_t address,
                         uint16_t reg,
                         uint8_t* tx_data,
                         uint16_t tx_len,
                         uint8_t reg_width) {
    if (tx_data == NULL || !tx_len) {
        return;
    }

    HAL_I2C_Mem_Write(&hi2c1, address << 1, reg, reg_width, tx_data, tx_len,
                      1000);
}
