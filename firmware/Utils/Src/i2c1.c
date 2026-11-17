#include "Utils/Inc/i2c1.h"

static void mcu_utils_i2c1_recover_bus(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_SET);

// Half period of the bit-banged clock. HAL_Delay(2) waits 1-2 ms, which is
// slow but valid for every I2C device and needs no calibration.
#define MCU_UTILS_I2C1_RECOVERY_HALF_PERIOD_MS 2

    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    gpio.Mode = GPIO_MODE_OUTPUT_OD;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &gpio);
    HAL_Delay(MCU_UTILS_I2C1_RECOVERY_HALF_PERIOD_MS);

    for (uint8_t i = 0; i < 9 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_RESET; i++) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
        HAL_Delay(MCU_UTILS_I2C1_RECOVERY_HALF_PERIOD_MS);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
        HAL_Delay(MCU_UTILS_I2C1_RECOVERY_HALF_PERIOD_MS);
    }

    // STOP: SDA rises while SCL is high.
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_Delay(MCU_UTILS_I2C1_RECOVERY_HALF_PERIOD_MS);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
    HAL_Delay(MCU_UTILS_I2C1_RECOVERY_HALF_PERIOD_MS);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_Delay(MCU_UTILS_I2C1_RECOVERY_HALF_PERIOD_MS);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
    HAL_Delay(MCU_UTILS_I2C1_RECOVERY_HALF_PERIOD_MS);
}

void mcu_utils_i2c1_init(bool is_rtos) {
    if (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_RESET) {
        // Runs before MX_I2C1_Init(): HAL_I2C_MspInit() then switches the
        // pins back to the I2C1 alternate function.
        mcu_utils_i2c1_recover_bus();
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

// Some STM32F103-compatible MCUs never finish the 2-byte (POS/BTF) reception
// sequence of HAL_I2C_Mem_Read(): BTF never rises, the read times out and
// I2C1 stays BUSY, so every following transfer fails. 1-byte and 3-byte
// reads use other sequences and work, so a 2-byte read is done as a 3-byte
// read of which the last byte is dropped. Callers never read a register with
// read side effects (e.g. a FIFO) as the last byte of a 2-byte read.
static HAL_StatusTypeDef mcu_utils_i2c1_read_chunk(uint8_t address,
                                                   uint16_t reg,
                                                   uint8_t* rx_data,
                                                   uint16_t rx_len,
                                                   uint8_t reg_width) {
    if (rx_len != 2) {
        return HAL_I2C_Mem_Read(&hi2c1, address << 1, reg, reg_width, rx_data, rx_len, 1000);
    }

    uint8_t buf[3];
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, address << 1, reg, reg_width, buf, sizeof(buf), 1000);
    if (status == HAL_OK) {
        rx_data[0] = buf[0];
        rx_data[1] = buf[1];
    }
    return status;
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
        if (mcu_utils_i2c1_read_chunk(address, reg + offset, rx_data + offset, chunk_len, reg_width) != HAL_OK) {
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
