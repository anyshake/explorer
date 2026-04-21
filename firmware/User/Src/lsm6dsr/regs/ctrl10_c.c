#include "User/Inc/lsm6dsr/regs/ctrl10_c.h"

lsm6dsr_reg_ctrl10_c_t lsm6dsr_reg_new_ctrl10_c(void) {
    lsm6dsr_reg_ctrl10_c_t ctrl10_c;
    ctrl10_c.timestamp_en = LSM6DSR_REG_CTRL10_C_TIMESTAMP_EN_ENABLE;
    return ctrl10_c;
}

void lsm6dsr_reg_set_ctrl10_c(lsm6dsr_reg_ctrl10_c_t* ctrl10_c) {
    uint8_t ctrl10_c_data = __LSM6DSR_REG_CTRL10_C_DEFAULT_VALUE;
    ctrl10_c_data &= ~(0x01 << 5);
    ctrl10_c_data |= (ctrl10_c->timestamp_en & 0x01) << 5;
    lsm6dsr_write_reg(LSM6DSR_REG_CTRL10_C, ctrl10_c_data);
}

void lsm6dsr_reg_get_ctrl10_c(lsm6dsr_reg_ctrl10_c_t* ctrl10_c) {
    uint8_t ctrl10_c_data = 0;
    lsm6dsr_read_reg(LSM6DSR_REG_CTRL10_C, &ctrl10_c_data);
    ctrl10_c->timestamp_en = (ctrl10_c_data >> 5) & 0x01;
}
