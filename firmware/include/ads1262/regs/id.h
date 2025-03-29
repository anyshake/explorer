#ifndef __ADS1262_REGS_ID_H
#define __ADS1262_REGS_ID_H

#include <stdint.h>

#include "ads1262/reg_map.h"
#include "ads1262/utils.h"

#define ADS1262_REG_ID_DEV_ID_ADS1262 0x00
#define ADS1262_REG_ID_DEV_ID_ADS1263 0x01

typedef struct {
    // Device ID, read-only
    // 0x00: ADS1262
    // 0x01: ADS1263
    uint8_t dev_id;
    // Revision ID, read-only
    // Note: the chip revision ID can change without notification
    uint8_t rev_id;
} ads1262_reg_id_t;

void ads1262_reg_get_id(ads1262_reg_id_t* id);

#endif
