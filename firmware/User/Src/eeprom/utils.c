#include "User/Inc/eeprom/utils.h"

void eeprom_init(mcu_utils_gpio_t pin_wp) {
    mcu_utils_gpio_mode(pin_wp, MCU_UTILS_GPIO_MODE_OUTPUT);
    mcu_utils_gpio_high(pin_wp);
}

void eeprom_protect(mcu_utils_gpio_t pin_wp, bool enable) {
    if (enable) {
        mcu_utils_gpio_high(pin_wp);
    } else {
        mcu_utils_gpio_low(pin_wp);
    }
}

void eeprom_earse(mcu_utils_gpio_t pin_wp, uint8_t len) {
    eeprom_protect(pin_wp, false);
    uint8_t buf[len];
    for (uint8_t i = 0; i < len; i++) {
        buf[i] = 0xFF;
    }
    mcu_utils_i2c_write(EEPROM_I2C_ADDRESS, 0x00, buf, len,
                        MCU_UTILS_I2C_REG_WIDTH_16);
    eeprom_protect(pin_wp, true);
}
