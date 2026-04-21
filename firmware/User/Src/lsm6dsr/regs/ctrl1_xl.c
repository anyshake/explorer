#include "User/Inc/lsm6dsr/regs/ctrl1_xl.h"

lsm6dsr_reg_ctrl1_xl_t lsm6dsr_reg_new_ctrl1_xl(void) {
    lsm6dsr_reg_ctrl1_xl_t ctrl1_xl;
    ctrl1_xl.odr_xl = LSM6DSR_REG_CTRL1_XL_ODR_XL_POWER_DOWN;
    ctrl1_xl.fs_xl = LSM6DSR_REG_CTRL1_XL_FS_XL_2G;
    ctrl1_xl.lpf2_xl_en = LSM6DSR_REG_CTRL1_XL_LPF2_XL_EN_1ST_STAGE;
    return ctrl1_xl;
}

void lsm6dsr_reg_set_ctrl1_xl(lsm6dsr_reg_ctrl1_xl_t* ctrl1_xl) {
    uint8_t ctrl1_xl_data = __LSM6DSR_REG_CTRL1_XL_DEFAULT_VALUE;
    ctrl1_xl_data &= ~(0x0F << 4);
    ctrl1_xl_data |= (ctrl1_xl->odr_xl & 0x0F) << 4;
    ctrl1_xl_data &= ~(0x03 << 2);
    ctrl1_xl_data |= (ctrl1_xl->fs_xl & 0x03) << 2;
    ctrl1_xl_data &= ~(0x01 << 1);
    ctrl1_xl_data |= (ctrl1_xl->lpf2_xl_en & 0x01) << 1;
    lsm6dsr_write_reg(LSM6DSR_REG_CTRL1_XL, ctrl1_xl_data);
}

void lsm6dsr_reg_get_ctrl1_xl(lsm6dsr_reg_ctrl1_xl_t* ctrl1_xl) {
    uint8_t ctrl1_xl_data = 0;
    lsm6dsr_read_reg(LSM6DSR_REG_CTRL1_XL, &ctrl1_xl_data);
    ctrl1_xl->odr_xl = (ctrl1_xl_data >> 4) & 0x0F;
    ctrl1_xl->fs_xl = (ctrl1_xl_data >> 2) & 0x03;
    ctrl1_xl->lpf2_xl_en = (ctrl1_xl_data >> 1) & 0x01;
}
