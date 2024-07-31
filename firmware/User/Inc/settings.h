#ifndef __SETTINGS_H
#define __SETTINGS_H

#include "User/Inc/ads1262/utils.h"
#include "User/Inc/gnss/utils.h"
#include "User/Inc/lsm6ds3/utils.h"
#include "Utils/Inc/gpio.h"

#define GNSS_UART_BAUDRATE 9600
#define ADC_AVSS_VOLTAGE 0.0
#define ADC_AVDD_VOLTAGE 3.3

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
static const mcu_utils_gpio_t OPTIONS_NO_GEOPHONE_PIN = {
    .port = GPIOB,
    .pin = GPIO_PIN_12,
};
static const mcu_utils_gpio_t OPTIONS_LEGACY_MODE_PIN = {
    .port = GPIOB,
    .pin = GPIO_PIN_11,
};
static const mcu_utils_gpio_t OPTIONS_ADC_24BIT_MODE_PIN = {
    .port = GPIOB,
    .pin = GPIO_PIN_10,
};
static const mcu_utils_gpio_t EEPROM_WP_PIN = {
    .port = GPIOB,
    .pin = GPIO_PIN_5,
};
static const mcu_utils_gpio_t MCU_STATE_PIN = {
    .port = GPIOC,
    .pin = GPIO_PIN_13,
};
static const gnss_ctl_pin_t GNSS_CTL_PIN = {
    .pps = {.port = GPIOA, .pin = GPIO_PIN_8},
    .rst = {.port = GPIOA, .pin = GPIO_PIN_15},
};
static const lsm6ds3_ints_pin_t LSM6DS3_INTS_PIN = {
    .int_1 = {.port = GPIOB, .pin = GPIO_PIN_1},
    .int_2 = {.port = GPIOB, .pin = GPIO_PIN_0},
};
static const ads1262_ctl_pin_t ADS1262_CTL_PIN = {
    .start = {.port = GPIOA, .pin = GPIO_PIN_12},
    .drdy = {.port = GPIOA, .pin = GPIO_PIN_11},
    .rst = {.port = GPIOA, .pin = GPIO_PIN_1},
};

#endif
