#ifndef __ADS1262_REGS_IDACMUX_H
#define __ADS1262_REGS_IDACMUX_H

#include <stdint.h>

#include "ads1262/reg_map.h"
#include "ads1262/utils.h"

#define __ADS1262_REG_IDACMUX_DEFAULT_VALUE 0xBB

#define ADS1262_REG_IDACMUX_AIN0 0x00
#define ADS1262_REG_IDACMUX_AIN1 0x01
#define ADS1262_REG_IDACMUX_AIN2 0x02
#define ADS1262_REG_IDACMUX_AIN3 0x03
#define ADS1262_REG_IDACMUX_AIN4 0x04
#define ADS1262_REG_IDACMUX_AIN5 0x05
#define ADS1262_REG_IDACMUX_AIN6 0x06
#define ADS1262_REG_IDACMUX_AIN7 0x07
#define ADS1262_REG_IDACMUX_AIN8 0x08
#define ADS1262_REG_IDACMUX_AIN9 0x09
#define ADS1262_REG_IDACMUX_AINCOM 0x0A
#define ADS1262_REG_IDACMUX_NOCONN 0x0B

typedef struct {
    // IDAC2 Output Multiplexer, Selects the analog input pin to connect IDAC2
    // 0x00: AIN0
    // 0x01: AIN1
    // 0x02: AIN2
    // 0x03: AIN3
    // 0x04: AIN4
    // 0x05: AIN5
    // 0x06: AIN6
    // 0x07: AIN7
    // 0x08: AIN8
    // 0x09: AIN9
    // 0x0A: AINCOM
    // 0x0B: No connection (default)
    uint8_t mux_2;
    // IDAC1 Output Multiplexer, Selects the analog input pin to connect IDAC1
    // 0x00: AIN0
    // 0x01: AIN1
    // 0x02: AIN2
    // 0x03: AIN3
    // 0x04: AIN4
    // 0x05: AIN5
    // 0x06: AIN6
    // 0x07: AIN7
    // 0x08: AIN8
    // 0x09: AIN9
    // 0x0A: AINCOM
    // 0x0B: No connection (default)
    uint8_t mux_1;
} ads1262_reg_idacmux_t;

ads1262_reg_idacmux_t ads1262_reg_new_idacmux(void);
void ads1262_reg_set_idacmux(ads1262_reg_idacmux_t* idacmux);
void ads1262_reg_get_idacmux(ads1262_reg_idacmux_t* idacmux);

#endif
