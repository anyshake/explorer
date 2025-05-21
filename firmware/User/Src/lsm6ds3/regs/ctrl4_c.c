#include "User/Inc/lsm6ds3/regs/ctrl4_c.h"

lsm6ds3_reg_ctrl4_c_t lsm6ds3_reg_new_ctrl4_c(void) {
    lsm6ds3_reg_ctrl4_c_t ctrl4_c;
    ctrl4_c.xl_bw_scal_odr = LSM6DS3_REG_CTRL4_C_XL_BW_SCAL_ODR_BYPASS;
    ctrl4_c.sleep_g = LSM6DS3_REG_CTRL4_C_SLEEP_G_DISABLE;
    ctrl4_c.int2_on_int1 = LSM6DS3_REG_CTRL4_C_INT2_ON_INT1_OFF;
    ctrl4_c.fifo_temp_en = LSM6DS3_REG_CTRL4_C_FIFO_TEMP_EN_DISABLE;
    ctrl4_c.drdy_mask = LSM6DS3_REG_CTRL4_C_DRDY_MASK_DISABLE;
    ctrl4_c.i2c_disable = LSM6DS3_REG_CTRL4_C_I2C_DISABLE_BYPASS;
    ctrl4_c.stop_on_fth = LSM6DS3_REG_CTRL4_C_STOP_ON_FTH_DISABLE;
    return ctrl4_c;
}

void lsm6ds3_reg_set_ctrl4_c(lsm6ds3_reg_ctrl4_c_t* ctrl4_c) {
    uint8_t ctrl4_c_data = __LSM6DS3_REG_CTRL4_C_DEFAULT_VALUE;
    ctrl4_c_data &= ~(0x01 << 7);
    ctrl4_c_data |= (ctrl4_c->xl_bw_scal_odr & 0x01) << 7;
    ctrl4_c_data &= ~(0x01 << 6);
    ctrl4_c_data |= (ctrl4_c->sleep_g & 0x01) << 6;
    ctrl4_c_data &= ~(0x01 << 5);
    ctrl4_c_data |= (ctrl4_c->int2_on_int1 & 0x01) << 5;
    ctrl4_c_data &= ~(0x01 << 4);
    ctrl4_c_data |= (ctrl4_c->fifo_temp_en & 0x01) << 4;
    ctrl4_c_data &= ~(0x01 << 3);
    ctrl4_c_data |= (ctrl4_c->drdy_mask & 0x01) << 3;
    ctrl4_c_data &= ~(0x01 << 2);
    ctrl4_c_data |= (ctrl4_c->i2c_disable & 0x01) << 2;
    ctrl4_c_data &= ~0x01;
    ctrl4_c_data |= (ctrl4_c->stop_on_fth & 0x01);
    lsm6ds3_write_reg(LSM6DS3_REG_CTRL4_C, ctrl4_c_data);
}

void lsm6ds3_reg_get_ctrl4_c(lsm6ds3_reg_ctrl4_c_t* ctrl4_c) {
    uint8_t ctrl4_c_data = 0;
    lsm6ds3_read_reg(LSM6DS3_REG_CTRL4_C, &ctrl4_c_data);
    ctrl4_c->xl_bw_scal_odr = (ctrl4_c_data >> 7) & 0x01;
    ctrl4_c->sleep_g = (ctrl4_c_data >> 6) & 0x01;
    ctrl4_c->int2_on_int1 = (ctrl4_c_data >> 5) & 0x01;
    ctrl4_c->fifo_temp_en = (ctrl4_c_data >> 4) & 0x01;
    ctrl4_c->drdy_mask = (ctrl4_c_data >> 3) & 0x01;
    ctrl4_c->i2c_disable = (ctrl4_c_data >> 2) & 0x01;
    ctrl4_c->stop_on_fth = ctrl4_c_data & 0x01;
}
