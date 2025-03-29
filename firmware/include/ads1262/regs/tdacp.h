#ifndef __ADS1262_REGS_TDACP_H
#define __ADS1262_REGS_TDACP_H

#include <stdint.h>

#include "ads1262/reg_map.h"
#include "ads1262/utils.h"

#define __ADS1262_REG_TDACP_DEFAULT_VALUE 0x00

#define ADS1262_REG_TDACP_OUTP_NOCONN 0x00
#define ADS1262_REG_TDACP_OUTP_TOAIN6 0x01

#define ADS1262_REG_TDACP_MAGP_2V5 0x00
#define ADS1262_REG_TDACP_MAGP_2V5078125 0x01
#define ADS1262_REG_TDACP_MAGP_2V515625 0x02
#define ADS1262_REG_TDACP_MAGP_2V53125 0x03
#define ADS1262_REG_TDACP_MAGP_2V5625 0x04
#define ADS1262_REG_TDACP_MAGP_2V625 0x05
#define ADS1262_REG_TDACP_MAGP_2V75 0x06
#define ADS1262_REG_TDACP_MAGP_3V0 0x07
#define ADS1262_REG_TDACP_MAGP_3V5 0x08
#define ADS1262_REG_TDACP_MAGP_4V5 0x09
#define ADS1262_REG_TDACP_MAGP_2V4921875 0x11
#define ADS1262_REG_TDACP_MAGP_2V484375 0x12
#define ADS1262_REG_TDACP_MAGP_2V46875 0x13
#define ADS1262_REG_TDACP_MAGP_2V4375 0x14
#define ADS1262_REG_TDACP_MAGP_2V375 0x15
#define ADS1262_REG_TDACP_MAGP_2V25 0x16
#define ADS1262_REG_TDACP_MAGP_2V0 0x17
#define ADS1262_REG_TDACP_MAGP_1V5 0x18
#define ADS1262_REG_TDACP_MAGP_0V5 0x19

typedef struct {
    // TDACP output connection, whether connect TDACP output to pin AIN6
    // 0x00: No connection (default)
    // 0x01: Output to AIN6
    uint8_t out_p;
    // TDACP current magnitude, selects the current values of TDACP
    // 0x00: 2.5 V (default)
    // 0x01: 2.5078125 V
    // 0x02: 2.515625 V
    // 0x03: 2.53125 V
    // 0x04: 2.5625 V
    // 0x05: 2.625 V
    // 0x06: 2.75 V
    // 0x07: 3.0 V
    // 0x08: 3.5 V
    // 0x09: 4.5 V
    // 0x11: 2.4921875 V
    // 0x12: 2.484375 V
    // 0x13: 2.46875 V
    // 0x14: 2.4375 V
    // 0x15: 2.375 V
    // 0x16: 2.25 V
    // 0x17: 2.0 V
    // 0x18: 1.5 V
    // 0x19: 0.5 V
    uint8_t mag_p;
} ads1262_reg_tdacp_t;

ads1262_reg_tdacp_t ads1262_reg_new_tdacp(void);
void ads1262_reg_set_tdacp(ads1262_reg_tdacp_t* tdacp);
void ads1262_reg_get_tdacp(ads1262_reg_tdacp_t* tdacp);

#endif
