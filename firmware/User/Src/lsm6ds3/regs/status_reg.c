#include "User/Inc/lsm6ds3/regs/status_reg.h"

void lsm6ds3_reg_get_status_reg(lsm6ds3_reg_status_reg_t* who_am_i) {
    uint8_t status_reg_data = 0;
    lsm6ds3_read_reg(LSM6DS3_REG_STATUS_REG, &status_reg_data);
    who_am_i->tda = (status_reg_data >> 2) & 0x01;
    who_am_i->gda = (status_reg_data >> 1) & 0x01;
    who_am_i->xlda = status_reg_data & 0x01;
}
