#ifndef __ADS1262_REGS_ID_HPP
#define __ADS1262_REGS_ID_HPP

#include <stdint.h>

#include "ads1262/regs/reg_map.hpp"
#include "ads1262/utils.hpp"

#define ADC_ID_DEV_ID_ADS1262 0x00
#define ADC_ID_DEV_ID_ADS1263 0x01

typedef struct {
    // Device ID, read-only
    // 0x00: ADS1262
    // 0x01: ADS1263
    uint8_t dev_id;
    // Revision ID, read-only
    // Note: the chip revision ID can change without notification
    uint8_t rev_id;
} adc_reg_id_t;

void adc_reg_get_id(adc_reg_id_t* id);

#endif
