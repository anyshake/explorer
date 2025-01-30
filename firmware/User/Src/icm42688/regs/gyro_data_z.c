#include "User/Inc/icm42688/regs/gyro_data_z.h"

void icm42688_reg_get_gyro_data_z(icm42688_reg_gyro_data_z_t* gyro_data_z) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    icm42688_read_reg(ICM42688_REG_GYRO_DATA_Z0, &gyro_data_z->gyro_data_z_l);
    icm42688_read_reg(ICM42688_REG_GYRO_DATA_Z1, &gyro_data_z->gyro_data_z_h);
}
