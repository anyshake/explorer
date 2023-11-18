#include "ads1256/adc.hpp"
#include "ads1256/channel.hpp"
#include "ads1256/command.hpp"
#include "ads1256/register.hpp"

// One Shot single channel mode, use with setChannel()
int32_t ADS1256::getSingleOneShot() {
    wait();
    SPI.beginTransaction(SPISettings(SPEED, MSBFIRST, SPI_MODE1));

    // CS must stay LOW during the entire
    digitalWrite(_cs_pin, LOW);
    // Issue RDATA command
    SPI.transfer(DEVICE_COMMAND_RDATA);
    delayMicroseconds(10);

    // Get the 24-bit result
    int32_t result = (int32_t)SPI.transfer(DEVICE_COMMAND_SDATAC) << 16 |
                     (int32_t)SPI.transfer(DEVICE_COMMAND_SDATAC) << 8 |
                     (int32_t)SPI.transfer(DEVICE_COMMAND_SDATAC);

    // Deselect the chip
    digitalWrite(_cs_pin, HIGH);
    SPI.endTransaction();

    return result;
}

// Continuous resding mode (i.e. AIN0+COM)
int32_t ADS1256::getSingleContinuous() {
    wait();
    SPI.beginTransaction(SPISettings(SPEED, MSBFIRST, SPI_MODE1));

    // CS must stay LOW during the entire
    digitalWrite(_cs_pin, LOW);

    // Get the 24-bit result
    int32_t result = (int32_t)SPI.transfer(DEVICE_COMMAND_SDATAC) << 16 |
                     (int32_t)SPI.transfer(DEVICE_COMMAND_SDATAC) << 8 |
                     (int32_t)SPI.transfer(DEVICE_COMMAND_SDATAC);

    // Deselect the chip
    digitalWrite(_cs_pin, HIGH);
    SPI.endTransaction();

    return result;
}

// Differential mode, also apply to single channel
int32_t ADS1256::getDifferential(AnalogInputChannel positiveCh,
                                 AnalogInputChannel negativeCh) {
    wait();
    SPI.beginTransaction(SPISettings(SPEED, MSBFIRST, SPI_MODE1));

    // CS must stay LOW during the entire
    digitalWrite(_cs_pin, LOW);
    SPI.transfer(DEVICE_COMMAND_WREG | ADC_REGISTER_MUX);
    SPI.transfer(0x00);

    // Write the positive and negative channels to MUX
    SPI.transfer((positiveCh << 4) | negativeCh);

    // Request a conversion
    SPI.transfer(DEVICE_COMMAND_SYNC);
    delayMicroseconds(4);
    SPI.transfer(DEVICE_COMMAND_WAKEUP);
    SPI.transfer(DEVICE_COMMAND_RDATA);
    delayMicroseconds(5);

    // Get the 24-bit result
    int32_t result = (int32_t)SPI.transfer(DEVICE_COMMAND_SDATAC) << 16 |
                     (int32_t)SPI.transfer(DEVICE_COMMAND_SDATAC) << 8 |
                     (int32_t)SPI.transfer(DEVICE_COMMAND_SDATAC);

    // Deselect the chip
    digitalWrite(_cs_pin, HIGH);
    SPI.endTransaction();

    return result;
}
