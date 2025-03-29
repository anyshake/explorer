#include "ads1262/regs/tdacn.h"

ads1262_reg_tdacn_t ads1262_reg_new_tdacn(void) {
    ads1262_reg_tdacn_t tdacn;
    tdacn.out_n = ADS1262_REG_TDACN_OUTN_NOCONN;
    tdacn.mag_n = ADS1262_REG_TDACN_MAGN_2V5;
    return tdacn;
}

void ads1262_reg_set_tdacn(ads1262_reg_tdacn_t* tdacn) {
    uint8_t tdacn_data = __ADS1262_REG_TDACN_DEFAULT_VALUE;
    tdacn_data &= ~(0x01 << 7);
    tdacn_data |= (tdacn->out_n & 0x01) << 7;
    tdacn_data &= ~0x1F;
    tdacn_data |= (tdacn->mag_n & 0x1F);
    ads1262_write_reg(ADS1262_REG_TDACN, &tdacn_data);
}

void ads1262_reg_get_tdacn(ads1262_reg_tdacn_t* tdacn) {
    uint8_t tdacn_data = 0;
    ads1262_read_reg(ADS1262_REG_TDACN, &tdacn_data);
    tdacn->out_n = (tdacn_data >> 7) & 0x01;
    tdacn->mag_n = tdacn_data & 0x1F;
}
