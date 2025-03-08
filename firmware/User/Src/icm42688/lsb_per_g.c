#include "User/Inc/icm42688/lsb_per_g.h"

uint16_t icm42688_get_lsb_per_g(void) {
    const uint16_t LSB_PER_G_TABLE[] = {2048, 4096, 8192, 16384};
    icm42688_reg_accel_config0_t icm42688_reg_accel_config0;
    icm42688_reg_get_accel_config0(&icm42688_reg_accel_config0);
    return LSB_PER_G_TABLE[icm42688_reg_accel_config0.accel_fs_sel];
}
