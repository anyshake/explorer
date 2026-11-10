#include "User/Inc/icm42688/regs/gyro_data_y.h"

void icm42688_reg_get_gyro_data_y(icm42688_reg_gyro_data_y_t* gyro_data_y) {
    uint8_t data[2] = {0};
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0x00);
    icm42688_read_regs(ICM42688_REG_GYRO_DATA_Y1, data, sizeof(data));
    gyro_data_y->gyro_data_y_l = data[1];
    gyro_data_y->gyro_data_y_h = data[0];
}
