#include "ads1262/regs/gpiodir.h"

ads1262_reg_gpiodir_t ads1262_reg_new_gpiodir(void) {
    ads1262_reg_gpiodir_t gpiodir;
    gpiodir.gpio_0 = ADS1262_REG_GPIODIR_GPIO0_OUTPUT;
    gpiodir.gpio_1 = ADS1262_REG_GPIODIR_GPIO1_OUTPUT;
    gpiodir.gpio_2 = ADS1262_REG_GPIODIR_GPIO2_OUTPUT;
    gpiodir.gpio_3 = ADS1262_REG_GPIODIR_GPIO3_OUTPUT;
    gpiodir.gpio_4 = ADS1262_REG_GPIODIR_GPIO4_OUTPUT;
    gpiodir.gpio_5 = ADS1262_REG_GPIODIR_GPIO5_OUTPUT;
    gpiodir.gpio_6 = ADS1262_REG_GPIODIR_GPIO6_OUTPUT;
    gpiodir.gpio_7 = ADS1262_REG_GPIODIR_GPIO7_OUTPUT;
    return gpiodir;
}

void ads1262_reg_set_gpiodir(ads1262_reg_gpiodir_t* gpiodir) {
    uint8_t gpiodir_data = __ADS1262_REG_GPIODIR_DEFAULT_VALUE;
    gpiodir_data &= ~(0x01 << 7);
    gpiodir_data |= (gpiodir->gpio_7 & 0x01) << 7;
    gpiodir_data &= ~(0x01 << 6);
    gpiodir_data |= (gpiodir->gpio_6 & 0x01) << 6;
    gpiodir_data &= ~(0x01 << 5);
    gpiodir_data |= (gpiodir->gpio_5 & 0x01) << 5;
    gpiodir_data &= ~(0x01 << 4);
    gpiodir_data |= (gpiodir->gpio_4 & 0x01) << 4;
    gpiodir_data &= ~(0x01 << 3);
    gpiodir_data |= (gpiodir->gpio_3 & 0x01) << 3;
    gpiodir_data &= ~(0x01 << 2);
    gpiodir_data |= (gpiodir->gpio_2 & 0x01) << 2;
    gpiodir_data &= ~(0x01 << 1);
    gpiodir_data |= (gpiodir->gpio_1 & 0x01) << 1;
    gpiodir_data &= ~0x01;
    gpiodir_data |= (gpiodir->gpio_0 & 0x01);
    ads1262_write_reg(ADS1262_REG_GPIODIR, &gpiodir_data);
}

void ads1262_reg_get_gpiodir(ads1262_reg_gpiodir_t* gpiodir) {
    uint8_t gpiodir_data = 0;
    ads1262_read_reg(ADS1262_REG_GPIODIR, &gpiodir_data);
    gpiodir->gpio_7 = (gpiodir_data >> 7) & 0x01;
    gpiodir->gpio_6 = (gpiodir_data >> 6) & 0x01;
    gpiodir->gpio_5 = (gpiodir_data >> 5) & 0x01;
    gpiodir->gpio_4 = (gpiodir_data >> 4) & 0x01;
    gpiodir->gpio_3 = (gpiodir_data >> 3) & 0x01;
    gpiodir->gpio_2 = (gpiodir_data >> 2) & 0x01;
    gpiodir->gpio_1 = (gpiodir_data >> 1) & 0x01;
    gpiodir->gpio_0 = gpiodir_data & 0x01;
}
