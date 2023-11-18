#include "ads1256/adc.hpp"
#include "ads1256/register.hpp"

// Set analog input buffer and calibration
void ADS1256::setStatus(uint8_t buffer, uint8_t calibration) {
    writeRegister(ADC_REGISTER_STATUS, (buffer << 1) | calibration << 2);
}

// Get analog input buffer and calibration
void ADS1256::getStatus(uint8_t *buffer, uint8_t *calibration) {
    uint8_t status = readRegister(ADC_REGISTER_STATUS);
    *buffer = (status >> 1) & 0x01;
    *calibration = (status >> 2) & 0x01;
}
