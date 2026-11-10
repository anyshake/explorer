#include "User/Inc/icm42688/lsb_per_g.h"

uint16_t icm42688_get_lsb_per_g(void) {
    const uint16_t lsb_per_g_table[] = {2048, 4096, 8192, 16384};
    icm42688_reg_accel_config0_t icm42688_reg_accel_config0;
    icm42688_reg_get_accel_config0(&icm42688_reg_accel_config0);
    // ACCEL_FS_SEL[2:0] values 0x04 through 0x07 are reserved.
    if (icm42688_reg_accel_config0.accel_fs_sel >= sizeof(lsb_per_g_table) / sizeof(lsb_per_g_table[0])) {
        return 0;
    }
    return lsb_per_g_table[icm42688_reg_accel_config0.accel_fs_sel];
}
