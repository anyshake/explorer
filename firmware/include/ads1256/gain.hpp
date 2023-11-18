#pragma once

#include <stdint.h>

typedef enum {
    GAIN_RATE_1X = 0x00,
    GAIN_RATE_2X = 0x01,
    GAIN_RATE_4X = 0x02,
    GAIN_RATE_8X = 0x03,
    GAIN_RATE_16X = 0x04,
    GAIN_RATE_32X = 0x05,
    GAIN_RATE_64X = 0x06,
} GainRate;
