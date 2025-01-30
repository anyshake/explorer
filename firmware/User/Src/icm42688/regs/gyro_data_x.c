#include "User/Inc/icm42688/regs/gyro_data_x.h"

void icm42688_reg_get_gyro_data_x(icm42688_reg_gyro_data_x_t* gyro_data_x) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    icm42688_read_reg(ICM42688_REG_GYRO_DATA_X0, &gyro_data_x->gyro_data_x_l);
    icm42688_read_reg(ICM42688_REG_GYRO_DATA_X1, &gyro_data_x->gyro_data_x_h);
}
