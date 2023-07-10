#include "ads1256/adc.hpp"

// Soft reset ADS1256 module
void ADS1256::reset() {
    SPI.beginTransaction(SPISettings(SPEED, MSBFIRST, SPI_MODE1));
    // Select chip
    digitalWrite(_cs_pin, LOW);
    // Wait a moment
    delayMicroseconds(10);
    // Send reset command
    SPI.transfer(0xFE);
    // Wait for reset to complete
    delay(2);
    // Issue SDATAC
    SPI.transfer(0x0F);
    delayMicroseconds(100);
    // Deselect chip
    digitalWrite(_cs_pin, HIGH);
    SPI.endTransaction();
}