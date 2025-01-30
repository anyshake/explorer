#include "User/Inc/icm42688/regs/int_config1.h"

icm42688_reg_int_config1_t icm42688_reg_new_int_config1(void) {
    icm42688_reg_int_config1_t int_config1;
    int_config1.int_tpulse_duration = ICM42688_REG_INT_CONFIG1_INT_TPULSE_DURATION_100US;
    int_config1.int_tdeassert_disable = ICM42688_REG_INT_CONFIG1_INT_TDEASSERT_DISABLE_MINIMUM;
    int_config1.int_async_reset = ICM42688_REG_INT_CONFIG1_INT_ASYNC_RESET_DISABLED;
    return int_config1;
}

void icm42688_reg_set_int_config1(icm42688_reg_int_config1_t* int_config1) {
    uint8_t int_config1_data = __ICM42688_REG_INT_CONFIG1_DEFAULT_VALUE;
    int_config1_data &= ~(0x01 << 6);
    int_config1_data |= (int_config1->int_tpulse_duration & 0x01) << 6;
    int_config1_data &= ~(0x01 << 5);
    int_config1_data |= (int_config1->int_tdeassert_disable & 0x01) << 5;
    int_config1_data &= ~(0x01 << 4);
    int_config1_data |= (int_config1->int_async_reset & 0x01) << 4;
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    icm42688_write_reg(ICM42688_REG_INT_CONFIG1, int_config1_data);
}

void icm42688_reg_get_int_config1(icm42688_reg_int_config1_t* int_config1) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    uint8_t int_config1_data = 0;
    icm42688_read_reg(ICM42688_REG_INT_CONFIG1, &int_config1_data);
    int_config1->int_tpulse_duration = (int_config1_data >> 6) & 0x01;
    int_config1->int_tdeassert_disable = (int_config1_data >> 5) & 0x01;
    int_config1->int_async_reset = (int_config1_data >> 4) & 0x01;
}
