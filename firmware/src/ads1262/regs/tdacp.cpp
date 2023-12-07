#include "ads1262/regs/tdacp.hpp"

void adc_reg_set_tdacp(adc_reg_tdacp_t* tdacp) {
    uint8_t tdacp_data = __ADC_TDACP_DEFAULT_VALUE;
    tdacp_data &= ~(0x01 << 7);
    tdacp_data |= (tdacp->out_p & 0x01) << 7;
    tdacp_data &= ~0x1F;
    tdacp_data |= tdacp->mag_p & 0x1F;
    adc_write_reg(ADC_REG_TDACP, &tdacp_data);
}

void adc_reg_get_tdacp(adc_reg_tdacp_t* tdacp) {
    uint8_t tdacp_data = 0;
    adc_read_reg(ADC_REG_TDACP, &tdacp_data);
    tdacp->out_p = (tdacp_data & 0x80) >> 7;
    tdacp->mag_p = (tdacp_data & 0x1F);
}
