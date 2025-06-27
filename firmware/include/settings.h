#ifndef __SETTINGS_H
#define __SETTINGS_H

#include <stdbool.h>

#include "ads1262/utils.h"
#include "utils/gpio.h"

// [Experimental Feature]
// Enable this if you are using 4.5 Hz geophone (e.g. LGT-4.5)
// this is useful for geophone low frequency response (<= 1.0 Hz)
// see filter.h, filter.c for detailed implementation
#define ENABLE_COMPENSATION true

// If EXPLORER_SAMPLERATE changed and ENABLE_COMPENSATION is true
// also remember to change the filter coefficients in filter.h
#define EXPLORER_SAMPLERATE 100
#define EXPLORER_BAUDRATE 115200

static const mcu_utils_gpio_t MCU_STATE_PIN = {
    .pin = 2,
};
static const ads1262_ctl_pin_t ADS1262_CTL_PIN = {
    .start = {.pin = 15},
    .drdy = {.pin = 4},
    .rst = {.pin = 5},
};

#endif
