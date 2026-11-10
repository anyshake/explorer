#include "User/Inc/lsm6ds3tr_c/regs/ctrl4_c.h"

lsm6ds3tr_c_reg_ctrl4_c_t lsm6ds3tr_c_reg_new_ctrl4_c(void) {
    lsm6ds3tr_c_reg_ctrl4_c_t ctrl4_c;
    ctrl4_c.den_xl_en = LSM6DS3TR_C_REG_CTRL4_C_DEN_XL_EN_DISABLED;
    ctrl4_c.sleep = LSM6DS3TR_C_REG_CTRL4_C_SLEEP_DISABLED;
    ctrl4_c.int2_on_int1 = LSM6DS3TR_C_REG_CTRL4_C_INT2_ON_INT1_OFF;
    ctrl4_c.den_drdy_int1 = LSM6DS3TR_C_REG_CTRL4_C_DEN_DRDY_INT1_DISABLED;
    ctrl4_c.drdy_mask = LSM6DS3TR_C_REG_CTRL4_C_DRDY_MASK_DISABLED;
    ctrl4_c.i2c_disable = LSM6DS3TR_C_REG_CTRL4_C_I2C_DISABLE_BYPASS;
    ctrl4_c.lpf1_sel_g = LSM6DS3TR_C_REG_CTRL4_C_LPF1_SEL_G_DISABLED;
    return ctrl4_c;
}

void lsm6ds3tr_c_reg_set_ctrl4_c(lsm6ds3tr_c_reg_ctrl4_c_t* ctrl4_c) {
    uint8_t ctrl4_c_data = __LSM6DS3TR_C_REG_CTRL4_C_DEFAULT_VALUE;
    ctrl4_c_data &= ~(0x01 << 7);
    ctrl4_c_data |= (ctrl4_c->den_xl_en & 0x01) << 7;
    ctrl4_c_data &= ~(0x01 << 6);
    ctrl4_c_data |= (ctrl4_c->sleep & 0x01) << 6;
    ctrl4_c_data &= ~(0x01 << 5);
    ctrl4_c_data |= (ctrl4_c->int2_on_int1 & 0x01) << 5;
    ctrl4_c_data &= ~(0x01 << 4);
    ctrl4_c_data |= (ctrl4_c->den_drdy_int1 & 0x01) << 4;
    ctrl4_c_data &= ~(0x01 << 3);
    ctrl4_c_data |= (ctrl4_c->drdy_mask & 0x01) << 3;
    ctrl4_c_data &= ~(0x01 << 2);
    ctrl4_c_data |= (ctrl4_c->i2c_disable & 0x01) << 2;
    ctrl4_c_data &= ~(0x01 << 1);
    ctrl4_c_data |= (ctrl4_c->lpf1_sel_g & 0x01) << 1;
    lsm6ds3tr_c_write_reg(LSM6DS3TR_C_REG_CTRL4_C, ctrl4_c_data);
}

void lsm6ds3tr_c_reg_get_ctrl4_c(lsm6ds3tr_c_reg_ctrl4_c_t* ctrl4_c) {
    uint8_t ctrl4_c_data = 0;
    lsm6ds3tr_c_read_reg(LSM6DS3TR_C_REG_CTRL4_C, &ctrl4_c_data);
    ctrl4_c->den_xl_en = (ctrl4_c_data >> 7) & 0x01;
    ctrl4_c->sleep = (ctrl4_c_data >> 6) & 0x01;
    ctrl4_c->int2_on_int1 = (ctrl4_c_data >> 5) & 0x01;
    ctrl4_c->den_drdy_int1 = (ctrl4_c_data >> 4) & 0x01;
    ctrl4_c->drdy_mask = (ctrl4_c_data >> 3) & 0x01;
    ctrl4_c->i2c_disable = (ctrl4_c_data >> 2) & 0x01;
    ctrl4_c->lpf1_sel_g = (ctrl4_c_data >> 1) & 0x01;
}
