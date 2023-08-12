#include "ads1256/status.h"

void adc_set_status(uint8_t buffer, uint8_t calibration) {
    adc_write_register(ADC_REGISTER_STATUS, (buffer << 1) | buffer << 2);
}
