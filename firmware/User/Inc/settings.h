#ifndef __SETTINGS_H
#define __SETTINGS_H

#include <stdint.h>

#include "User/Inc/ads1262/utils.h"
#include "User/Inc/gnss/utils.h"
#include "User/Inc/icm42688/utils.h"
#include "User/Inc/lsm6ds3/utils.h"
#include "User/Inc/lsm6ds3tr_c/utils.h"
#include "User/Inc/lsm6dsr/utils.h"
#include "Utils/Inc/gpio.h"

// Empirical PPS waveform alignment; positive values move packet times later.
#define DATA_TIMESTAMP_OFFSET_MS 10
#define PACKET_SENDING_INTERVAL 100
#define ADC_ACQUISITION_RATE 250
#define SENSOR_ACQUISITION_QUEUE_DEPTH 8
#define GNSS_REQUIRED_VALID_HDOP 5.0f
#define GNSS_RESYNC_INTERVAL_MS (10 * 60 * 1000)

static const mcu_utils_gpio_t BAUDRATE_SELECT_P1 = {
    .port = GPIOB,
    .pin = GPIO_PIN_8,
};
static const mcu_utils_gpio_t BAUDRATE_SELECT_P2 = {
    .port = GPIOB,
    .pin = GPIO_PIN_9,
};
static const mcu_utils_gpio_t SAMPLERATE_SELECT_P1 = {
    .port = GPIOB,
    .pin = GPIO_PIN_3,
};
static const mcu_utils_gpio_t SAMPLERATE_SELECT_P2 = {
    .port = GPIOB,
    .pin = GPIO_PIN_4,
};
static const mcu_utils_gpio_t OPTIONS_USE_ACCELEROMETER_PIN = {
    .port = GPIOB,
    .pin = GPIO_PIN_12,
};
static const mcu_utils_gpio_t OPTIONS_USE_GNSS_PIN = {
    .port = GPIOB,
    .pin = GPIO_PIN_11,
};
static const mcu_utils_gpio_t OPTIONS_CHANNEL_6D_PIN = {
    .port = GPIOB,
    .pin = GPIO_PIN_10,
};
static const mcu_utils_gpio_t EEPROM_WP_PIN = {
    .port = GPIOB,
    .pin = GPIO_PIN_5,
};
static const mcu_utils_gpio_t MCU_BOOT1_PIN = {
    .port = GPIOB,
    .pin = GPIO_PIN_2,
};
static const mcu_utils_gpio_t MCU_STATE_PIN = {
    .port = GPIOC,
    .pin = GPIO_PIN_13,
};
static const gnss_ctl_pin_t GNSS_CTL_PIN = {
    .pps = {.port = GPIOA, .pin = GPIO_PIN_8},
    .rst = {.port = GPIOA, .pin = GPIO_PIN_15},
};
static const lsm6dsr_ints_pin_t LSM6DSR_INTS_PIN = {
    .int_1 = {.port = GPIOB, .pin = GPIO_PIN_0},
    .int_2 = {.port = GPIOB, .pin = GPIO_PIN_1},
};
static const lsm6ds3_ints_pin_t LSM6DS3_INTS_PIN = {
    .int_1 = {.port = GPIOB, .pin = GPIO_PIN_0},
    .int_2 = {.port = GPIOB, .pin = GPIO_PIN_1},
};
static const lsm6ds3tr_c_ints_pin_t LSM6DS3TR_C_INTS_PIN = {
    .int_1 = {.port = GPIOB, .pin = GPIO_PIN_0},
    .int_2 = {.port = GPIOB, .pin = GPIO_PIN_1},
};
static const icm42688_ints_pin_t ICM42688_INTS_PIN = {
    .int_1 = {.port = GPIOB, .pin = GPIO_PIN_1},
    .int_2 = {.port = GPIOB, .pin = GPIO_PIN_0},
};
static const ads1262_ctl_pin_t ADS1262_CTL_PIN = {
    .start = {.port = GPIOA, .pin = GPIO_PIN_12},
    .drdy = {.port = GPIOA, .pin = GPIO_PIN_11},
    .rst = {.port = GPIOA, .pin = GPIO_PIN_1},
};

