#ifndef __LSM6DS3TR_C_UTILS_H
#define __LSM6DS3TR_C_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#include "User/Inc/lsm6ds3tr_c/regs/ctrl3_c.h"
#include "User/Inc/lsm6ds3tr_c/regs/int1_ctrl.h"
#include "User/Inc/lsm6ds3tr_c/regs/status_reg.h"
#include "Utils/Inc/delay.h"
#include "Utils/Inc/gpio.h"
#include "Utils/Inc/i2c1.h"

#define LSM6DS3TR_C_I2C_ADDRESS 0x6A

typedef struct {
    mcu_utils_gpio_t int_1;
    mcu_utils_gpio_t int_2;
} lsm6ds3tr_c_ints_pin_t;

void lsm6ds3tr_c_wait(void);
void lsm6ds3tr_c_init(lsm6ds3tr_c_ints_pin_t pin);
void lsm6ds3tr_c_reset(bool is_rtos);
void lsm6ds3tr_c_read_reg(uint8_t reg, uint8_t* rx_data);
// Multi-byte access requires CTRL3_C.IF_INC = 1.
void lsm6ds3tr_c_read_regs(uint8_t reg, uint8_t* rx_data, uint16_t rx_len);
void lsm6ds3tr_c_write_reg(uint8_t reg, uint8_t tx_data);

#endif
