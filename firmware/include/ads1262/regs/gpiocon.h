#ifndef __ADS1262_REGS_GPIOCON_H
#define __ADS1262_REGS_GPIOCON_H

#include <stdint.h>

#include "ads1262/reg_map.h"
#include "ads1262/utils.h"

#define __ADS1262_REG_GPIOCON_DEFAULT_VALUE 0x00

#define ADS1262_REG_GPIOCON_GPIO0_NOCONN 0x00
#define ADS1262_REG_GPIOCON_GPIO0_TOAIN3 0x01

#define ADS1262_REG_GPIOCON_GPIO1_NOCONN 0x00
#define ADS1262_REG_GPIOCON_GPIO1_TOAIN4 0x01

#define ADS1262_REG_GPIOCON_GPIO2_NOCONN 0x00
#define ADS1262_REG_GPIOCON_GPIO2_TOAIN5 0x01

#define ADS1262_REG_GPIOCON_GPIO3_NOCONN 0x00
#define ADS1262_REG_GPIOCON_GPIO3_TOAIN6 0x01

#define ADS1262_REG_GPIOCON_GPIO4_NOCONN 0x00
#define ADS1262_REG_GPIOCON_GPIO4_TOAIN7 0x01

#define ADS1262_REG_GPIOCON_GPIO5_NOCONN 0x00
#define ADS1262_REG_GPIOCON_GPIO5_TOAIN8 0x01

#define ADS1262_REG_GPIOCON_GPIO6_NOCONN 0x00
#define ADS1262_REG_GPIOCON_GPIO6_TOAIN9 0x01

#define ADS1262_REG_GPIOCON_GPIO7_NOCONN 0x00
#define ADS1262_REG_GPIOCON_GPIO7_TOAINCOM 0x01

typedef struct {
    // GPIO0 configuration, whether connect GPIO0 to pin AIN3
    // 0x00: No connection (default)
    // 0x01: AIN3
    uint8_t gpio_0;
    // GPIO1 configuration, whether connect GPIO1 to pin AIN4
    // 0x00: No connection (default)
    // 0x01: AIN4
    uint8_t gpio_1;
    // GPIO2 configuration, whether connect GPIO2 to pin AIN5
    // 0x00: No connection (default)
    // 0x01: AIN5
    uint8_t gpio_2;
    // GPIO3 configuration, whether connect GPIO3 to pin AIN6
    // 0x00: No connection (default)
    // 0x01: AIN6
    uint8_t gpio_3;
    // GPIO4 configuration, whether connect GPIO4 to pin AIN7
    // 0x00: No connection (default)
    // 0x01: AIN7
    uint8_t gpio_4;
    // GPIO5 configuration, whether connect GPIO5 to pin AIN8
    // 0x00: No connection (default)
    // 0x01: AIN8
    uint8_t gpio_5;
    // GPIO6 configuration, whether connect GPIO6 to pin AIN9
    // 0x00: No connection (default)
    // 0x01: AIN9
    uint8_t gpio_6;
    // GPIO7 configuration, whether connect GPIO7 to pin AINCOM
    // 0x00: No connection (default)
    // 0x01: AINCOM
    uint8_t gpio_7;
} ads1262_reg_gpiocon_t;

ads1262_reg_gpiocon_t ads1262_reg_new_gpiocon(void);
void ads1262_reg_set_gpiocon(ads1262_reg_gpiocon_t* gpiocon);
void ads1262_reg_get_gpiocon(ads1262_reg_gpiocon_t* gpiocon);

#endif
