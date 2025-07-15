#include "User/Inc/ssd1306/utils.h"

void ssd1306_init(void) {
    ssd1306_write_cmd(0xAE);
    ssd1306_write_cmd(0x00);
    ssd1306_write_cmd(0x10);
    ssd1306_write_cmd(0x40);
    ssd1306_write_cmd(0xB0);
    ssd1306_write_cmd(0x81);
    ssd1306_write_cmd(0xFF);
    ssd1306_write_cmd(0xA1);
    ssd1306_write_cmd(0xA6);
    ssd1306_write_cmd(0xA8);
    ssd1306_write_cmd(0x3F);
    ssd1306_write_cmd(0xC8);
    ssd1306_write_cmd(0xD3);
    ssd1306_write_cmd(0x00);
    ssd1306_write_cmd(0xD5);
    ssd1306_write_cmd(0x80);
    ssd1306_write_cmd(0xD8);
    ssd1306_write_cmd(0x05);
    ssd1306_write_cmd(0xD9);
    ssd1306_write_cmd(0xF1);
    ssd1306_write_cmd(0xDA);
    ssd1306_write_cmd(0x12);
    ssd1306_write_cmd(0xDB);
    ssd1306_write_cmd(0x30);
    ssd1306_write_cmd(0x8D);
    ssd1306_write_cmd(0x14);
    ssd1306_write_cmd(0xAF);
}

void ssd1306_enable(void) {
    ssd1306_write_cmd(0x8D);
    ssd1306_write_cmd(0x14);
    ssd1306_write_cmd(0xAF);
}

void ssd1306_disable(void) {
    ssd1306_write_cmd(0x8D);
    ssd1306_write_cmd(0x10);
    ssd1306_write_cmd(0xAE);
}

void ssd1306_clear(void) {
    for (uint8_t i = 0; i < 8; i++) {
        ssd1306_write_cmd(0xb0 + i);
        ssd1306_write_cmd(0x01);
        ssd1306_write_cmd(0x10);
        for (uint8_t j = 0; j < 128; j++) {
            ssd1306_write_data(0x00);
        }
    }
}

void ssd1306_set_brightness(uint8_t brightness) {
    ssd1306_write_cmd(0x81);
    ssd1306_write_cmd(brightness);
}

void ssd1306_write_cmd(uint8_t tx_data) {
    mcu_utils_i2c_write(SSD1306_I2C_ADDRESS, SSD1306_REG_TYPE_CMD, &tx_data, sizeof(tx_data), MCU_UTILS_I2C_REG_WIDTH_8);
}

void ssd1306_write_data(uint8_t tx_data) {
    mcu_utils_i2c_write(SSD1306_I2C_ADDRESS, SSD1306_REG_TYPE_DATA, &tx_data, sizeof(tx_data), MCU_UTILS_I2C_REG_WIDTH_8);
}
