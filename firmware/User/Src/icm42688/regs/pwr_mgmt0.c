#include "User/Inc/icm42688/regs/pwr_mgmt0.h"

icm42688_reg_pwr_mgmt0_t icm42688_reg_new_pwr_mgmt0(void) {
    icm42688_reg_pwr_mgmt0_t pwr_mgmt0;
    pwr_mgmt0.temp_dis = ICM42688_REG_PWR_MGMT0_TEMP_DIS_ENABLED;
    pwr_mgmt0.idle = ICM42688_REG_PWR_MGMT0_IDLE_ENABLED;
    pwr_mgmt0.gyro_mode = ICM42688_REG_PWR_MGMT0_GYRO_MODE_OFF;
    pwr_mgmt0.accel_mode = ICM42688_REG_PWR_MGMT0_ACCEL_MODE_OFF;
    return pwr_mgmt0;
}

void icm42688_reg_set_pwr_mgmt0(icm42688_reg_pwr_mgmt0_t* pwr_mgmt0) {
    uint8_t pwr_mgmt0_data = __ICM42688_REG_PWR_MGMT0_DEFAULT_VALUE;
    pwr_mgmt0_data &= ~(0x01 << 5);
    pwr_mgmt0_data |= (pwr_mgmt0->temp_dis & 0x01) << 5;
    pwr_mgmt0_data &= ~(0x01 << 4);
    pwr_mgmt0_data |= (pwr_mgmt0->idle & 0x01) << 4;
    pwr_mgmt0_data &= ~(0x03 << 2);
    pwr_mgmt0_data |= (pwr_mgmt0->gyro_mode & 0x03) << 2;
    pwr_mgmt0_data &= ~0x03;
    pwr_mgmt0_data |= (pwr_mgmt0->accel_mode & 0x03);
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    icm42688_write_reg(ICM42688_REG_PWR_MGMT0, pwr_mgmt0_data);
}

void icm42688_reg_get_pwr_mgmt0(icm42688_reg_pwr_mgmt0_t* pwr_mgmt0) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    uint8_t pwr_mgmt0_data = 0;
    icm42688_read_reg(ICM42688_REG_PWR_MGMT0, &pwr_mgmt0_data);
    pwr_mgmt0->temp_dis = (pwr_mgmt0_data >> 5) & 0x01;
    pwr_mgmt0->idle = (pwr_mgmt0_data >> 4) & 0x01;
    pwr_mgmt0->gyro_mode = (pwr_mgmt0_data >> 2) & 0x03;
    pwr_mgmt0->accel_mode = pwr_mgmt0_data & 0x03;
}
