#include "ads1262/regs/mode_2.hpp"

void adc_reg_set_mode_2(adc_reg_mode_2_t* mode_2) {
    uint8_t mode_2_data = __ADC_MODE_2_DEFAULT_VALUE;
    mode_2_data &= ~(0x01 << 7);
    mode_2_data |= (mode_2->bypass & 0x01) << 7;
    mode_2_data &= ~(0x07 << 4);
    mode_2_data |= (mode_2->gain & 0x07) << 4;
    mode_2_data &= ~0x0F;
    mode_2_data |= (mode_2->dr & 0x0F);
    adc_write_reg(ADC_REG_MODE2, &mode_2_data);
}

void adc_reg_get_mode_2(adc_reg_mode_2_t* mode_2) {
    uint8_t mode_2_data = 0;
    adc_read_reg(ADC_REG_MODE2, &mode_2_data);
    mode_2->bypass = (mode_2_data & 0x80) >> 7;
    mode_2->gain = (mode_2_data & 0x70) >> 4;
    mode_2->dr = (mode_2_data & 0x0F);
}
