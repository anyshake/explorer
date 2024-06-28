#ifndef __MCU_UTILS_I2C__
#define __MCU_UTILS_I2C__

#include <Wire.h>
#include <stdint.h>
#include "settings.hpp"

void mcu_utils_i2c_init();
void mcu_utils_i2c_read(uint8_t address, uint8_t reg, uint8_t* data, uint8_t len);
void mcu_utils_i2c_write(uint8_t address, uint8_t reg, uint8_t* data, uint8_t len);

#endif
