#include "ads1262/regs/tdacn.hpp"

void adc_reg_set_tdacn(adc_reg_tdacn_t* tdacn) {
    uint8_t tdacn_data = __ADC_TDACN_DEFAULT_VALUE;
    tdacn_data &= ~(0x01 << 7);
    tdacn_data |= (tdacn->out_n & 0x01) << 7;
    tdacn_data &= ~0x1F;
    tdacn_data |= tdacn->mag_n & 0x1F;
    adc_write_reg(ADC_REG_TDACN, &tdacn_data);
}

void adc_reg_get_tdacn(adc_reg_tdacn_t* tdacn) {
    uint8_t tdacn_data = 0;
    adc_read_reg(ADC_REG_TDACN, &tdacn_data);
    tdacn->out_n = (tdacn_data & 0x80) >> 7;
    tdacn->mag_n = (tdacn_data & 0x1F);
}
