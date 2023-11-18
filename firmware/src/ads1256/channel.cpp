#include "ads1256/channel.hpp"
#include "ads1256/adc.hpp"
#include "ads1256/register.hpp"

// Set single ended input channel
void ADS1256::setChannel(AnalogInputChannel channel) {
    writeRegister(ADC_REGISTER_MUX, (channel << 4) | ANALOG_INPUT_AINCOM);
}

// Get single ended input channel
AnalogInputChannel ADS1256::getChannel() {
    return (AnalogInputChannel)(readRegister(ADC_REGISTER_MUX) >> 4);
}
