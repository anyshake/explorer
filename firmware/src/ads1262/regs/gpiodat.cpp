#include "ads1262/regs/gpiodat.h"

ads1262_reg_gpiodat_t ads1262_reg_new_gpiodat(void) {
    ads1262_reg_gpiodat_t gpiodat;
    gpiodat.gpio_0 = ADS1262_REG_GPIODAT_GPIO0_LOW;
    gpiodat.gpio_1 = ADS1262_REG_GPIODAT_GPIO1_LOW;
    gpiodat.gpio_2 = ADS1262_REG_GPIODAT_GPIO2_LOW;
    gpiodat.gpio_3 = ADS1262_REG_GPIODAT_GPIO3_LOW;
    gpiodat.gpio_4 = ADS1262_REG_GPIODAT_GPIO4_LOW;
    gpiodat.gpio_5 = ADS1262_REG_GPIODAT_GPIO5_LOW;
    gpiodat.gpio_6 = ADS1262_REG_GPIODAT_GPIO6_LOW;
    gpiodat.gpio_7 = ADS1262_REG_GPIODAT_GPIO7_LOW;
    return gpiodat;
}

void ads1262_reg_set_gpiodat(ads1262_reg_gpiodat_t* gpiodat) {
    uint8_t gpiodat_data = __ADS1262_REG_GPIODAT_DEFAULT_VALUE;
    gpiodat_data &= ~(0x01 << 7);
    gpiodat_data |= (gpiodat->gpio_7 & 0x01) << 7;
    gpiodat_data &= ~(0x01 << 6);
    gpiodat_data |= (gpiodat->gpio_6 & 0x01) << 6;
    gpiodat_data &= ~(0x01 << 5);
    gpiodat_data |= (gpiodat->gpio_5 & 0x01) << 5;
    gpiodat_data &= ~(0x01 << 4);
    gpiodat_data |= (gpiodat->gpio_4 & 0x01) << 4;
    gpiodat_data &= ~(0x01 << 3);
    gpiodat_data |= (gpiodat->gpio_3 & 0x01) << 3;
    gpiodat_data &= ~(0x01 << 2);
    gpiodat_data |= (gpiodat->gpio_2 & 0x01) << 2;
    gpiodat_data &= ~(0x01 << 1);
    gpiodat_data |= (gpiodat->gpio_1 & 0x01) << 1;
    gpiodat_data &= ~0x01;
    gpiodat_data |= (gpiodat->gpio_0 & 0x01);
    ads1262_write_reg(ADS1262_REG_GPIODAT, &gpiodat_data);
}

void ads1262_reg_get_gpiodat(ads1262_reg_gpiodat_t* gpiodat) {
    uint8_t gpiodat_data = 0;
    ads1262_read_reg(ADS1262_REG_GPIODAT, &gpiodat_data);
    gpiodat->gpio_7 = (gpiodat_data >> 7) & 0x01;
    gpiodat->gpio_6 = (gpiodat_data >> 6) & 0x01;
    gpiodat->gpio_5 = (gpiodat_data >> 5) & 0x01;
    gpiodat->gpio_4 = (gpiodat_data >> 4) & 0x01;
    gpiodat->gpio_3 = (gpiodat_data >> 3) & 0x01;
    gpiodat->gpio_2 = (gpiodat_data >> 2) & 0x01;
    gpiodat->gpio_1 = (gpiodat_data >> 1) & 0x01;
    gpiodat->gpio_0 = gpiodat_data & 0x01;
}
