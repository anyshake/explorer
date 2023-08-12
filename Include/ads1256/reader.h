#ifndef APP_ADS1256_READER_H
#define APP_ADS1256_READER_H

#include "spi.h"
#include "ads1256/utils.h"
#include "ads1256/config.h"

int32_t adc_get_one_shot(void);
int32_t adc_get_continuous(void);
int32_t adc_get_differential(uint8_t positiveCh,
                             uint8_t negativeCh);

#endif
