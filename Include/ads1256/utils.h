#ifndef APP_ADS1256_UTILS_H
#define APP_ADS1256_UTILS_H

#include "gd32f30x.h"
#include "spi.h"

#include "ads1256/rw.h"
#include "ads1256/config.h"
#include "ads1256/command.h"
#include "ads1256/register.h"

void adc_begin(void);
void adc_reset(void);
double adc_get_voltage(int32_t value);

#endif
