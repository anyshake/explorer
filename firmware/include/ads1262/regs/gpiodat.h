#ifndef __ADS1262_REGS_GPIODAT_H
#define __ADS1262_REGS_GPIODAT_H

#include <stdint.h>

#include "ads1262/reg_map.h"
#include "ads1262/utils.h"

#define __ADS1262_REG_GPIODAT_DEFAULT_VALUE 0x00

#define ADS1262_REG_GPIODAT_GPIO0_LOW 0x00
#define ADS1262_REG_GPIODAT_GPIO0_HIGH 0x01

#define ADS1262_REG_GPIODAT_GPIO1_LOW 0x00
#define ADS1262_REG_GPIODAT_GPIO1_HIGH 0x01

#define ADS1262_REG_GPIODAT_GPIO2_LOW 0x00
#define ADS1262_REG_GPIODAT_GPIO2_HIGH 0x01

#define ADS1262_REG_GPIODAT_GPIO3_LOW 0x00
#define ADS1262_REG_GPIODAT_GPIO3_HIGH 0x01

#define ADS1262_REG_GPIODAT_GPIO4_LOW 0x00
#define ADS1262_REG_GPIODAT_GPIO4_HIGH 0x01

#define ADS1262_REG_GPIODAT_GPIO5_LOW 0x00
#define ADS1262_REG_GPIODAT_GPIO5_HIGH 0x01

#define ADS1262_REG_GPIODAT_GPIO6_LOW 0x00
#define ADS1262_REG_GPIODAT_GPIO6_HIGH 0x01

#define ADS1262_REG_GPIODAT_GPIO7_LOW 0x00
#define ADS1262_REG_GPIODAT_GPIO7_HIGH 0x01

typedef struct {
    // GPIO0 data, whether GPIO0 is low or high
    // 0x00: Low (default)
    // 0x01: High
    uint8_t gpio_0;
    // GPIO1 data, whether GPIO1 is low or high
    // 0x00: Low (default)
    // 0x01: High
    uint8_t gpio_1;
    // GPIO2 data, whether GPIO2 is low or high
    // 0x00: Low (default)
    // 0x01: High
    uint8_t gpio_2;
    // GPIO3 data, whether GPIO3 is low or high
    // 0x00: Low (default)
    // 0x01: High
    uint8_t gpio_3;
    // GPIO4 data, whether GPIO4 is low or high
    // 0x00: Low (default)
    // 0x01: High
    uint8_t gpio_4;
    // GPIO5 data, whether GPIO5 is low or high
    // 0x00: Low (default)
    // 0x01: High
    uint8_t gpio_5;
    // GPIO6 data, whether GPIO6 is low or high
    // 0x00: Low (default)
    // 0x01: High
    uint8_t gpio_6;
    // GPIO7 data, whether GPIO7 is low or high
    // 0x00: Low (default)
    // 0x01: High
    uint8_t gpio_7;
} ads1262_reg_gpiodat_t;

ads1262_reg_gpiodat_t ads1262_reg_new_gpiodat(void);
void ads1262_reg_set_gpiodat(ads1262_reg_gpiodat_t* gpiodat);
void ads1262_reg_get_gpiodat(ads1262_reg_gpiodat_t* gpiodat);

#endif
