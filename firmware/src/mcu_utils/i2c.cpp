#include "mcu_utils/i2c.hpp"

void mcu_utils_i2c_init() {
    Wire.begin();
}

void mcu_utils_i2c_read(uint8_t address,
                        uint8_t reg,
                        uint8_t* data,
                        uint8_t len) {
    if (!len) {
        return;
    }
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.requestFrom(address, len);
    for (uint8_t i = 0; i < len; i++) {
        if (data != NULL) {
            data[i] = Wire.read();
        } else {
            Wire.read();
        }
    }

    Wire.endTransmission();
}

void mcu_utils_i2c_write(uint8_t address,
                         uint8_t reg,
                         uint8_t* data,
                         uint8_t len) {
    if (data == NULL || !len) {
        return;
    }
    Wire.beginTransmission(address);
    Wire.write(reg);
    for (uint8_t i = 0; i < len; i++) {
        Wire.write(data[i]);
    }

    Wire.endTransmission();
}
