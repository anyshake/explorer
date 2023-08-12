#ifndef APP_ADS1256_GAIN_H
#define APP_ADS1256_GAIN_H

#include "ads1256/rw.h"
#include "ads1256/register.h"

#define GAIN_RATE_1X 0x00
#define GAIN_RATE_2X 0x01
#define GAIN_RATE_4X 0x02
#define GAIN_RATE_8X 0x03
#define GAIN_RATE_16X 0x04
#define GAIN_RATE_32X 0x05
#define GAIN_RATE_64X 0x06

void adc_set_gain(uint8_t gain);
uint8_t adc_get_gain(void);

#endif
