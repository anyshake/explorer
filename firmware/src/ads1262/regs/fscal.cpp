#include "ads1262/regs/fscal.hpp"

void adc_reg_set_fscal(adc_reg_fscal_t* fscal) {
    uint8_t fscal_0_data = (__ADC_FSCAL_DEFAULT_VALUE >> 16) & 0xFF;
    fscal_0_data &= ~(0x01 << 7);
    fscal_0_data |= (fscal->fscal_0 & 0x01) << 7;
    adc_write_reg(ADC_REG_FSCAL0, &fscal_0_data);
    uint8_t fscal_1_data = (__ADC_FSCAL_DEFAULT_VALUE >> 8) & 0xFF;
    fscal_1_data &= ~(0x01 << 7);
    fscal_1_data |= (fscal->fscal_1 & 0x01) << 7;
    adc_write_reg(ADC_REG_FSCAL1, &fscal_1_data);
    uint8_t fscal_2_data = __ADC_FSCAL_DEFAULT_VALUE & 0xFF;
    fscal_2_data &= ~(0x01 << 7);
    fscal_2_data |= (fscal->fscal_2 & 0x01) << 7;
    adc_write_reg(ADC_REG_FSCAL2, &fscal_2_data);
}

void adc_reg_get_fscal(adc_reg_fscal_t* fscal) {
    uint8_t fscal_0_data = 0;
    adc_read_reg(ADC_REG_FSCAL0, &fscal_0_data);
    fscal->fscal_0 = fscal_0_data;
    uint8_t fscal_1_data = 0;
    adc_read_reg(ADC_REG_FSCAL1, &fscal_1_data);
    fscal->fscal_1 = fscal_1_data;
    uint8_t fscal_2_data = 0;
    adc_read_reg(ADC_REG_FSCAL2, &fscal_2_data);
    fscal->fscal_2 = fscal_2_data;
}
