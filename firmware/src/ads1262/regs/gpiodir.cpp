#include "ads1262/regs/gpiodir.hpp"

void adc_reg_set_gpiodir(adc_reg_gpiodir_t* gpiodir) {
    uint8_t gpiodir_data = __ADC_GPIODIR_DEFAULT_VALUE;
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
    adc_write_reg(ADC_REG_GPIODIR, &gpiodir_data);
}

void adc_reg_get_gpiodir(adc_reg_gpiodir_t* gpiodir) {
    uint8_t gpiodir_data = 0;
    adc_read_reg(ADC_REG_GPIODIR, &gpiodir_data);
    gpiodir->gpio_7 = (gpiodir_data & 0x80) >> 7;
    gpiodir->gpio_6 = (gpiodir_data & 0x40) >> 6;
    gpiodir->gpio_5 = (gpiodir_data & 0x20) >> 5;
    gpiodir->gpio_4 = (gpiodir_data & 0x10) >> 4;
    gpiodir->gpio_3 = (gpiodir_data & 0x08) >> 3;
    gpiodir->gpio_2 = (gpiodir_data & 0x04) >> 2;
    gpiodir->gpio_1 = (gpiodir_data & 0x02) >> 1;
    gpiodir->gpio_0 = (gpiodir_data & 0x01);
}
