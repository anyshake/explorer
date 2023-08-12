#include "ads1256/gain.h"

void adc_set_gain(uint8_t gain) {
    adc_write_register(ADC_REGISTER_ADCON, gain & 0x07);
}

uint8_t adc_get_gain() {
    return adc_read_register(ADC_REGISTER_ADCON) & 0x07;
}
