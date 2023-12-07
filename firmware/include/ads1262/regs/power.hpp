#ifndef __ADS1262_REGS_POWER_HPP
#define __ADS1262_REGS_POWER_HPP

#include <stdint.h>

#include "ads1262/regs/reg_map.hpp"
#include "ads1262/utils.hpp"

#define __ADC_POWER_DEFAULT_VALUE 0x11

#define ADC_POWER_RESET_NONE 0x00
#define ADC_POWER_RESET_OCCURRED 0x01

#define ADC_POWER_VBIAS_DISABLED 0x00
#define ADC_POWER_VBIAS_ENABLED 0x01

#define ADC_POWER_INTREF_DISABLED 0x00
#define ADC_POWER_INTREF_ENABLED 0x01

typedef struct {
    // Reset indicator, indicates ADC reset has occurred. Clear this bit to
    // detect the next device reset.
    // 0x00: No reset
    // 0x01: Reset has occurred (default)
    uint8_t reset = 0x01;
    // Level shift voltage enable, enables the internal level shift voltage to
    // the AINCOM pin.
    // 0x00: Disabled (default),
    // 0x01: Enabled
    uint8_t vbias = 0x00;
    // Internal reference enable, enables the internal reference voltage. Note
    // the IDAC and temperature sensor require the internal voltage reference.
    // 0x00: Disabled
    // 0x01: Enabled (default)
    uint8_t intref = 0x01;
} adc_reg_power_t;

void adc_reg_set_power(adc_reg_power_t* power);
void adc_reg_get_power(adc_reg_power_t* power);

#endif
