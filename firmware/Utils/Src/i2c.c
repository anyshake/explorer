#include "Utils/Inc/i2c.h"

void mcu_utils_i2c_init(bool is_rtos) {
    if (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_RESET) {
        MX_I2C1_Init();
        HAL_GPIO_WritePin(GPIOB, 6, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, 7, GPIO_PIN_SET);
        SET_BIT(hi2c1.Instance->CR1, I2C_CR1_SWRST);
        CLEAR_BIT(hi2c1.Instance->CR1, I2C_CR1_SWRST);
    }
    mcu_utils_delay_ms(100, is_rtos);
}

void mcu_utils_i2c_end(void) {
    if (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_RESET) {
        HAL_I2C_DeInit(&hi2c1);
    }
}

void mcu_utils_i2c_read(uint8_t address,
                        uint16_t reg,
                        uint8_t* rx_data,
                        uint16_t rx_len,
                        uint8_t reg_width) {
    if (rx_data == NULL || !rx_len) {
        return;
    }

    HAL_I2C_StateTypeDef ret = HAL_I2C_Mem_Read(&hi2c1, address << 1, reg, reg_width, rx_data, rx_len, 1000);
    if (ret != 0) {
        HAL_I2C_DeInit(&hi2c1);
        MX_I2C1_Init();
        return mcu_utils_i2c_read(address, reg, rx_data, rx_len, reg_width);
    }
}

void mcu_utils_i2c_write(uint8_t address,
                         uint16_t reg,
                         uint8_t* tx_data,
                         uint16_t tx_len,
                         uint8_t reg_width) {
    if (tx_data == NULL || !tx_len) {
        return;
    }

    HAL_I2C_StateTypeDef ret = HAL_I2C_Mem_Write(&hi2c1, address << 1, reg, reg_width, tx_data, tx_len, 1000);
    if (ret != 0) {
        HAL_I2C_DeInit(&hi2c1);
        MX_I2C1_Init();
        return mcu_utils_i2c_write(address, reg, tx_data, tx_len, reg_width);
    }
}
