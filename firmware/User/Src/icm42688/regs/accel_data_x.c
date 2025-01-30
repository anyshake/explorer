#include "User/Inc/icm42688/regs/accel_data_x.h"

void icm42688_reg_get_accel_data_x(icm42688_reg_accel_data_x_t* accel_data_x) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    icm42688_read_reg(ICM42688_REG_ACCEL_DATA_X0, &accel_data_x->accel_data_x_l);
    icm42688_read_reg(ICM42688_REG_ACCEL_DATA_X1, &accel_data_x->accel_data_x_h);
}
