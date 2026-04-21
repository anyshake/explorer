#include "User/Inc/lsm6dsr/regs/ctrl4_c.h"

lsm6dsr_reg_ctrl4_c_t lsm6dsr_reg_new_ctrl4_c(void) {
    lsm6dsr_reg_ctrl4_c_t ctrl4_c;
    ctrl4_c.sleep_g = LSM6DSR_REG_CTRL4_C_SLEEP_G_DISABLE;
    ctrl4_c.int2_on_int1 = LSM6DSR_REG_CTRL4_C_INT2_ON_INT1_OFF;
    ctrl4_c.drdy_mask = LSM6DSR_REG_CTRL4_C_DRDY_MASK_DISABLE;
    ctrl4_c.i2c_disable = LSM6DSR_REG_CTRL4_C_I2C_DISABLE_BYPASS;
    ctrl4_c.lpf1_sel_g = LSM6DSR_REG_CTRL4_C_LPF1_SEL_G_DISABLE;
    return ctrl4_c;
}

void lsm6dsr_reg_set_ctrl4_c(lsm6dsr_reg_ctrl4_c_t* ctrl4_c) {
    uint8_t ctrl4_c_data = __LSM6DSR_REG_CTRL4_C_DEFAULT_VALUE;
    ctrl4_c_data &= ~(0x01 << 6);
    ctrl4_c_data |= (ctrl4_c->sleep_g & 0x01) << 6;
    ctrl4_c_data &= ~(0x01 << 5);
    ctrl4_c_data |= (ctrl4_c->int2_on_int1 & 0x01) << 5;
    ctrl4_c_data &= ~(0x01 << 3);
    ctrl4_c_data |= (ctrl4_c->drdy_mask & 0x01) << 3;
    ctrl4_c_data &= ~(0x01 << 2);
    ctrl4_c_data |= (ctrl4_c->i2c_disable & 0x01) << 2;
    ctrl4_c_data &= ~(0x01 << 1);
    ctrl4_c_data |= (ctrl4_c->lpf1_sel_g & 0x01) << 1;
    lsm6dsr_write_reg(LSM6DSR_REG_CTRL4_C, ctrl4_c_data);
}

void lsm6dsr_reg_get_ctrl4_c(lsm6dsr_reg_ctrl4_c_t* ctrl4_c) {
    uint8_t ctrl4_c_data = 0;
    lsm6dsr_read_reg(LSM6DSR_REG_CTRL4_C, &ctrl4_c_data);
    ctrl4_c->sleep_g = (ctrl4_c_data >> 6) & 0x01;
    ctrl4_c->int2_on_int1 = (ctrl4_c_data >> 5) & 0x01;
    ctrl4_c->drdy_mask = (ctrl4_c_data >> 3) & 0x01;
    ctrl4_c->i2c_disable = (ctrl4_c_data >> 2) & 0x01;
    ctrl4_c->lpf1_sel_g = (ctrl4_c_data >> 1) & 0x01;
}
