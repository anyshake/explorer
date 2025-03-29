#ifndef __ADS1262_REGS_INPMUX_H
#define __ADS1262_REGS_INPMUX_H

#include <stdint.h>

#include "ads1262/reg_map.h"
#include "ads1262/utils.h"

#define __ADS1262_REG_INPMUX_DEFAULT_VALUE 0x01

#define ADS1262_REG_INPMUX_AIN0 0x00
#define ADS1262_REG_INPMUX_AIN1 0x01
#define ADS1262_REG_INPMUX_AIN2 0x02
#define ADS1262_REG_INPMUX_AIN3 0x03
#define ADS1262_REG_INPMUX_AIN4 0x04
#define ADS1262_REG_INPMUX_AIN5 0x05
#define ADS1262_REG_INPMUX_AIN6 0x06
#define ADS1262_REG_INPMUX_AIN7 0x07
#define ADS1262_REG_INPMUX_AIN8 0x08
#define ADS1262_REG_INPMUX_AIN9 0x09
#define ADS1262_REG_INPMUX_AINCOM 0x0A
#define ADS1262_REG_INPMUX_TEMP 0x0B
#define ADS1262_REG_INPMUX_AVDD 0x0C
#define ADS1262_REG_INPMUX_DVDD 0x0D
#define ADS1262_REG_INPMUX_TDAC 0x0E
#define ADS1262_REG_INPMUX_FLOAT 0x0F

typedef struct {
    // Positive input multiplexer, selects the positive input channel
    // 0x00: AIN0 (default)
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
    // 0x0B: Temperature sensor monitor positive
    // 0x0C: Analog power supply monitor positive
    // 0x0D: Digital power supply monitor positive
    // 0x0E: TDAC test signal positive
    // 0x0F: Float (open connection)
    uint8_t mux_p;
    // Negative input multiplexer, selects the negative input channel
    // 0x00: AIN0
    // 0x01: AIN1 (default)
    // 0x02: AIN2
    // 0x03: AIN3
    // 0x04: AIN4
    // 0x05: AIN5
    // 0x06: AIN6
    // 0x07: AIN7
    // 0x08: AIN8
    // 0x09: AIN9
    // 0x0A: AINCOM
    // 0x0B: Temperature sensor monitor negative
    // 0x0C: Analog power supply monitor negative
    // 0x0D: Digital power supply monitor negative
    // 0x0E: TDAC test signal negative
    // 0x0F: Float (open connection, default)
    uint8_t mux_n;
} ads1262_reg_inpmux_t;

ads1262_reg_inpmux_t ads1262_reg_new_inpmux(void);
void ads1262_reg_set_inpmux(ads1262_reg_inpmux_t* inpmux);
void ads1262_reg_get_inpmux(ads1262_reg_inpmux_t* inpmux);

#endif
