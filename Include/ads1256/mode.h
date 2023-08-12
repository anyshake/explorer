#ifndef APP_ADS1256_MODE_H
#define APP_ADS1256_MODE_H

#include "spi.h"
#include "ads1256/rw.h"

#define ADC_MODE_ONESHOT 0x00
#define ADC_MODE_CONTINUOUS 0x01

void adc_set_mode(uint8_t mode);

#endif
