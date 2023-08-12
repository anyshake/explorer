#include "ads1256/channel.h"

void adc_set_channel(uint8_t channel) {
    adc_write_register(ADC_REGISTER_MUX, (channel << 4) | ANALOG_INPUT_AINCOM);
}

uint8_t adc_get_channel() {
    return adc_read_register(ADC_REGISTER_MUX) >> 4;
}
