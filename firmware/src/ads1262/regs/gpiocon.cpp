#include "ads1262/regs/gpiocon.h"

ads1262_reg_gpiocon_t ads1262_reg_new_gpiocon(void) {
    ads1262_reg_gpiocon_t gpiocon;
    gpiocon.gpio_0 = ADS1262_REG_GPIOCON_GPIO0_NOCONN;
    gpiocon.gpio_1 = ADS1262_REG_GPIOCON_GPIO1_NOCONN;
    gpiocon.gpio_2 = ADS1262_REG_GPIOCON_GPIO2_NOCONN;
    gpiocon.gpio_3 = ADS1262_REG_GPIOCON_GPIO3_NOCONN;
    gpiocon.gpio_4 = ADS1262_REG_GPIOCON_GPIO4_NOCONN;
    gpiocon.gpio_5 = ADS1262_REG_GPIOCON_GPIO5_NOCONN;
    gpiocon.gpio_6 = ADS1262_REG_GPIOCON_GPIO6_NOCONN;
    gpiocon.gpio_7 = ADS1262_REG_GPIOCON_GPIO7_NOCONN;
    return gpiocon;
}

void ads1262_reg_set_gpiocon(ads1262_reg_gpiocon_t* gpiocon) {
    uint8_t gpiocon_data = __ADS1262_REG_GPIOCON_DEFAULT_VALUE;
    gpiocon_data &= ~(0x01 << 7);
    gpiocon_data |= (gpiocon->gpio_7 & 0x01) << 7;
    gpiocon_data &= ~(0x01 << 6);
    gpiocon_data |= (gpiocon->gpio_6 & 0x01) << 6;
    gpiocon_data &= ~(0x01 << 5);
    gpiocon_data |= (gpiocon->gpio_5 & 0x01) << 5;
    gpiocon_data &= ~(0x01 << 4);
    gpiocon_data |= (gpiocon->gpio_4 & 0x01) << 4;
    gpiocon_data &= ~(0x01 << 3);
    gpiocon_data |= (gpiocon->gpio_3 & 0x01) << 3;
    gpiocon_data &= ~(0x01 << 2);
    gpiocon_data |= (gpiocon->gpio_2 & 0x01) << 2;
    gpiocon_data &= ~(0x01 << 1);
    gpiocon_data |= (gpiocon->gpio_1 & 0x01) << 1;
    gpiocon_data &= ~0x01;
    gpiocon_data |= (gpiocon->gpio_0 & 0x01);
    ads1262_write_reg(ADS1262_REG_GPIOCON, &gpiocon_data);
}

void ads1262_reg_get_gpiocon(ads1262_reg_gpiocon_t* gpiocon) {
    uint8_t gpiocon_data = 0;
    ads1262_read_reg(ADS1262_REG_GPIOCON, &gpiocon_data);
    gpiocon->gpio_7 = (gpiocon_data >> 7) & 0x01;
    gpiocon->gpio_6 = (gpiocon_data >> 6) & 0x01;
    gpiocon->gpio_5 = (gpiocon_data >> 5) & 0x01;
    gpiocon->gpio_4 = (gpiocon_data >> 4) & 0x01;
    gpiocon->gpio_3 = (gpiocon_data >> 3) & 0x01;
    gpiocon->gpio_2 = (gpiocon_data >> 2) & 0x01;
    gpiocon->gpio_1 = (gpiocon_data >> 1) & 0x01;
    gpiocon->gpio_0 = gpiocon_data & 0x01;
}
