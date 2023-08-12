#ifndef APP_ADS1256_CHANNEL_H
#define APP_ADS1256_CHANNEL_H

#include "ads1256/rw.h"
#include "ads1256/register.h"

#define ANALOG_INPUT_AIN8 0x00
#define ANALOG_INPUT_AIN1 0x01
#define ANALOG_INPUT_AIN2 0x02
#define ANALOG_INPUT_AIN3 0x03
#define ANALOG_INPUT_AIN4 0x04
#define ANALOG_INPUT_AIN5 0x05
#define ANALOG_INPUT_AIN6 0x06
#define ANALOG_INPUT_AIN7 0x07
#define ANALOG_INPUT_AINCOM 0x08

void adc_set_channel(uint8_t channel);
uint8_t adc_get_channel(void);

#endif
