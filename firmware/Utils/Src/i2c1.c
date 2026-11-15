#include "Utils/Inc/i2c1.h"

void mcu_utils_i2c1_init(bool is_rtos) {
    if (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_RESET) {
        MX_I2C1_Init();
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
    }
    mcu_utils_delay_ms(100, is_rtos);
}

void mcu_utils_i2c1_end(void) {
    if (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_RESET) {
        HAL_I2C_DeInit(&hi2c1);
    }
}

void mcu_utils_i2c1_read_mem(uint8_t address,
                             uint16_t reg,
                             uint8_t* rx_data,
                             uint16_t rx_len,
                             uint8_t reg_width) {
    if (rx_data == NULL || !rx_len) {
        return;
    }

    // Workaround for HAL V1.1.9 (STM32CubeF1 V1.8.5): HAL_I2C_Mem_Read() with more than 3 bytes
    // reads 2 bytes at once when BTF is set (e.g. task preempted mid-transfer) and can skip the
    // N=3 step that clears ACK. The last byte is then ACKed, the slave keeps SDA low, STOP never
    // completes and I2C1 stays BUSY until IWDG resets the MCU.
    // Chunks of <= 3 bytes never take that path, so the chunk size must not exceed 3; 2 bytes also
    // keeps each 16-bit register in one transaction. Fixed upstream in HAL V1.1.10 (STM32CubeF1 V1.8.6).
#define MCU_UTILS_I2C1_MEM_READ_CHUNK_SIZE 2

    for (uint16_t offset = 0; offset < rx_len; offset += MCU_UTILS_I2C1_MEM_READ_CHUNK_SIZE) {
        uint16_t chunk_len = rx_len - offset;
        if (chunk_len > MCU_UTILS_I2C1_MEM_READ_CHUNK_SIZE) {
            chunk_len = MCU_UTILS_I2C1_MEM_READ_CHUNK_SIZE;
        }
        if (HAL_I2C_Mem_Read(&hi2c1, address << 1, reg + offset, reg_width, rx_data + offset, chunk_len, 1000) != HAL_OK) {
            return;
        }
    }
}

void mcu_utils_i2c1_write_mem(uint8_t address,
                              uint16_t reg,
                              uint8_t* tx_data,
                              uint16_t tx_len,
                              uint8_t reg_width) {
    if (tx_data == NULL || !tx_len) {
        return;
    }

    HAL_I2C_Mem_Write(&hi2c1, address << 1, reg, reg_width, tx_data, tx_len, 1000);
}

void mcu_utils_i2c1_read(uint8_t address, uint8_t* rx_data, uint16_t rx_len) {
    if (rx_data == NULL || !rx_len) {
        return;
    }

    HAL_I2C_Master_Receive(&hi2c1, address << 1, rx_data, rx_len, 1000);
}

void mcu_utils_i2c1_write(uint8_t address, uint8_t* tx_data, uint16_t tx_len) {
    if (tx_data == NULL || !tx_len) {
        return;
    }

    HAL_I2C_Master_Transmit(&hi2c1, address << 1, tx_data, tx_len, 1000);
}