#define IIR_DF1_ORDER 3
#define ANTIALIAS_TAPS 61
#define ANTIALIAS_DELAY_MS 120
#define RESAMPLE_250_PHASE_TAPS 41
#define RESAMPLE_250_DELAY_MS 80

#define FILTER_COEFF_SCALE (1LL << 30)

/* scipy.signal.firwin(61, 26.7461839682361, window=("kaiser", 5.65326), fs=250).
 * The design cutoff gives -3.0103 dB at 25 Hz (firwin's cutoff is -6 dB).
 * Symmetric Q30 coefficients sum to exactly 1 << 30. Their absolute sum
 * is below 2 << 30, so full-scale int32 inputs fit in an int64 accumulator.
 * Unlike the previous Butterworth IIR, the 120 ms group delay is constant
 * across the passband and can be removed from the sample timestamps.
 */
static const int32_t ANTIALIAS_COEFFS[ANTIALIAS_TAPS] = {
    224896, 230527, -16116, -532453, -1110531, -1340874,
    -807029, 610603, 2489120, 3873499, 3647597, 1172717,
    -3107241, -7484016, -9493486, -7057386, 188636, 10073732,
    18194432, 19521516, 10847538, -6941579, -27940326, -42409190,
    -39983261, -13856233, 35721472, 100223599, 164705509, 212307652,
    229835176, 212307652, 164705509, 100223599, 35721472, -13856233,
    -39983261, -42409190, -27940326, -6941579, 10847538, 19521516,
    18194432, 10073732, 188636, -7057386, -9493486, -7484016,
    -3107241, 1172717, 3647597, 3873499, 2489120, 610603,
    -807029, -1340874, -1110531, -532453, -16116, 230527,
    224896,
};

/* Four phases of scipy.signal.firwin(161, 37.5, window=("kaiser", 5.65326), fs=1000).
 * Each phase is normalized separately to unity DC gain, quantized to Q30, and
 * adjusted so its integer coefficients sum to exactly 1 << 30. Phases 1 and 3
 * are reverses of one another; phases 0 and 2 are individually symmetric.
 * The FIR adds at most 0.012 dB error over 0..25 Hz to the existing 25 Hz LPF.
 * Its 161 taps at the virtual 1000 Hz rate give 80 ms of common group delay.
 * Phase 0 has 41 taps; phases 1..3 have 40 (the final array element is unused).
 * 100 Hz uses the original two phases (now numbered 0 and 2); 200 Hz uses all four.
 * Each phase's absolute coefficient sum is below 2 * 2^30, so full-scale int32
 * inputs fit safely in a signed 64-bit accumulator.
 */
static const int32_t RESAMPLE_250_COEFFS[4][RESAMPLE_250_PHASE_TAPS] = {
    {
        0, -577164, -1179040, -607561, 1725844, 4199196,
        3415876, -2422225, -9852038, -10898538, 0, 17825510,
        26552685, 10927477, -26467905, -58127864, -45305028, 33235500,
        158464148, 274841779, 322240520, 274841779, 158464148, 33235500,
        -45305028, -58127864, -26467905, 10927477, 26552685, 17825510,
        0, -10898538, -9852038, -2422225, 3415876, 4199196,
        1725844, -607561, -1179040, -577164, 0,
    },
    {
        -99586, -765408, -1194567, -171362, 2450477, 4441199,
        2402049, -4399124, -11043733, -9324030, 4294179, 21605225,
        25251266, 2945180, -36438585, -60462501, -31864290, 61880718,
        190993846, 295019970, 319157587, 250200363, 125427132, 7711348,
        -54427516, -52930177, -16208222, 17413328, 26241073, 13479852,
        -3798268, -11670574, -8252052, -571906, 4085315, 3746792,
        1021146, -921457, -1087671, -395192,
    },
    {
        -232931, -942404, -1113990, 378249, 3143477, 4417783,
        1058916, -6386716, -11689264, -6941092, 8872914, 24507536,
        22213732, -6235956, -45480685, -59376969, -14121701, 92904423,
        221923118, 309972472, 309972472, 221923118, 92904423, -14121701,
        -59376969, -45480685, -6235956, 22213732, 24507536, 8872914,
        -6941092, -11689264, -6386716, 1058916, 4417783, 3143477,
        378249, -1113990, -942404, -232931,
    },
    {
        -395192, -1087671, -921457, 1021146, 3746792, 4085315,
        -571906, -8252052, -11670574, -3798268, 13479852, 26241073,
        17413328, -16208222, -52930177, -54427516, 7711348, 125427132,
        250200363, 319157587, 295019970, 190993846, 61880718, -31864290,
        -60462501, -36438585, 2945180, 25251266, 21605225, 4294179,
        -9324030, -11043733, -4399124, 2402049, 4441199, 2450477,
        -171362, -1194567, -765408, -99586,
    },
};

