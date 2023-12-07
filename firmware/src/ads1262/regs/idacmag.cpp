#include "ads1262/regs/idacmag.hpp"

void adc_reg_set_idacmag(adc_reg_idacmag_t* idacmag) {
    uint8_t idacmag_data = __ADC_IDACMAG_DEFAULT_VALUE;
    idacmag_data &= ~(0x0F << 4);
    idacmag_data |= (idacmag->mag_2 & 0x0F) << 4;
    idacmag_data &= ~0x0F;
    idacmag_data |= idacmag->mag_1 & 0x0F;
    adc_write_reg(ADC_REG_IDACMAG, &idacmag_data);
}

void adc_reg_get_idacmag(adc_reg_idacmag_t* idacmag) {
    uint8_t idacmag_data = 0;
    adc_read_reg(ADC_REG_IDACMAG, &idacmag_data);
    idacmag->mag_2 = (idacmag_data & 0xF0) >> 4;
    idacmag->mag_1 = (idacmag_data & 0x0F);
}
