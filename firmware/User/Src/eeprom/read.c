#include "User/Inc/eeprom/read.h"

void eeprom_read(uint8_t* buf, uint8_t len) {
    mcu_utils_i2c_read(EEPROM_I2C_ADDRESS, 0x00, buf, len,
                       MCU_UTILS_I2C_REG_WIDTH_16);
}
