#ifndef APP_ADS1256_RW_H
#define APP_ADS1256_RW_H

#include "spi.h"
#include "delay.h"
#include "gd32f30x.h"
#include "ads1256/config.h"
#include "ads1256/command.h"
#include "ads1256/register.h"

void adc_wait(void);
uint8_t adc_read_register(uint8_t reg);
void adc_write_register(uint8_t reg, uint8_t value);

#endif
