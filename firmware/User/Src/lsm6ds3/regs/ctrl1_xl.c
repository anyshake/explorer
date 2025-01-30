#include "User/Inc/lsm6ds3/regs/ctrl1_xl.h"

lsm6ds3_reg_ctrl1_xl_t lsm6ds3_reg_new_ctrl1_xl(void) {
    lsm6ds3_reg_ctrl1_xl_t ctrl1_xl;
    ctrl1_xl.odr_xl = LSM6DS3_REG_CTRL1_XL_ODR_XL_POWER_DOWN;
    ctrl1_xl.fs_xl = LSM6DS3_REG_CTRL1_XL_FS_XL_2G;
    ctrl1_xl.bw_xl = LSM6DS3_REG_CTRL1_XL_BW0_XL_400HZ;
    return ctrl1_xl;
}

void lsm6ds3_reg_set_ctrl1_xl(lsm6ds3_reg_ctrl1_xl_t* ctrl1_xl) {
    uint8_t ctrl1_xl_data = __LSM6DS3_REG_CTRL1_XL_DEFAULT_VALUE;
    ctrl1_xl_data &= ~(0x0F << 4);
    ctrl1_xl_data |= (ctrl1_xl->odr_xl & 0x0F) << 4;
    ctrl1_xl_data &= ~(0x03 << 2);
    ctrl1_xl_data |= (ctrl1_xl->fs_xl & 0x03) << 2;
    ctrl1_xl_data &= ~0x03;
    ctrl1_xl_data |= (ctrl1_xl->bw_xl & 0x03);
    lsm6ds3_write_reg(LSM6DS3_REG_CTRL1_XL, ctrl1_xl_data);
}

void lsm6ds3_reg_get_ctrl1_xl(lsm6ds3_reg_ctrl1_xl_t* ctrl1_xl) {
    uint8_t ctrl1_xl_data = 0;
    lsm6ds3_read_reg(LSM6DS3_REG_CTRL1_XL, &ctrl1_xl_data);
    ctrl1_xl->odr_xl = (ctrl1_xl_data >> 4) & 0x0F;
    ctrl1_xl->fs_xl = (ctrl1_xl_data >> 2) & 0x03;
    ctrl1_xl->bw_xl = ctrl1_xl_data & 0x03;
}