/* SINC2 / 4800 SPS, 250 Hz acquisition.
 * Common 1PPS calibration on 2026-11-11 minimizes 15..25 Hz phase error.
 * Fit each channel independently; CH3 is not constrained to twice CH2.
 */
static const int32_t ADC_CHANNEL_DELAYS_US[2][3] = {
    {0, 965, 1960}, // 6D disabled (ADC only).
    {0, 960, 1845}, // 6D enabled.
};
#define ADC_US_PER_SECOND 1000000

static const float IIR_DF1_B_COEFFS_250_HZ[] = {1.0035599006842317f, -1.9821566718388282f, 0.9789035443346837f};
static const float IIR_DF1_A_COEFFS_250_HZ[] = {1.0f, -1.982231799964564f, 0.9823883168931801f};

static const uint8_t ANYSHAKE_LOGO_BITMAP_RLE[] = {
    255, 0x00,
    255, 0x00,
    12, 0x00,
    1, 0xE0,
    2, 0xF0,
    1, 0xE0,
    2, 0x00,
    2, 0x80,
    12, 0x00,
    1, 0xFE,
    2, 0xFF,
    1, 0xFE,
    1, 0x00,
    1, 0xE0,
    2, 0xF0,
    1, 0xE0,
    15, 0x00,
    1, 0x0C,
    1, 0xEC,
    1, 0xFC,
    1, 0xBC,
    1, 0xFC,
    1, 0xF8,
    1, 0xC0,
    2, 0x00,
    1, 0x30,
    2, 0xF0,
    1, 0x70,
    1, 0x30,
    1, 0x70,
    1, 0xF0,
    1, 0xE0,
    1, 0x00,
    1, 0x30,
    2, 0xF0,
    1, 0xB0,
    1, 0x00,
    1, 0xB0,
    2, 0xF0,
    1, 0x30,
    1, 0x00,
    1, 0x38,
    1, 0x7C,
    1, 0xFC,
    2, 0xCC,
    2, 0xDC,
    1, 0x98,
    1, 0x00,
    1, 0x06,
    2, 0xFE,
    3, 0x30,
    1, 0xF0,
    1, 0xE0,
    2, 0x00,
    1, 0xA0,
    4, 0xB0,
    1, 0xF0,
    1, 0xE0,
    1, 0x00,
    1, 0x04,
    1, 0x0E,
    2, 0xFE,
    1, 0xE0,
    1, 0xF0,
    1, 0x70,
    2, 0x30,
    1, 0x00,
    1, 0xC0,
    1, 0xE0,
    1, 0xF0,
    2, 0xB0,
    1, 0xF0,
    1, 0xE0,
    1, 0xC0,
    1, 0x00,
    1, 0xC0,
    2, 0xE0,
    1, 0xC0,
    1, 0x00,
    1, 0xFC,
    2, 0xFE,
    1, 0xFC,
    1, 0x00,
    4, 0xFF,
    1, 0x00,
    4, 0xFF,
    1, 0x00,
    1, 0xF0,
    2, 0xF8,
    1, 0xF0,
    1, 0x00,
    1, 0xFE,
    2, 0xFF,
    1, 0xFE,
    1, 0x00,
    4, 0xFF,
    1, 0x00,
    4, 0xFF,
    1, 0x00,
    1, 0xFE,
    2, 0xFF,
    1, 0xFE,
    1, 0x00,
    1, 0xE0,
    2, 0xF0,
    1, 0xE0,
    4, 0x00,
    1, 0x0C,
    2, 0x0F,
    1, 0x0D,
    2, 0x01,
    3, 0x0F,
    2, 0x0C,
    2, 0x0F,
    1, 0x0C,
    1, 0x00,
    1, 0x0C,
    2, 0x0F,
    1, 0x0C,
    2, 0x60,
    1, 0x63,
    1, 0x7F,
    1, 0x7E,
    1, 0x6F,
    1, 0x03,
    3, 0x00,
    1, 0x07,
    1, 0x0F,
    1, 0x0E,
    2, 0x0C,
    1, 0x0E,
    2, 0x07,
    1, 0x00,
    1, 0x0C,
    2, 0x0F,
    1, 0x0C,
    1, 0x00,
    1, 0x0E,
    2, 0x0F,
    1, 0x0C,
    1, 0x03,
    1, 0x07,
    1, 0x0F,
    3, 0x0D,
    2, 0x0F,
    1, 0x0C,
    1, 0x04,
    1, 0x0E,
    2, 0x0F,
    1, 0x01,
    2, 0x0F,
    2, 0x0E,
    1, 0x00,
    1, 0x03,
    1, 0x07,
    1, 0x0F,
    4, 0x0D,
    1, 0x01,
    1, 0x00,
    1, 0x01,
    2, 0x03,
    1, 0x01,
    1, 0x00,
    1, 0x0F,
    2, 0x1F,
    1, 0x0F,
    1, 0x00,
    4, 0xFF,
    1, 0x00,
    1, 0x7F,
    2, 0xFF,
    1, 0x7F,
    1, 0x00,
    1, 0x03,
    2, 0x07,
    1, 0x03,
    1, 0x00,
    1, 0x1F,
    2, 0x3F,
    1, 0x1F,
    1, 0x00,
    4, 0xFF,
    1, 0x00,
    4, 0xFF,
    1, 0x00,
    1, 0x1F,
    2, 0x3F,
    1, 0x1F,
    2, 0x00,
    2, 0x01,
    5, 0x00,
    1, 0x0C,
    3, 0xFC,
    1, 0xEC,
    1, 0xFC,
    1, 0x0C,
    2, 0xBC,
    1, 0x00,
    1, 0x30,
    1, 0x70,
    2, 0xF0,
    1, 0xC0,
    1, 0xF0,
    1, 0x70,
    1, 0x30,
    1, 0x00,
    1, 0x30,
    2, 0xF0,
    4, 0x30,
    1, 0xF0,
    1, 0xE0,
    2, 0x00,
    2, 0x06,
    2, 0xFE,
    4, 0x00,
    1, 0xE0,
    1, 0xF0,
    1, 0x30,
    2, 0x38,
    1, 0x30,
    1, 0xF0,
    1, 0xE0,
    2, 0x00,
    1, 0x30,
    3, 0xF0,
    3, 0x30,
    1, 0x20,
    1, 0x00,
    1, 0xE0,
    1, 0xF0,
    1, 0xF8,
    2, 0xD8,
    1, 0xF8,
    1, 0xF0,
    1, 0xE0,
    1, 0x00,
    2, 0x30,
    3, 0xF0,
    2, 0x30,
    1, 0x20,
    11, 0x00,
    1, 0x03,
    2, 0x07,
    1, 0x03,
    16, 0x00,
    1, 0x1F,
    2, 0x3F,
    1, 0x1F,
    2, 0x00,
    2, 0x01,
    15, 0x00,
    1, 0x06,
    3, 0x07,
    1, 0x06,
    1, 0x07,
    1, 0x06,
    2, 0x07,
    1, 0x00,
    1, 0x06,
    3, 0x07,
    1, 0x01,
    2, 0x07,
    1, 0x06,
    1, 0x04,
    1, 0x30,
    2, 0x3F,
    2, 0x36,
    2, 0x06,
    1, 0x07,
    1, 0x03,
    1, 0x00,
    3, 0x06,
    2, 0x07,
    3, 0x06,
    1, 0x00,
    1, 0x03,
    1, 0x07,
    1, 0x06,
    2, 0x0E,
    1, 0x06,
    1, 0x07,
    1, 0x03,
    1, 0x00,
    2, 0x06,
    3, 0x07,
    3, 0x06,
    2, 0x00,
    1, 0x01,
    1, 0x03,
    1, 0x07,
    4, 0x06,
    2, 0x00,
    2, 0x06,
    3, 0x07,
    2, 0x06,
    2, 0x00  // end
};

#endif
