#include "ads1262/regs/id.hpp"

void adc_reg_get_id(adc_reg_id_t* id) {
    uint8_t id_data = 0;
    adc_read_reg(ADC_REG_ID, &id_data);
    id->dev_id = (id_data & 0xE0) >> 5;
    id->rev_id = id_data & 0x1F;
}
