#ifndef __ADS1262_REGS_POWER_H
#define __ADS1262_REGS_POWER_H

#include <stdint.h>

#include "ads1262/reg_map.h"
#include "ads1262/utils.h"

#define __ADS1262_REG_POWER_DEFAULT_VALUE 0x11

#define ADS1262_REG_POWER_RESET_NONE 0x00
#define ADS1262_REG_POWER_RESET_OCCURRED 0x01

#define ADS1262_REG_POWER_VBIAS_DISABLED 0x00
#define ADS1262_REG_POWER_VBIAS_ENABLED 0x01

#define ADS1262_REG_POWER_INTREF_DISABLED 0x00
#define ADS1262_REG_POWER_INTREF_ENABLED 0x01

typedef struct {
    // Reset indicator, indicates ADC reset has occurred. Clear this bit to
    // detect the next device reset.
    // 0x00: No reset
    // 0x01: Reset has occurred (default)
    uint8_t reset;
    // Level shift voltage enable, enables the internal level shift voltage to
    // the AINCOM pin.
    // 0x00: Disabled (default),
    // 0x01: Enabled
    uint8_t vbias;
    // Internal reference enable, enables the internal reference voltage. Note
    // the IDAC and temperature sensor require the internal voltage reference.
    // 0x00: Disabled
    // 0x01: Enabled (default)
    uint8_t intref;
} ads1262_reg_power_t;

ads1262_reg_power_t ads1262_reg_new_power(void);
void ads1262_reg_set_power(ads1262_reg_power_t* power);
void ads1262_reg_get_power(ads1262_reg_power_t* power);

#endif
