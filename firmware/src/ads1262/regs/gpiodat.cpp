#include "ads1262/regs/gpiodat.hpp"

void adc_reg_set_gpiodat(adc_reg_gpiodat_t* gpiodat) {
    uint8_t gpiodat_data = __ADC_GPIODAT_DEFAULT_VALUE;
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
    adc_write_reg(ADC_REG_GPIODAT, &gpiodat_data);
}

void adc_reg_get_gpiodat(adc_reg_gpiodat_t* gpiodat) {
    uint8_t gpiodat_data = 0;
    adc_read_reg(ADC_REG_GPIODAT, &gpiodat_data);
    gpiodat->gpio_7 = (gpiodat_data & 0x80) >> 7;
    gpiodat->gpio_6 = (gpiodat_data & 0x40) >> 6;
    gpiodat->gpio_5 = (gpiodat_data & 0x20) >> 5;
    gpiodat->gpio_4 = (gpiodat_data & 0x10) >> 4;
    gpiodat->gpio_3 = (gpiodat_data & 0x08) >> 3;
    gpiodat->gpio_2 = (gpiodat_data & 0x04) >> 2;
    gpiodat->gpio_1 = (gpiodat_data & 0x02) >> 1;
    gpiodat->gpio_0 = (gpiodat_data & 0x01);
}
