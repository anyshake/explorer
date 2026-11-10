#include "User/Inc/icm42688/regs/accel_data_x.h"

void icm42688_reg_get_accel_data_x(icm42688_reg_accel_data_x_t* accel_data_x) {
    uint8_t data[2] = {0};
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0x00);
    icm42688_read_regs(ICM42688_REG_ACCEL_DATA_X1, data, sizeof(data));
    accel_data_x->accel_data_x_l = data[1];
    accel_data_x->accel_data_x_h = data[0];
}
