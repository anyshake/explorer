#ifndef __ADS1262_UTILS_H
#define __ADS1262_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#include "ads1262/cmd_map.h"
#include "utils/delay.h"
#include "utils/gpio.h"
#include "utils/spi.h"

#define ADS1262_RESET_RESET_TYPE_HARD 0x00
#define ADS1262_RESET_RESET_TYPE_SOFT 0x01

#define ADS1262_INIT_CONTROL_TYPE_HARD 0x00
#define ADS1262_INIT_CONTROL_TYPE_SOFT 0x01

#define ADS1262_WRITE_CMD_WAIT_DISABLE 0x00
#define ADS1262_WRITE_CMD_WAIT_ENABLE 0x01

typedef struct {
    mcu_utils_gpio_t start;
    mcu_utils_gpio_t drdy;
    mcu_utils_gpio_t rst;
} ads1262_ctl_pin_t;

void ads1262_wait(ads1262_ctl_pin_t pin);
void ads1262_init(ads1262_ctl_pin_t pin, uint8_t control_type);
void ads1262_reset(ads1262_ctl_pin_t pin, uint8_t reset_type, bool is_rtos);
void ads1262_read_reg(uint8_t reg, uint8_t* rx_data);
void ads1262_write_reg(uint8_t reg, uint8_t* tx_data);
void ads1262_write_cmd(ads1262_ctl_pin_t pin,
                       uint8_t cmd,
                       uint8_t* rx_data,
                       uint8_t rx_len,
                       uint8_t wait);

#endif
