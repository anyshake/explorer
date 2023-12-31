#ifndef __ADS1262_REGS_INTERFACE_HPP
#define __ADS1262_REGS_INTERFACE_HPP

#include <stdint.h>

#include "ads1262/regs/reg_map.hpp"
#include "ads1262/utils.hpp"

#define __ADC_INTERFACE_DEFAULT_VALUE 0x05

#define ADC_INTERFACE_TIMEOUT_DISABLED 0x00
#define ADC_INTERFACE_TIMEOUT_ENABLED 0x01

#define ADC_INTERFACE_STATUS_DISABLED 0x00
#define ADC_INTERFACE_STATUS_ENABLED 0x01

#define ADC_INTERFACE_CRC_DISABLED 0x00
#define ADC_INTERFACE_CRC_CHECKSUM 0x01
#define ADC_INTERFACE_CRC_CRC 0x02

typedef struct {
    // Serial interface time-out enable, enables the serial interface automatic
    // time-out mode
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t timeout = 0x00;
    // Status byte enable, Enables the inclusion of the status byte during
    // conversion data read-back
    // 0x00: Disabled
    // 0x01: Enabled (default)
    uint8_t status = 0x01;
    // Checksum byte enable, Enables the inclusion of the checksum byte during
    // conversion data read-back
    // 0x00: Disabled
    // 0x01: Enable Checksum byte in Checksum mode during conversion data
    // read-back (default)
    // 0x02: Enable Checksum byte in CRC mode during conversion data read-back
    uint8_t crc = 0x01;
} adc_reg_interface_t;

void adc_reg_set_interface(adc_reg_interface_t* interface);
void adc_reg_get_interface(adc_reg_interface_t* interface);

#endif
