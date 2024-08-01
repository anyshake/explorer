#include "User/Inc/eeprom/write.h"

void eeprom_write(mcu_utils_gpio_t pin_wp, uint8_t* buf, uint8_t len) {
    eeprom_protect(pin_wp, false);
    mcu_utils_i2c_write(EEPROM_I2C_ADDRESS, 0x00, buf, len,
                        MCU_UTILS_I2C_REG_WIDTH_16);
    eeprom_protect(pin_wp, true);
}
