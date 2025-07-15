#include "User/Inc/ssd1306/display.h"

void ssd1306_set_position(uint8_t x, uint8_t y) {
    ssd1306_write_cmd(0xb0 + y);
    ssd1306_write_cmd(((x & 0xf0) >> 4) | 0x10);
    ssd1306_write_cmd(x & 0x0f);
}

void ssd1306_display_char(uint8_t x,
                          uint8_t y,
                          uint8_t ch,
                          uint8_t font,
                          uint8_t color) {
    uint8_t offset = ch - ' ';
    ssd1306_set_position(x, y);
    if (font == SSD1306_FONT_TYPE_ASCII_8X16) {
        for (uint8_t i = 0; i < 8; i++) {
            ssd1306_write_data(color == SSD1306_FONT_DISPLAY_COLOR_WHITE ? SSD1306_FONT_TYPE_ASCII_8X16_LIB[offset][i] : ~SSD1306_FONT_TYPE_ASCII_8X16_LIB[offset][i]);
        }
        ssd1306_set_position(x, y + 1);
        for (uint8_t i = 0; i < 8; i++) {
            ssd1306_write_data(color == SSD1306_FONT_DISPLAY_COLOR_WHITE ? SSD1306_FONT_TYPE_ASCII_8X16_LIB[offset][i + 8] : ~SSD1306_FONT_TYPE_ASCII_8X16_LIB[offset][i + 8]);
        }
    } else if (font == SSD1306_FONT_TYPE_ASCII_8X6) {
        for (uint8_t i = 0; i < 6; i++) {
            ssd1306_write_data(color == SSD1306_FONT_DISPLAY_COLOR_WHITE ? SSD1306_FONT_TYPE_ASCII_8X6_LIB[offset][i] : ~SSD1306_FONT_TYPE_ASCII_8X6_LIB[offset][i]);
        }
    }
}

void ssd1306_display_string(uint8_t x,
                            uint8_t y,
                            const char* str,
                            uint8_t font,
                            uint8_t color,
                            bool clear) {
        uint8_t char_width = (font == SSD1306_FONT_TYPE_ASCII_8X16) ? 8 : 6;
    uint8_t char_height = (font == SSD1306_FONT_TYPE_ASCII_8X16) ? 2 : 1;

    if (clear) {
        for (uint8_t col = 0; col < 128; col += char_width) {
            ssd1306_display_char(col, y, ' ', font, SSD1306_FONT_DISPLAY_COLOR_WHITE);
        }
    }

    uint8_t i = 0;
    while (str[i] != '\0') {
        ssd1306_display_char(x, y, str[i], font, color);
        x += char_width;
        if (x > 128 - char_width) {
            x = 0;
            y += char_height;
        }
        i++;
    }
}

void ssd1306_display_bitmap(uint8_t start_x,
                            uint8_t start_y,
                            uint8_t width,
                            uint8_t height,
                            const uint8_t* bitmap_rle,
                            uint8_t color) {
    uint16_t total_bytes = width * height;
    uint16_t rendered = 0;
    uint16_t i = 0;

    uint8_t y = 0;
    uint8_t x = 0;

    while (rendered < total_bytes) {
        uint8_t count = bitmap_rle[i++];
        uint8_t value = bitmap_rle[i++];

        uint8_t final_value = (color == SSD1306_FONT_DISPLAY_COLOR_WHITE) ? value : ~value;

        for (uint8_t j = 0; j < count; j++) {
            ssd1306_set_position(start_x + x, start_y + y);
            ssd1306_write_data(final_value);
            rendered++;

            x++;
            if (x >= width) {
                x = 0;
                y++;
                if (y >= height) {
                    return;
                }
            }
        }
    }
}
