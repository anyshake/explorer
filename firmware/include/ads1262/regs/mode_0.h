#ifndef __ADS1262_REGS_MODE_0_H
#define __ADS1262_REGS_MODE_0_H

#include <stdint.h>

#include "ads1262/reg_map.h"
#include "ads1262/utils.h"

#define __ADS1262_REG_MODE_0_DEFAULT_VALUE 0x00

#define ADS1262_REG_MODE_0_REF_REV_NORMAL 0x00
#define ADS1262_REG_MODE_0_REF_REV_REVERSED 0x01

#define ADS1262_REG_MODE_0_RUN_MODE_CONTINUOUS 0x00
#define ADS1262_REG_MODE_0_RUN_MODE_ONESHOT 0x01

#define ADS1262_REG_MODE_0_CHOP_DISABLED 0x00
#define ADS1262_REG_MODE_0_CHOP_ENABLED 0x01
#define ADS1262_REG_MODE_0_CHOP_IDAC 0x02
#define ADS1262_REG_MODE_0_CHOP_BOTH 0x03

#define ADS1262_REG_MODE_0_DELAY_0 0x00
#define ADS1262_REG_MODE_0_DELAY_8_7 0x01
#define ADS1262_REG_MODE_0_DELAY_17 0x02
#define ADS1262_REG_MODE_0_DELAY_35 0x03
#define ADS1262_REG_MODE_0_DELAY_69 0x04
#define ADS1262_REG_MODE_0_DELAY_139 0x05
#define ADS1262_REG_MODE_0_DELAY_278 0x06
#define ADS1262_REG_MODE_0_DELAY_555 0x07
#define ADS1262_REG_MODE_0_DELAY_1100 0x08
#define ADS1262_REG_MODE_0_DELAY_2200 0x09
#define ADS1262_REG_MODE_0_DELAY_4400 0x0A
#define ADS1262_REG_MODE_0_DELAY_8800 0x0B

typedef struct {
    // Reference mux polarity reversal, reverses the ADC1 reference multiplexer
    // output polarity
    // 0x00: Normal polarity of reference multiplexer output (default)
    // 0x01: Reverse polarity of reference multiplexer output
    uint8_t ref_rev;
    // ADC conversion run mode, selects the ADC conversion (run) mode
    // 0x00: Continuous conversion mode (default)
    // 0x01: One-shot conversion mode
    uint8_t run_mode;
    // Chop mode enable, enables the ADC chop and IDAC rotation options
    // 0x00: Input chop and IDAC rotation disabled (default)
    // 0x01: Input chop enabled
    // 0x02: IDAC rotation enabled
    // 0x03: Input chop and IDAC rotation enabled
    uint8_t chop;
    // Conversion delay, provides additional delay from conversion start to the
    // beginning of the actual conversion
    // 0x00: No delay (default)
    // 0x01: 8.7 us
    // 0x02: 17 us
    // 0x03: 35 us
    // 0x04: 69 us
    // 0x05: 139 us
    // 0x06: 278 us
    // 0x07: 555 us
    // 0x08: 1.1 ms
    // 0x09: 2.2 ms
    // 0x0A: 4.4 ms
    // 0x0B: 8.8 ms
    uint8_t delay;
} ads1262_reg_mode_0_t;

ads1262_reg_mode_0_t ads1262_reg_new_mode_0(void);
void ads1262_reg_set_mode_0(ads1262_reg_mode_0_t* mode_0);
void ads1262_reg_get_mode_0(ads1262_reg_mode_0_t* mode_0);

#endif
