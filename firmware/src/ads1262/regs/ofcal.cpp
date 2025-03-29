#include "ads1262/regs/ofcal.h"

ads1262_reg_ofcal_t ads1262_reg_new_ofcal(void) {
    ads1262_reg_ofcal_t ofcal;
    ofcal.ofcal_0 = 0;
    ofcal.ofcal_1 = 0;
    ofcal.ofcal_2 = 0;
    return ofcal;
}

void ads1262_reg_set_ofcal(ads1262_reg_ofcal_t* ofcal) {
    uint8_t ofcal_0_data = ofcal->ofcal_0;
    ads1262_write_reg(ADS1262_REG_OFCAL0, &ofcal_0_data);
    uint8_t ofcal_1_data = ofcal->ofcal_1;
    ads1262_write_reg(ADS1262_REG_OFCAL1, &ofcal_1_data);
    uint8_t ofcal_2_data = ofcal->ofcal_2;
    ads1262_write_reg(ADS1262_REG_OFCAL2, &ofcal_2_data);
}

void ads1262_reg_get_ofcal(ads1262_reg_ofcal_t* ofcal) {
    uint8_t ofcal_0_data = 0;
    ads1262_read_reg(ADS1262_REG_OFCAL0, &ofcal_0_data);
    ofcal->ofcal_0 = ofcal_0_data;
    uint8_t ofcal_1_data = 0;
    ads1262_read_reg(ADS1262_REG_OFCAL1, &ofcal_1_data);
    ofcal->ofcal_1 = ofcal_1_data;
    uint8_t ofcal_2_data = 0;
    ads1262_read_reg(ADS1262_REG_OFCAL2, &ofcal_2_data);
    ofcal->ofcal_2 = ofcal_2_data;
}
