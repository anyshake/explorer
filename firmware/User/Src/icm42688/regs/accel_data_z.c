#include "User/Inc/icm42688/regs/accel_data_z.h"

void icm42688_reg_get_accel_data_z(icm42688_reg_accel_data_z_t* accel_data_z) {
    uint8_t data[2] = {0};
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0x00);
    icm42688_read_regs(ICM42688_REG_ACCEL_DATA_Z1, data, sizeof(data));
    accel_data_z->accel_data_z_l = data[1];
    accel_data_z->accel_data_z_h = data[0];
}
