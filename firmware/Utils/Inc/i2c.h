#ifndef __ANYSHAKE_MCU_UTILS_I2C_H
#define __ANYSHAKE_MCU_UTILS_I2C_H

#include <stdbool.h>
#include <stdint.h>

#include "Core/Inc/i2c.h"
#include "Utils/Inc/delay.h"
#include "stm32f1xx_hal.h"

#define MCU_UTILS_I2C_REG_WIDTH_8 I2C_MEMADD_SIZE_8BIT
#define MCU_UTILS_I2C_REG_WIDTH_16 I2C_MEMADD_SIZE_16BIT

void mcu_utils_i2c_init(bool is_rtos);
void mcu_utils_i2c_end(void);
void mcu_utils_i2c_read(uint8_t address,
                        uint16_t reg,
                        uint8_t* rx_data,
                        uint16_t rx_len,
                        uint8_t reg_width);
void mcu_utils_i2c_write(uint8_t address,
                         uint16_t reg,
                         uint8_t* tx_data,
                         uint16_t tx_len,
                         uint8_t reg_width);

#endif
