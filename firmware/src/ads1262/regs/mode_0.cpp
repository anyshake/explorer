#include "ads1262/regs/mode_0.h"

ads1262_reg_mode_0_t ads1262_reg_new_mode_0(void) {
    ads1262_reg_mode_0_t mode_0;
    mode_0.ref_rev = ADS1262_REG_MODE_0_REF_REV_NORMAL;
    mode_0.run_mode = ADS1262_REG_MODE_0_RUN_MODE_CONTINUOUS;
    mode_0.chop = ADS1262_REG_MODE_0_CHOP_DISABLED;
    mode_0.delay = ADS1262_REG_MODE_0_DELAY_0;
    return mode_0;
}

void ads1262_reg_set_mode_0(ads1262_reg_mode_0_t* mode_0) {
    uint8_t mode_0_data = __ADS1262_REG_MODE_0_DEFAULT_VALUE;
    mode_0_data &= ~(0x01 << 7);
    mode_0_data |= (mode_0->ref_rev & 0x01) << 7;
    mode_0_data &= ~(0x01 << 6);
    mode_0_data |= (mode_0->run_mode & 0x01) << 6;
    mode_0_data &= ~(0x03 << 4);
    mode_0_data |= (mode_0->chop & 0x03) << 4;
    mode_0_data &= ~0x0F;
    mode_0_data |= (mode_0->delay & 0x0F);
    ads1262_write_reg(ADS1262_REG_MODE0, &mode_0_data);
}

void ads1262_reg_get_mode_0(ads1262_reg_mode_0_t* mode_0) {
    uint8_t mode_0_data = 0;
    ads1262_read_reg(ADS1262_REG_MODE0, &mode_0_data);
    mode_0->ref_rev = (mode_0_data >> 7) & 0x01;
    mode_0->run_mode = (mode_0_data >> 6) & 0x01;
    mode_0->chop = (mode_0_data >> 4) & 0x03;
    mode_0->delay = mode_0_data & 0x0F;
}
