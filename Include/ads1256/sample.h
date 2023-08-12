#ifndef APP_ADS1256_SAMPLE_H
#define APP_ADS1256_SAMPLE_H

#include "ads1256/rw.h"

#define SAMPLE_RATE_30000_SPS 0xF0
#define SAMPLE_RATE_15000_SPS 0xE0
#define SAMPLE_RATE_7500_SPS 0xD0
#define SAMPLE_RATE_3750_SPS 0xC0
#define SAMPLE_RATE_2000_SPS 0xB0
#define SAMPLE_RATE_1000_SPS 0xA1
#define SAMPLE_RATE_500_SPS 0x92
#define SAMPLE_RATE_100_SPS 0x82
#define SAMPLE_RATE_60_SPS 0x72
#define SAMPLE_RATE_50_SPS 0x63
#define SAMPLE_RATE_30_SPS 0x53
#define SAMPLE_RATE_25_SPS 0x43
#define SAMPLE_RATE_15_SPS 0x33
#define SAMPLE_RATE_10_SPS 0x23
#define SAMPLE_RATE_5_SPS 0x13
#define SAMPLE_RATE_2_5_SPS 0x03

void adc_set_sample(uint8_t rate);
uint8_t adc_get_sample(void);

#endif
