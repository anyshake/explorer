#include "ads1262/regs/idacmux.hpp"

void adc_reg_set_idacmux(adc_reg_idacmux_t* idacmux) {
    uint8_t idacmux_data = __ADC_IDACMUX_DEFAULT_VALUE;
    idacmux_data &= ~(0x0F << 4);
    idacmux_data |= (idacmux->mux_2 & 0x0F) << 4;
    idacmux_data &= ~0x0F;
    idacmux_data |= (idacmux->mux_1 & 0x0F);
    adc_write_reg(ADC_REG_IDACMUX, &idacmux_data);
}

void adc_reg_get_idacmux(adc_reg_idacmux_t* idacmux) {
    uint8_t idacmux_data = 0;
    adc_read_reg(ADC_REG_IDACMUX, &idacmux_data);
    idacmux->mux_2 = (idacmux_data & 0xF0) >> 4;
    idacmux->mux_1 = idacmux_data & 0x0F;
}
