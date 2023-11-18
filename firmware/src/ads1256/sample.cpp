#include "ads1256/sample.hpp"
#include "ads1256/adc.hpp"
#include "ads1256/register.hpp"

// Set ADC sample rate
void ADS1256::setSample(SampleRateSPS rate) {
    writeRegister(ADC_REGISTER_DRATE, rate);
}

// Get ADC sample rate
SampleRateSPS ADS1256::getSample() {
    return (SampleRateSPS)readRegister(ADC_REGISTER_DRATE);
}
