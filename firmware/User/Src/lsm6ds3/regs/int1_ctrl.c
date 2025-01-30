#include "User/Inc/lsm6ds3/regs/int1_ctrl.h"

lsm6ds3_reg_int1_ctrl_t lsm6ds3_reg_new_int1_ctrl(void) {
    lsm6ds3_reg_int1_ctrl_t int1_ctrl;
    int1_ctrl.step_detector = LSM6DS3_REG_INT1_CTRL_STEP_DETECTOR_DISABLED;
    int1_ctrl.sign_mot = LSM6DS3_REG_INT1_CTRL_SIGN_MOTION_DISABLED;
    int1_ctrl.full_flag = LSM6DS3_REG_INT1_CTRL_FULL_FLAG_DISABLED;
    int1_ctrl.fifo_ovr = LSM6DS3_REG_INT1_CTRL_FIFO_OVERRUN_DISABLED;
    int1_ctrl.fth = LSM6DS3_REG_INT1_CTRL_FTH_DISABLED;
    int1_ctrl.boot = LSM6DS3_REG_INT1_CTRL_BOOT_DISABLED;
    int1_ctrl.drdy_g = LSM6DS3_REG_INT1_CTRL_DRDY_G_DISABLED;
    int1_ctrl.drdy_xl = LSM6DS3_REG_INT1_CTRL_DRDY_XL_DISABLED;
    return int1_ctrl;
}

void lsm6ds3_reg_set_int1_ctrl(lsm6ds3_reg_int1_ctrl_t* int1_ctrl) {
    uint8_t int1_ctrl_data = __LSM6DS3_REG_INT1_CTRL_DEFAULT_VALUE;
    int1_ctrl_data &= ~(0x01 << 7);
    int1_ctrl_data |= (int1_ctrl->step_detector & 0x01) << 7;
    int1_ctrl_data &= ~(0x01 << 6);
    int1_ctrl_data |= (int1_ctrl->sign_mot & 0x01) << 6;
    int1_ctrl_data &= ~(0x01 << 5);
    int1_ctrl_data |= (int1_ctrl->full_flag & 0x01) << 5;
    int1_ctrl_data &= ~(0x01 << 4);
    int1_ctrl_data |= (int1_ctrl->fifo_ovr & 0x01) << 4;
    int1_ctrl_data &= ~(0x01 << 3);
    int1_ctrl_data |= (int1_ctrl->fth & 0x01) << 3;
    int1_ctrl_data &= ~(0x01 << 2);
    int1_ctrl_data |= (int1_ctrl->boot & 0x01) << 2;
    int1_ctrl_data &= ~(0x01 << 1);
    int1_ctrl_data |= (int1_ctrl->drdy_g & 0x01) << 1;
    int1_ctrl_data &= ~0x01;
    int1_ctrl_data |= (int1_ctrl->drdy_xl & 0x01);
    lsm6ds3_write_reg(LSM6DS3_REG_INT1_CTRL, int1_ctrl_data);
}

void lsm6ds3_reg_get_int1_ctrl(lsm6ds3_reg_int1_ctrl_t* int1_ctrl) {
    uint8_t int1_ctrl_data = 0;
    lsm6ds3_read_reg(LSM6DS3_REG_INT1_CTRL, &int1_ctrl_data);
    int1_ctrl->step_detector = (int1_ctrl_data >> 7) & 0x01;
    int1_ctrl->sign_mot = (int1_ctrl_data >> 6) & 0x01;
    int1_ctrl->full_flag = (int1_ctrl_data >> 5) & 0x01;
    int1_ctrl->fifo_ovr = (int1_ctrl_data >> 4) & 0x01;
    int1_ctrl->fth = (int1_ctrl_data >> 3) & 0x01;
    int1_ctrl->boot = (int1_ctrl_data >> 2) & 0x01;
    int1_ctrl->drdy_g = (int1_ctrl_data >> 1) & 0x01;
    int1_ctrl->drdy_xl = int1_ctrl_data & 0x01;
}
