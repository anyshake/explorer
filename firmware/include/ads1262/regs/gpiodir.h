#ifndef __ADS1262_REGS_GPIODIR_H
#define __ADS1262_REGS_GPIODIR_H

#include <stdint.h>

#include "ads1262/reg_map.h"
#include "ads1262/utils.h"

#define __ADS1262_REG_GPIODIR_DEFAULT_VALUE 0x00

#define ADS1262_REG_GPIODIR_GPIO0_OUTPUT 0x00
#define ADS1262_REG_GPIODIR_GPIO0_INPUT 0x01

#define ADS1262_REG_GPIODIR_GPIO1_OUTPUT 0x00
#define ADS1262_REG_GPIODIR_GPIO1_INPUT 0x01

#define ADS1262_REG_GPIODIR_GPIO2_OUTPUT 0x00
#define ADS1262_REG_GPIODIR_GPIO2_INPUT 0x01

#define ADS1262_REG_GPIODIR_GPIO3_OUTPUT 0x00
#define ADS1262_REG_GPIODIR_GPIO3_INPUT 0x01

#define ADS1262_REG_GPIODIR_GPIO4_OUTPUT 0x00
#define ADS1262_REG_GPIODIR_GPIO4_INPUT 0x01

#define ADS1262_REG_GPIODIR_GPIO5_OUTPUT 0x00
#define ADS1262_REG_GPIODIR_GPIO5_INPUT 0x01

#define ADS1262_REG_GPIODIR_GPIO6_OUTPUT 0x00
#define ADS1262_REG_GPIODIR_GPIO6_INPUT 0x01

#define ADS1262_REG_GPIODIR_GPIO7_OUTPUT 0x00
#define ADS1262_REG_GPIODIR_GPIO7_INPUT 0x01

typedef struct {
    // GPIO0 direction, whether GPIO0 is input or output
    // 0x00: Output (default)
    // 0x01: Input
    uint8_t gpio_0;
    // GPIO1 direction, whether GPIO1 is input or output
    // 0x00: Output (default)
    // 0x01: Input
    uint8_t gpio_1;
    // GPIO2 direction, whether GPIO2 is input or output
    // 0x00: Output (default)
    // 0x01: Input
    uint8_t gpio_2;
    // GPIO3 direction, whether GPIO3 is input or output
    // 0x00: Output (default)
    // 0x01: Input
    uint8_t gpio_3;
    // GPIO4 direction, whether GPIO4 is input or output
    // 0x00: Output (default)
    // 0x01: Input
    uint8_t gpio_4;
    // GPIO5 direction, whether GPIO5 is input or output
    // 0x00: Output (default)
    // 0x01: Input
    uint8_t gpio_5;
    // GPIO6 direction, whether GPIO6 is input or output
    // 0x00: Output (default)
    // 0x01: Input
    uint8_t gpio_6;
    // GPIO7 direction, whether GPIO7 is input or output
    // 0x00: Output (default)
    // 0x01: Input
    uint8_t gpio_7;
} ads1262_reg_gpiodir_t;

ads1262_reg_gpiodir_t ads1262_reg_new_gpiodir(void);
void ads1262_reg_set_gpiodir(ads1262_reg_gpiodir_t* gpiodir);
void ads1262_reg_get_gpiodir(ads1262_reg_gpiodir_t* gpiodir);

#endif
