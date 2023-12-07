#include "ads1262/regs/refmux.hpp"

void adc_reg_set_refmux(adc_reg_refmux_t* refmux) {
    uint8_t refmux_data = __ADC_REFMUX_DEFAULT_VALUE;
    refmux_data &= ~(0x07 << 3);
    refmux_data |= (refmux->refmux_p & 0x07) << 3;
    refmux_data &= ~0x07;
    refmux_data |= refmux->refmux_n & 0x07;
    adc_write_reg(ADC_REG_REFMUX, &refmux_data);
}

void adc_reg_get_refmux(adc_reg_refmux_t* refmux) {
    uint8_t refmux_data = 0;
    adc_read_reg(ADC_REG_REFMUX, &refmux_data);
    refmux->refmux_p = (refmux_data & 0x70) >> 3;
    refmux->refmux_n = (refmux_data & 0x07);
}
