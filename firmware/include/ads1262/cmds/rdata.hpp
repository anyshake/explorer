#ifndef __ADS1262_CMDS_RDATA_HPP__
#define __ADS1262_CMDS_RDATA_HPP__

#include <stdint.h>

#include "ads1262/cmds/cmd_map.hpp"
#include "ads1262/utils.hpp"
#include "mcu_utils/gpio.hpp"
#include "settings.hpp"

typedef struct {
    // Status byte, contains the status of the ADC
    // Refer to the datasheet for more information (Figure 9-45, Table 9-18)
    uint8_t status;
    // Conversion data, contains the conversion result
    // Refer to the datasheet for more information (Figure 9-46, Table 9-19)
    int32_t data = 0;
    // Checksum byte, contains the checksum of the conversion result
    // Refer to the datasheet for more information (Figure 9-48, Table 9-20)
    uint8_t crc;
} adc_cmd_rdata_t;

void adc_cmd_rdata(adc_cmd_rdata_t* rdata, uint8_t control_type);

#endif
