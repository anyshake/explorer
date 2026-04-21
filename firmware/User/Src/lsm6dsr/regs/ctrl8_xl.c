#include "User/Inc/lsm6dsr/regs/ctrl8_xl.h"

lsm6dsr_reg_ctrl8_xl_t lsm6dsr_reg_new_ctrl8_xl(void) {
    lsm6dsr_reg_ctrl8_xl_t ctrl8_xl;
    ctrl8_xl.hpcf_xl = LSM6DSR_REG_CTRL8_XL_HPCF_XL_LPF2_ODR_4;
    ctrl8_xl.hp_ref_mode_xl = LSM6DSR_REG_CTRL8_XL_HP_REF_MODE_XL_DISABLE;
    ctrl8_xl.fastsettl_mode_xl = LSM6DSR_REG_CTRL8_XL_FASTSETTL_MODE_XL_DISABLE;
    ctrl8_xl.hp_slope_xl_en = LSM6DSR_REG_CTRL8_XL_HP_SLOPE_XL_EN_DISABLE;
    ctrl8_xl.low_pass_on_6d = LSM6DSR_REG_CTRL8_XL_LOW_PASS_ON_6D_DISABLE;
    return ctrl8_xl;
}

void lsm6dsr_reg_set_ctrl8_xl(lsm6dsr_reg_ctrl8_xl_t* ctrl8_xl) {
    uint8_t ctrl8_xl_data = __LSM6DSR_REG_CTRL8_XL_DEFAULT_VALUE;
    ctrl8_xl_data &= ~(0x07 << 5);
    ctrl8_xl_data |= (ctrl8_xl->hpcf_xl & 0x07) << 5;
    ctrl8_xl_data &= ~(0x01 << 4);
    ctrl8_xl_data |= (ctrl8_xl->hp_ref_mode_xl & 0x01) << 4;
    ctrl8_xl_data &= ~(0x01 << 3);
    ctrl8_xl_data |= (ctrl8_xl->fastsettl_mode_xl & 0x01) << 3;
    ctrl8_xl_data &= ~(0x01 << 2);
    ctrl8_xl_data |= (ctrl8_xl->hp_slope_xl_en & 0x01) << 2;
    ctrl8_xl_data &= ~0x01;
    ctrl8_xl_data |= (ctrl8_xl->low_pass_on_6d & 0x01);
    lsm6dsr_write_reg(LSM6DSR_REG_CTRL8_XL, ctrl8_xl_data);
}

void lsm6dsr_reg_get_ctrl8_xl(lsm6dsr_reg_ctrl8_xl_t* ctrl8_xl) {
    uint8_t ctrl8_xl_data = 0;
    lsm6dsr_read_reg(LSM6DSR_REG_CTRL8_XL, &ctrl8_xl_data);
    ctrl8_xl->hpcf_xl = (ctrl8_xl_data >> 5) & 0x07;
    ctrl8_xl->hp_ref_mode_xl = (ctrl8_xl_data >> 4) & 0x01;
    ctrl8_xl->fastsettl_mode_xl = (ctrl8_xl_data >> 3) & 0x01;
    ctrl8_xl->hp_slope_xl_en = (ctrl8_xl_data >> 2) & 0x01;
    ctrl8_xl->low_pass_on_6d = ctrl8_xl_data & 0x01;
}
