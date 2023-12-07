#include "ads1262/regs/inpmux.hpp"

void adc_reg_set_inpmux(adc_reg_inpmux_t* inpmux) {
    uint8_t inpmux_data = __ADC_INPMUX_DEFAULT_VALUE;
    inpmux_data &= ~(0x0F << 4);
    inpmux_data |= (inpmux->mux_n & 0x0F) << 4;
    inpmux_data &= ~0x0F;
    inpmux_data |= inpmux->mux_p & 0x0F;
    adc_write_reg(ADC_REG_INPMUX, &inpmux_data);
}

void adc_reg_get_inpmux(adc_reg_inpmux_t* inpmux) {
    uint8_t inpmux_data = 0;
    adc_read_reg(ADC_REG_INPMUX, &inpmux_data);
    inpmux->mux_n = (inpmux_data & 0xF0) >> 4;
    inpmux->mux_p = (inpmux_data & 0x0F);
}
