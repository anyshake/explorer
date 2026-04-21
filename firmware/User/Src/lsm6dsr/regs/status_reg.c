#include "User/Inc/lsm6dsr/regs/status_reg.h"

void lsm6dsr_reg_get_status_reg(lsm6dsr_reg_status_reg_t* who_am_i) {
    uint8_t status_reg_data = 0;
    lsm6dsr_read_reg(LSM6DSR_REG_STATUS_REG, &status_reg_data);
    who_am_i->tda = (status_reg_data >> 2) & 0x01;
    who_am_i->gda = (status_reg_data >> 1) & 0x01;
    who_am_i->xlda = status_reg_data & 0x01;
}
