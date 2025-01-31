#ifndef __LSM6DS3_UTILS_H
#define __LSM6DS3_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#include "User/Inc/lsm6ds3/regs/ctrl3_c.h"
#include "Utils/Inc/delay.h"
#include "Utils/Inc/gpio.h"
#include "Utils/Inc/i2c.h"

#define LSM6DS3_I2C_ADDRES 0x6A

typedef struct {
    mcu_utils_gpio_t int_1;
    mcu_utils_gpio_t int_2;
} lsm6ds3_ints_pin_t;

void lsm6ds3_wait(lsm6ds3_ints_pin_t pin);
void lsm6ds3_init(lsm6ds3_ints_pin_t pin);
void lsm6ds3_reset(bool is_rtos);
void lsm6ds3_read_reg(uint8_t reg, uint8_t* rx_data);
void lsm6ds3_write_reg(uint8_t reg, uint8_t tx_data);

#endif
