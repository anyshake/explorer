#include "User/Inc/lsm6ds3/regs/ctrl3_c.h"

lsm6ds3_reg_ctrl3_c_t lsm6ds3_reg_new_ctrl3_c(void) {
    lsm6ds3_reg_ctrl3_c_t ctrl3_c;
    ctrl3_c.boot = LSM6DS3_REG_CTRL3_C_BOOT_NORMAL_MODE;
    ctrl3_c.bdu = LSM6DS3_REG_CTRL3_C_BDU_CONTINUOUS_UPDATE;
    ctrl3_c.h_lactive = LSM6DS3_REG_CTRL3_C_H_LACTIVE_ACTIVE_HIGH;
    ctrl3_c.pp_od = LSM6DS3_REG_CTRL3_C_PP_OD_PUSH_PULL_MODE;
    ctrl3_c.sim = LSM6DS3_REG_CTRL3_C_SIM_4_WIRE_INTERFACE;
    ctrl3_c.if_inc = LSM6DS3_REG_CTRL3_C_IF_INC_DISABLED;
    ctrl3_c.ble = LSM6DS3_REG_CTRL3_C_BLE_DATA_LSB;
    ctrl3_c.sw_reset = LSM6DS3_REG_CTRL3_C_SW_RESET_NORMAL_MODE;
    return ctrl3_c;
}

void lsm6ds3_reg_set_ctrl3_c(lsm6ds3_reg_ctrl3_c_t* ctrl3_c) {
    uint8_t ctrl3_c_data = __LSM6DS3_REG_CTRL3_C_DEFAULT_VALUE;
    ctrl3_c_data &= ~(0x01 << 7);
    ctrl3_c_data |= (ctrl3_c->boot & 0x01) << 7;
    ctrl3_c_data &= ~(0x01 << 6);
    ctrl3_c_data |= (ctrl3_c->bdu & 0x01) << 6;
    ctrl3_c_data &= ~(0x01 << 5);
    ctrl3_c_data |= (ctrl3_c->h_lactive & 0x01) << 5;
    ctrl3_c_data &= ~(0x01 << 4);
    ctrl3_c_data |= (ctrl3_c->pp_od & 0x01) << 4;
    ctrl3_c_data &= ~(0x01 << 3);
    ctrl3_c_data |= (ctrl3_c->sim & 0x01) << 3;
    ctrl3_c_data &= ~(0x01 << 2);
    ctrl3_c_data |= (ctrl3_c->if_inc & 0x01) << 2;
    ctrl3_c_data &= ~(0x01 << 1);
    ctrl3_c_data |= (ctrl3_c->ble & 0x01) << 1;
    ctrl3_c_data &= ~0x01;
    ctrl3_c_data |= (ctrl3_c->sw_reset & 0x01);
    lsm6ds3_write_reg(LSM6DS3_REG_CTRL3_C, ctrl3_c_data);
}

void lsm6ds3_reg_get_ctrl3_c(lsm6ds3_reg_ctrl3_c_t* ctrl3_c) {
    uint8_t ctrl3_c_data = 0;
    lsm6ds3_read_reg(LSM6DS3_REG_CTRL3_C, &ctrl3_c_data);
    ctrl3_c->boot = (ctrl3_c_data >> 7) & 0x01;
    ctrl3_c->bdu = (ctrl3_c_data >> 6) & 0x01;
    ctrl3_c->h_lactive = (ctrl3_c_data >> 5) & 0x01;
    ctrl3_c->pp_od = (ctrl3_c_data >> 4) & 0x01;
    ctrl3_c->sim = (ctrl3_c_data >> 3) & 0x01;
    ctrl3_c->if_inc = (ctrl3_c_data >> 2) & 0x01;
    ctrl3_c->ble = (ctrl3_c_data >> 1) & 0x01;
    ctrl3_c->sw_reset = ctrl3_c_data & 0x01;
}
