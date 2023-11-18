#pragma once

#include <stdint.h>

typedef enum {
    ANALOG_INPUT_AIN8 = 0x00,
    ANALOG_INPUT_AIN1 = 0x01,
    ANALOG_INPUT_AIN2 = 0x02,
    ANALOG_INPUT_AIN3 = 0x03,
    ANALOG_INPUT_AIN4 = 0x04,
    ANALOG_INPUT_AIN5 = 0x05,
    ANALOG_INPUT_AIN6 = 0x06,
    ANALOG_INPUT_AIN7 = 0x07,
    ANALOG_INPUT_AINCOM = 0x08,
} AnalogInputChannel;
