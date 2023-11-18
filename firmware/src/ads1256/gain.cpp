#include "ads1256/gain.hpp"
#include "ads1256/adc.hpp"
#include "ads1256/register.hpp"

// Set ADC gain
void ADS1256::setGain(GainRate gain) {
    writeRegister(ADC_REGISTER_ADCON, gain & 0x07);
}

// Get ADC gain
GainRate ADS1256::getGain() {
    return (GainRate)(readRegister(ADC_REGISTER_ADCON) & 0x07);
}
