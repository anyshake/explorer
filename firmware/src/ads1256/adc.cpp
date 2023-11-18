#include "ads1256/adc.hpp"

ADS1256::ADS1256(uint8_t cs_pin, uint8_t drdy_pin, uint8_t rst_pin) {
    _cs_pin = cs_pin;
    _drdy_pin = drdy_pin;
    _rst_pin = rst_pin;
}
