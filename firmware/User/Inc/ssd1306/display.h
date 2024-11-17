#ifndef __SSD1306_DISPLAY_H
#define __SSD1306_DISPLAY_H

#include <stdint.h>

#include "User/Inc/ssd1306/font.h"
#include "User/Inc/ssd1306/utils.h"

void ssd1306_set_position(uint8_t x, uint8_t y);
void ssd1306_display_char(uint8_t x,
                          uint8_t y,
                          uint8_t ch,
                          uint8_t font,
                          uint8_t color);
void ssd1306_display_string(uint8_t x,
                            uint8_t y,
                            const char* str,
                            uint8_t font,
                            uint8_t color);
void ssd1306_display_bitmap(uint8_t start_x,
                            uint8_t start_y,
                            uint8_t width,
                            uint8_t height,
                            const uint8_t bitmap[],
                            uint8_t color);

#endif
