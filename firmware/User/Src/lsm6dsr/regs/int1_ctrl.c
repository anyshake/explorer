#include "User/Inc/lsm6dsr/regs/int1_ctrl.h"

lsm6dsr_reg_int1_ctrl_t lsm6dsr_reg_new_int1_ctrl(void) {
    lsm6dsr_reg_int1_ctrl_t int1_ctrl;
    int1_ctrl.den_drdy_flag = LSM6DSR_REG_INT1_CTRL_DEN_DRDY_FLAG_DISABLED;
    int1_ctrl.int1_cnt_bdr = LSM6DSR_REG_INT1_CTRL_INT1_CNT_BDR_DISABLED;
    int1_ctrl.int1_fifo_full = LSM6DSR_REG_INT1_CTRL_INT1_FIFO_FULL_DISABLED;
    int1_ctrl.int1_fifo_ovr = LSM6DSR_REG_INT1_CTRL_INT1_FIFO_OVR_DISABLED;
    int1_ctrl.int1_fifo_th = LSM6DSR_REG_INT1_CTRL_INT1_FIFO_TH_DISABLED;
    int1_ctrl.int1_boot = LSM6DSR_REG_INT1_CTRL_INT1_BOOT_DISABLED;
    int1_ctrl.int1_drdy_g = LSM6DSR_REG_INT1_CTRL_INT1_DRDY_G_DISABLED;
    int1_ctrl.int1_drdy_xl = LSM6DSR_REG_INT1_CTRL_INT1_DRDY_XL_DISABLED;
    return int1_ctrl;
}

void lsm6dsr_reg_set_int1_ctrl(lsm6dsr_reg_int1_ctrl_t* int1_ctrl) {
    uint8_t int1_ctrl_data = __LSM6DSR_REG_INT1_CTRL_DEFAULT_VALUE;
    int1_ctrl_data &= ~(0x01 << 7);
    int1_ctrl_data |= (int1_ctrl->den_drdy_flag & 0x01) << 7;
    int1_ctrl_data &= ~(0x01 << 6);
    int1_ctrl_data |= (int1_ctrl->int1_cnt_bdr & 0x01) << 6;
    int1_ctrl_data &= ~(0x01 << 5);
    int1_ctrl_data |= (int1_ctrl->int1_fifo_full & 0x01) << 5;
    int1_ctrl_data &= ~(0x01 << 4);
    int1_ctrl_data |= (int1_ctrl->int1_fifo_ovr & 0x01) << 4;
    int1_ctrl_data &= ~(0x01 << 3);
    int1_ctrl_data |= (int1_ctrl->int1_fifo_th & 0x01) << 3;
    int1_ctrl_data &= ~(0x01 << 2);
    int1_ctrl_data |= (int1_ctrl->int1_boot & 0x01) << 2;
    int1_ctrl_data &= ~(0x01 << 1);
    int1_ctrl_data |= (int1_ctrl->int1_drdy_g & 0x01) << 1;
    int1_ctrl_data &= ~0x01;
    int1_ctrl_data |= (int1_ctrl->int1_drdy_xl & 0x01);
    lsm6dsr_write_reg(LSM6DSR_REG_INT1_CTRL, int1_ctrl_data);
}

void lsm6dsr_reg_get_int1_ctrl(lsm6dsr_reg_int1_ctrl_t* int1_ctrl) {
    uint8_t int1_ctrl_data = 0;
    lsm6dsr_read_reg(LSM6DSR_REG_INT1_CTRL, &int1_ctrl_data);
    int1_ctrl->den_drdy_flag = (int1_ctrl_data >> 7) & 0x01;
    int1_ctrl->int1_cnt_bdr = (int1_ctrl_data >> 6) & 0x01;
    int1_ctrl->int1_fifo_full = (int1_ctrl_data >> 5) & 0x01;
    int1_ctrl->int1_fifo_ovr = (int1_ctrl_data >> 4) & 0x01;
    int1_ctrl->int1_fifo_th = (int1_ctrl_data >> 3) & 0x01;
    int1_ctrl->int1_boot = (int1_ctrl_data >> 2) & 0x01;
    int1_ctrl->int1_drdy_g = (int1_ctrl_data >> 1) & 0x01;
    int1_ctrl->int1_drdy_xl = int1_ctrl_data & 0x01;
}
