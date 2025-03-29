#ifndef __ADS1262_REGS_IDACMAG_H
#define __ADS1262_REGS_IDACMAG_H

#include <stdint.h>

#include "ads1262/reg_map.h"
#include "ads1262/utils.h"

#define __ADS1262_REG_IDACMAG_DEFAULT_VALUE 0x00

#define ADS1262_REG_IDACMAG_DISABLED 0x00
#define ADS1262_REG_IDACMAG_50UA 0x01
#define ADS1262_REG_IDACMAG_100UA 0x02
#define ADS1262_REG_IDACMAG_250UA 0x03
#define ADS1262_REG_IDACMAG_500UA 0x04
#define ADS1262_REG_IDACMAG_750UA 0x05
#define ADS1262_REG_IDACMAG_1000UA 0x06
#define ADS1262_REG_IDACMAG_1500UA 0x07
#define ADS1262_REG_IDACMAG_2000UA 0x08
#define ADS1262_REG_IDACMAG_2500UA 0x09
#define ADS1262_REG_IDACMAG_3000UA 0x0A

typedef struct {
    // IDAC2 current magnitude, selects the current values of IDAC2
    // 0x00: Disabled (default)
    // 0x01: 50 uA
    // 0x02: 100 uA
    // 0x03: 250 uA
    // 0x04: 500 uA
    // 0x05: 750 uA
    // 0x06: 1000 uA
    // 0x07: 1500 uA
    // 0x08: 2000 uA
    // 0x09: 2500 uA
    // 0x0A: 3000 uA
    uint8_t mag_2;
    // IDAC1 current magnitude, selects the current values of IDAC1
    // 0x00: Disabled (default)
    // 0x01: 50 uA
    // 0x02: 100 uA
    // 0x03: 250 uA
    // 0x04: 500 uA
    // 0x05: 750 uA
    // 0x06: 1000 uA
    // 0x07: 1500 uA
    // 0x08: 2000 uA
    // 0x09: 2500 uA
    // 0x0A: 3000 uA
    uint8_t mag_1;
} ads1262_reg_idacmag_t;

ads1262_reg_idacmag_t ads1262_reg_new_idacmag(void);
void ads1262_reg_set_idacmag(ads1262_reg_idacmag_t* idacmag);
void ads1262_reg_get_idacmag(ads1262_reg_idacmag_t* idacmag);

#endif
