#include "User/Inc/icm42688/regs/accel_data_y.h"

void icm42688_reg_get_accel_data_y(icm42688_reg_accel_data_y_t* accel_data_y) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    icm42688_read_reg(ICM42688_REG_ACCEL_DATA_Y0, &accel_data_y->accel_data_y_l);
    icm42688_read_reg(ICM42688_REG_ACCEL_DATA_Y1, &accel_data_y->accel_data_y_h);
}
