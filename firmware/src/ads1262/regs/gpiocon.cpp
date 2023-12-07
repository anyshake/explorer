#include "ads1262/regs/gpiocon.hpp"

void adc_reg_set_gpiocon(adc_reg_gpiocon_t* gpiocon) {
    uint8_t gpiocon_data = __ADC_GPIOCON_DEFAULT_VALUE;
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
    adc_write_reg(ADC_REG_GPIOCON, &gpiocon_data);
}

void adc_reg_get_gpiocon(adc_reg_gpiocon_t* gpiocon) {
    uint8_t gpiocon_data = 0;
    adc_read_reg(ADC_REG_GPIOCON, &gpiocon_data);
    gpiocon->gpio_7 = (gpiocon_data & 0x80) >> 7;
    gpiocon->gpio_6 = (gpiocon_data & 0x40) >> 6;
    gpiocon->gpio_5 = (gpiocon_data & 0x20) >> 5;
    gpiocon->gpio_4 = (gpiocon_data & 0x10) >> 4;
    gpiocon->gpio_3 = (gpiocon_data & 0x08) >> 3;
    gpiocon->gpio_2 = (gpiocon_data & 0x04) >> 2;
    gpiocon->gpio_1 = (gpiocon_data & 0x02) >> 1;
    gpiocon->gpio_0 = (gpiocon_data & 0x01);
}
