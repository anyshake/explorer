#include "User/Inc/lsm6ds3tr_c/regs/ctrl8_xl.h"

lsm6ds3tr_c_reg_ctrl8_xl_t lsm6ds3tr_c_reg_new_ctrl8_xl(void) {
    lsm6ds3tr_c_reg_ctrl8_xl_t ctrl8_xl;
    ctrl8_xl.lpf2_xl_en = LSM6DS3TR_C_REG_CTRL8_XL_LPF2_XL_EN_DISABLED;
    ctrl8_xl.hpcf_xl = LSM6DS3TR_C_REG_CTRL8_XL_HPCF_XL_SLOPE_HPF_ODR_4;
    ctrl8_xl.hp_ref_mode = LSM6DS3TR_C_REG_CTRL8_XL_HP_REF_MODE_DISABLED;
    ctrl8_xl.input_composite = LSM6DS3TR_C_REG_CTRL8_XL_INPUT_COMPOSITE_LOW_LATENCY;
    ctrl8_xl.hp_slope_xl_en = LSM6DS3TR_C_REG_CTRL8_XL_HP_SLOPE_XL_EN_DISABLED;
    ctrl8_xl.low_pass_on_6d = LSM6DS3TR_C_REG_CTRL8_XL_LOW_PASS_ON_6D_DISABLED;
    return ctrl8_xl;
}

void lsm6ds3tr_c_reg_set_ctrl8_xl(lsm6ds3tr_c_reg_ctrl8_xl_t* ctrl8_xl) {
    uint8_t ctrl8_xl_data = __LSM6DS3TR_C_REG_CTRL8_XL_DEFAULT_VALUE;
    ctrl8_xl_data &= ~(0x01 << 7);
    ctrl8_xl_data |= (ctrl8_xl->lpf2_xl_en & 0x01) << 7;
    ctrl8_xl_data &= ~(0x03 << 5);
    ctrl8_xl_data |= (ctrl8_xl->hpcf_xl & 0x03) << 5;
    ctrl8_xl_data &= ~(0x01 << 4);
    ctrl8_xl_data |= (ctrl8_xl->hp_ref_mode & 0x01) << 4;
    ctrl8_xl_data &= ~(0x01 << 3);
    ctrl8_xl_data |= (ctrl8_xl->input_composite & 0x01) << 3;
    ctrl8_xl_data &= ~(0x01 << 2);
    ctrl8_xl_data |= (ctrl8_xl->hp_slope_xl_en & 0x01) << 2;
    ctrl8_xl_data &= ~0x01;
    ctrl8_xl_data |= (ctrl8_xl->low_pass_on_6d & 0x01);
    lsm6ds3tr_c_write_reg(LSM6DS3TR_C_REG_CTRL8_XL, ctrl8_xl_data);
}

void lsm6ds3tr_c_reg_get_ctrl8_xl(lsm6ds3tr_c_reg_ctrl8_xl_t* ctrl8_xl) {
    uint8_t ctrl8_xl_data = 0;
    lsm6ds3tr_c_read_reg(LSM6DS3TR_C_REG_CTRL8_XL, &ctrl8_xl_data);
    ctrl8_xl->lpf2_xl_en = (ctrl8_xl_data >> 7) & 0x01;
    ctrl8_xl->hpcf_xl = (ctrl8_xl_data >> 5) & 0x03;
    ctrl8_xl->hp_ref_mode = (ctrl8_xl_data >> 4) & 0x01;
    ctrl8_xl->input_composite = (ctrl8_xl_data >> 3) & 0x01;
    ctrl8_xl->hp_slope_xl_en = (ctrl8_xl_data >> 2) & 0x01;
    ctrl8_xl->low_pass_on_6d = ctrl8_xl_data & 0x01;
}
