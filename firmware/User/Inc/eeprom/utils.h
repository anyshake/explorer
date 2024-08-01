#ifndef __EEPROM_UTILS_H
#define __EEPROM_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#include "Utils/Inc/delay.h"
#include "Utils/Inc/gpio.h"
#include "Utils/Inc/i2c.h"

#define EEPROM_I2C_ADDRESS 0x50

void eeprom_init(mcu_utils_gpio_t pin_wp, bool is_rtos);
void eeprom_protect(mcu_utils_gpio_t pin_wp, bool enable);
void eeprom_earse(mcu_utils_gpio_t pin_wp, uint8_t len);

#endif
