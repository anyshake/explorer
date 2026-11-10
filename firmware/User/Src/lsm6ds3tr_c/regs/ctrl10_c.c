#include "User/Inc/lsm6ds3tr_c/regs/ctrl10_c.h"

lsm6ds3tr_c_reg_ctrl10_c_t lsm6ds3tr_c_reg_new_ctrl10_c(void) {
    lsm6ds3tr_c_reg_ctrl10_c_t ctrl10_c;
    ctrl10_c.wrist_tilt_en = LSM6DS3TR_C_REG_CTRL10_C_WRIST_TILT_EN_DISABLED;
    ctrl10_c.timer_en = LSM6DS3TR_C_REG_CTRL10_C_TIMER_EN_DISABLED;
    ctrl10_c.pedo_en = LSM6DS3TR_C_REG_CTRL10_C_PEDO_EN_DISABLED;
    ctrl10_c.tilt_en = LSM6DS3TR_C_REG_CTRL10_C_TILT_EN_DISABLED;
    ctrl10_c.func_en = LSM6DS3TR_C_REG_CTRL10_C_FUNC_EN_DISABLED;
    ctrl10_c.pedo_rst_step = LSM6DS3TR_C_REG_CTRL10_C_PEDO_RST_STEP_DISABLED;
    ctrl10_c.sign_motion_en = LSM6DS3TR_C_REG_CTRL10_C_SIGN_MOTION_EN_DISABLED;
    return ctrl10_c;
}

void lsm6ds3tr_c_reg_set_ctrl10_c(lsm6ds3tr_c_reg_ctrl10_c_t* ctrl10_c) {
    uint8_t ctrl10_c_data = __LSM6DS3TR_C_REG_CTRL10_C_DEFAULT_VALUE;
    ctrl10_c_data &= ~(0x01 << 7);
    ctrl10_c_data |= (ctrl10_c->wrist_tilt_en & 0x01) << 7;
    ctrl10_c_data &= ~(0x01 << 5);
    ctrl10_c_data |= (ctrl10_c->timer_en & 0x01) << 5;
    ctrl10_c_data &= ~(0x01 << 4);
    ctrl10_c_data |= (ctrl10_c->pedo_en & 0x01) << 4;
    ctrl10_c_data &= ~(0x01 << 3);
    ctrl10_c_data |= (ctrl10_c->tilt_en & 0x01) << 3;
    ctrl10_c_data &= ~(0x01 << 2);
    ctrl10_c_data |= (ctrl10_c->func_en & 0x01) << 2;
    ctrl10_c_data &= ~(0x01 << 1);
    ctrl10_c_data |= (ctrl10_c->pedo_rst_step & 0x01) << 1;
    ctrl10_c_data &= ~0x01;
    ctrl10_c_data |= (ctrl10_c->sign_motion_en & 0x01);
    lsm6ds3tr_c_write_reg(LSM6DS3TR_C_REG_CTRL10_C, ctrl10_c_data);
}

void lsm6ds3tr_c_reg_get_ctrl10_c(lsm6ds3tr_c_reg_ctrl10_c_t* ctrl10_c) {
    uint8_t ctrl10_c_data = 0;
    lsm6ds3tr_c_read_reg(LSM6DS3TR_C_REG_CTRL10_C, &ctrl10_c_data);
    ctrl10_c->wrist_tilt_en = (ctrl10_c_data >> 7) & 0x01;
    ctrl10_c->timer_en = (ctrl10_c_data >> 5) & 0x01;
    ctrl10_c->pedo_en = (ctrl10_c_data >> 4) & 0x01;
    ctrl10_c->tilt_en = (ctrl10_c_data >> 3) & 0x01;
    ctrl10_c->func_en = (ctrl10_c_data >> 2) & 0x01;
    ctrl10_c->pedo_rst_step = (ctrl10_c_data >> 1) & 0x01;
    ctrl10_c->sign_motion_en = ctrl10_c_data & 0x01;
}
