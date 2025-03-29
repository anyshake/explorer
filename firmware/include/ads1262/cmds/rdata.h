#ifndef __ADS1262_CMDS_RDATA_H
#define __ADS1262_CMDS_RDATA_H

#include <stdbool.h>
#include <stdint.h>

#include "ads1262/cmd_map.h"
#include "ads1262/regs/interface.h"
#include "ads1262/utils.h"
#include "utils/gpio.h"

typedef struct {
    // Status byte, contains the status of the ADC
    // Refer to the datasheet for more information (Figure 9-45, Table 9-18)
    uint8_t status;
    // Conversion data, contains the conversion result
    // Refer to the datasheet for more information (Figure 9-46, Table 9-19)
    int32_t data;
    // Checksum byte, contains the checksum of the conversion result
    // Refer to the datasheet for more information (Figure 9-48, Table 9-20)
    uint8_t crc;
} ads1262_cmd_rdata_t;

void ads1262_cmd_rdata(ads1262_ctl_pin_t pin,
                       ads1262_cmd_rdata_t* rdata,
                       uint8_t control_type);
bool ads1262_cmd_rdata_is_valid(ads1262_cmd_rdata_t* rdata, uint8_t crc_mode);

#endif
