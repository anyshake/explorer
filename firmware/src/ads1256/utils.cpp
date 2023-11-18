#include "ads1256/adc.hpp"
#include "ads1256/command.hpp"
#include "ads1256/register.hpp"

// Begin SPI communication
void ADS1256::begin() {
    // Set pin mode
    pinMode(_cs_pin, OUTPUT);
    pinMode(_drdy_pin, INPUT);
    pinMode(_rst_pin, OUTPUT);
    // Hard reset ADS1256
    digitalWrite(_rst_pin, LOW);
    delay(250);
    digitalWrite(_rst_pin, HIGH);
    delay(250);
    // Initialize SPI
    SPI.begin();
    // Enable calibration
    writeRegister(ADC_REGISTER_STATUS, 0x04);
}

// Soft reset ADS1256 module
void ADS1256::reset() {
    SPI.beginTransaction(SPISettings(SPEED, MSBFIRST, SPI_MODE1));
    // Select chip
    digitalWrite(_cs_pin, LOW);
    // Wait a moment
    delayMicroseconds(10);
    // Send reset command
    SPI.transfer(DEVICE_COMMAND_RESET);
    // Wait for reset to complete
    delay(2);
    // Issue SDATAC
    SPI.transfer(DEVICE_COMMAND_SDATAC);
    delayMicroseconds(100);
    // Deselect chip
    digitalWrite(_cs_pin, HIGH);
    SPI.endTransaction();
}

// Get voltage from ADC
double ADS1256::getVoltage(int32_t value) {
    // if the 24th bit (sign) is not 0, the number is negative
    if (value >> 23) {
        // conversion for the negative sign "mirroring" around zero
        value -= 16777216;
    }

    // 2.5 = Vref; 8388607 = 2^{23} - 1
    double voltage = ((2 * VREF) / 8388607) * value;
    return voltage;
}
