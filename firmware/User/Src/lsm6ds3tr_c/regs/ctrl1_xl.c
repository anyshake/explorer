#include "User/Inc/lsm6ds3tr_c/regs/ctrl1_xl.h"

lsm6ds3tr_c_reg_ctrl1_xl_t lsm6ds3tr_c_reg_new_ctrl1_xl(void) {
    lsm6ds3tr_c_reg_ctrl1_xl_t ctrl1_xl;
    ctrl1_xl.odr_xl = LSM6DS3TR_C_REG_CTRL1_XL_ODR_XL_POWER_DOWN;
    ctrl1_xl.fs_xl = LSM6DS3TR_C_REG_CTRL1_XL_FS_XL_2G;
    ctrl1_xl.lpf1_bw_sel = LSM6DS3TR_C_REG_CTRL1_XL_LPF1_BW_SEL_ODR_2;
    ctrl1_xl.bw0_xl = LSM6DS3TR_C_REG_CTRL1_XL_BW0_XL_1500HZ;
    return ctrl1_xl;
}

void lsm6ds3tr_c_reg_set_ctrl1_xl(lsm6ds3tr_c_reg_ctrl1_xl_t* ctrl1_xl) {
    uint8_t ctrl1_xl_data = __LSM6DS3TR_C_REG_CTRL1_XL_DEFAULT_VALUE;
    ctrl1_xl_data &= ~(0x0F << 4);
    ctrl1_xl_data |= (ctrl1_xl->odr_xl & 0x0F) << 4;
    ctrl1_xl_data &= ~(0x03 << 2);
    ctrl1_xl_data |= (ctrl1_xl->fs_xl & 0x03) << 2;
    ctrl1_xl_data &= ~(0x01 << 1);
    ctrl1_xl_data |= (ctrl1_xl->lpf1_bw_sel & 0x01) << 1;
    ctrl1_xl_data &= ~0x01;
    ctrl1_xl_data |= (ctrl1_xl->bw0_xl & 0x01);
    lsm6ds3tr_c_write_reg(LSM6DS3TR_C_REG_CTRL1_XL, ctrl1_xl_data);
}

void lsm6ds3tr_c_reg_get_ctrl1_xl(lsm6ds3tr_c_reg_ctrl1_xl_t* ctrl1_xl) {
    uint8_t ctrl1_xl_data = 0;
    lsm6ds3tr_c_read_reg(LSM6DS3TR_C_REG_CTRL1_XL, &ctrl1_xl_data);
    ctrl1_xl->odr_xl = (ctrl1_xl_data >> 4) & 0x0F;
    ctrl1_xl->fs_xl = (ctrl1_xl_data >> 2) & 0x03;
    ctrl1_xl->lpf1_bw_sel = (ctrl1_xl_data >> 1) & 0x01;
    ctrl1_xl->bw0_xl = ctrl1_xl_data & 0x01;
}
