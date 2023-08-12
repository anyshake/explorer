#include "ads1256/sample.h"

void adc_set_sample(uint8_t rate) {
    adc_write_register(ADC_REGISTER_DRATE, rate);
}

uint8_t adc_get_sample() {
    return adc_read_register(ADC_REGISTER_DRATE);
}
