#include "User/Inc/lsm6ds3/regs/ctrl1_xl.h"

void lsm6ds3_reg_set_ctrl1_xl(lsm6ds3_reg_ctrl1_xl_t* ctrl1_xl) {
    uint8_t ctrl1_xl_data = __LSM6DS3_CTRL1_XL_DEFAULT_VALUE;
    ctrl1_xl_data &= ~(0x0F << 4);
    ctrl1_xl_data |= (ctrl1_xl->odr_xl & 0x0F) << 4;
    ctrl1_xl_data &= ~(0x03 << 2);
    ctrl1_xl_data |= (ctrl1_xl->fs_xl & 0x03) << 2;
    ctrl1_xl_data &= ~(0x01 << 1);
    ctrl1_xl_data |= (ctrl1_xl->lpf1_bw_sel & 0x01) << 1;
    ctrl1_xl_data &= ~0x01;
    ctrl1_xl_data |= ctrl1_xl->bw0_xl & 0x01;
    lsm6ds3_write_reg(LSM6DS3_REG_CTRL1_XL, ctrl1_xl_data);
}

void lsm6ds3_reg_get_ctrl1_xl(lsm6ds3_reg_ctrl1_xl_t* ctrl1_xl) {
    uint8_t ctrl1_xl_data = 0;
    lsm6ds3_read_reg(LSM6DS3_REG_CTRL1_XL, &ctrl1_xl_data);
    ctrl1_xl->odr_xl = (ctrl1_xl_data & 0xF0) >> 4;
    ctrl1_xl->fs_xl = (ctrl1_xl_data & 0x0C) >> 2;
    ctrl1_xl->lpf1_bw_sel = (ctrl1_xl_data & 0x02) >> 1;
    ctrl1_xl->bw0_xl = ctrl1_xl_data & 0x01;
}
