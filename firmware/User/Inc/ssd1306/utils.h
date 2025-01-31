#ifndef __SSD1306_UTILS_H
#define __SSD1306_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#include "Utils/Inc/delay.h"
#include "Utils/Inc/i2c.h"

#define SSD1306_I2C_ADDRESS 0x3C

#define SSD1306_REG_TYPE_CMD 0x00
#define SSD1306_REG_TYPE_DATA 0x40

void ssd1306_init(void);
void ssd1306_enable(void);
void ssd1306_disable(void);
void ssd1306_clear(void);
void ssd1306_write_cmd(uint8_t dat);
void ssd1306_write_data(uint8_t dat);

#endif
