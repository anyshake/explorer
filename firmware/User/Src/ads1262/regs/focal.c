#include "User/Inc/ads1262/regs/ofcal.h"

void ads1262_reg_set_ofcal(ads1262_reg_ofcal_t* ofcal) {
    uint8_t ofcal_0_data = (__ADS1262_OFCAL_DEFAULT_VALUE >> 16) & 0xFF;
    ofcal_0_data &= ~(0x01 << 7);
    ofcal_0_data |= (ofcal->ofcal_0 & 0x01) << 7;
    ads1262_write_reg(ADS1262_REG_OFCAL0, &ofcal_0_data);
    uint8_t ofcal_1_data = (__ADS1262_OFCAL_DEFAULT_VALUE >> 8) & 0xFF;
    ofcal_1_data &= ~(0x01 << 7);
    ofcal_1_data |= (ofcal->ofcal_1 & 0x01) << 7;
    ads1262_write_reg(ADS1262_REG_OFCAL1, &ofcal_1_data);
    uint8_t ofcal_2_data = __ADS1262_OFCAL_DEFAULT_VALUE & 0xFF;
    ofcal_2_data &= ~(0x01 << 7);
    ofcal_2_data |= (ofcal->ofcal_2 & 0x01) << 7;
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
