#ifndef __ADS1262_UTILS_HPP
#define __ADS1262_UTILS_HPP

#include <stdbool.h>
#include <stdint.h>

#include "ads1262/cmds/cmd_map.hpp"
#include "mcu_utils/delay.hpp"
#include "mcu_utils/gpio.hpp"
#include "mcu_utils/spi.hpp"
#include "settings.hpp"

#define ADC_RESET_RESET_TYPE_HARD 0x00
#define ADC_RESET_RESET_TYPE_SOFT 0x01

#define ADC_INIT_CONTROL_TYPE_HARD 0x00
#define ADC_INIT_CONTROL_TYPE_SOFT 0x01

#define ADC_WRITE_CMD_WAIT_DISABLE 0x00
#define ADC_WRITE_CMD_WAIT_ENABLE 0x01

void adc_wait();
void adc_init(uint8_t control_type);
void adc_reset(uint8_t reset_type);
void adc_read_reg(uint8_t reg, uint8_t* rx_data);
void adc_write_reg(uint8_t reg, uint8_t* tx_data);
void adc_write_cmd(uint8_t cmd, uint8_t* rx_data, uint8_t rx_len, uint8_t wait);

#endif
