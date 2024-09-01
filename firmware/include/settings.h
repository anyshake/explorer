#ifndef __SETTINGS_H
#define __SETTINGS_H

#include "ads1262/utils.h"
#include "utils/gpio.h"

#define EXPLORER_LEGACY_MODE 0
#define EXPLORER_24BIT_MODE 1
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
