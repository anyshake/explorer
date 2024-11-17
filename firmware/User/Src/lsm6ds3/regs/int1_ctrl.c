#include "User/Inc/lsm6ds3/regs/int1_ctrl.h"

void lsm6ds3_reg_set_int1_ctrl(lsm6ds3_reg_int1_ctrl_t* int1_ctrl) {
    uint8_t int1_ctrl_data = __LSM6DS3_INT1_CTRL_DEFAULT_VALUE;
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
    int1_ctrl_data |= int1_ctrl->drdy_xl & 0x01;
    lsm6ds3_write_reg(LSM6DS3_REG_INT1_CTRL, int1_ctrl_data);
}

void lsm6ds3_reg_get_int1_ctrl(lsm6ds3_reg_int1_ctrl_t* int1_ctrl) {
    uint8_t int1_ctrl_data = 0;
    lsm6ds3_read_reg(LSM6DS3_REG_INT1_CTRL, &int1_ctrl_data);
    int1_ctrl->step_detector = (int1_ctrl_data & 0x80) >> 7;
    int1_ctrl->sign_mot = (int1_ctrl_data & 0x40) >> 6;
    int1_ctrl->full_flag = (int1_ctrl_data & 0x20) >> 5;
    int1_ctrl->fifo_ovr = (int1_ctrl_data & 0x10) >> 4;
    int1_ctrl->fth = (int1_ctrl_data & 0x08) >> 3;
    int1_ctrl->boot = (int1_ctrl_data & 0x04) >> 2;
    int1_ctrl->drdy_g = (int1_ctrl_data & 0x02) >> 1;
    int1_ctrl->drdy_xl = int1_ctrl_data & 0x01;
}
