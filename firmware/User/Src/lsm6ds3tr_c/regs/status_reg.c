#include "User/Inc/lsm6ds3tr_c/regs/status_reg.h"

void lsm6ds3tr_c_reg_get_status_reg(lsm6ds3tr_c_reg_status_reg_t* status_reg) {
    uint8_t status_reg_data = 0;
    lsm6ds3tr_c_read_reg(LSM6DS3TR_C_REG_STATUS_REG, &status_reg_data);
    status_reg->tda = (status_reg_data >> 2) & 0x01;
    status_reg->gda = (status_reg_data >> 1) & 0x01;
    status_reg->xlda = status_reg_data & 0x01;
}
