#ifndef __ADS1262_REGS_MODE_1_H
#define __ADS1262_REGS_MODE_1_H

#include <stdint.h>

#include "ads1262/reg_map.h"
#include "ads1262/utils.h"

#define __ADS1262_REG_MODE_1_DEFAULT_VALUE 0x80

#define ADS1262_REG_MODE_1_FILTER_SINC1 0x00
#define ADS1262_REG_MODE_1_FILTER_SINC2 0x01
#define ADS1262_REG_MODE_1_FILTER_SINC3 0x02
#define ADS1262_REG_MODE_1_FILTER_SINC4 0x03
#define ADS1262_REG_MODE_1_FILTER_FIR 0x04

#define ADS1262_REG_MODE_1_SB_ADC1 0x01
#define ADS1262_REG_MODE_1_SB_ADC2 0x02

#define ADS1262_REG_MODE_1_SB_POL_UP 0x01
#define ADS1262_REG_MODE_1_SB_POL_DOWN 0x02

#define ADS1262_REG_MODE_1_SB_MAG_NONE 0x00
#define ADS1262_REG_MODE_1_SB_MAG_0P5 0x01
#define ADS1262_REG_MODE_1_SB_MAG_2 0x02
#define ADS1262_REG_MODE_1_SB_MAG_10 0x03
#define ADS1262_REG_MODE_1_SB_MAG_50 0x04
#define ADS1262_REG_MODE_1_SB_MAG_200 0x05
#define ADS1262_REG_MODE_1_SB_MAG_10M 0x06

typedef struct {
    // Digital filter, configures the ADC digital filter
    // 0x00: Sinc1 filter
    // 0x01: Sinc2 filter
    // 0x02: Sinc3 filter
    // 0x03: Sinc4 filter
    // 0x04: FIR filter (default)
    uint8_t filter;
    // Sensor bias ADC connection, selects the ADC to connect the sensor bias
    // 0x01: Sensor bias connected to ADC1 mux out (default)
    // 0x02: Sensor bias connected to ADC2 mux out
    uint8_t sb_adc;
    // Sensor bias polarity, selects the sensor bias for pull-up or pull-down
    // 0x01: Sensor bias is pull-up (default)
    // 0x02: Sensor bias is pull-down
    uint8_t sb_pol;
    // Sensor bias magnitude, selects the sensor bias current magnitude or the
    // bias resistor
    // 0x00: No sensor bias current or resistor (default)
    // 0x01: 0.5-uA sensor bias current
    // 0x02: 2-uA sensor bias current
    // 0x03: 10-uA sensor bias current
    // 0x04: 50-uA sensor bias current
    // 0x05: 200-uA sensor bias current
    // 0x06: 10-MOhm resistor
    uint8_t sb_mag;
} ads1262_reg_mode_1_t;

ads1262_reg_mode_1_t ads1262_reg_new_mode_1(void);
void ads1262_reg_set_mode_1(ads1262_reg_mode_1_t* mode_1);
void ads1262_reg_get_mode_1(ads1262_reg_mode_1_t* mode_1);

#endif
