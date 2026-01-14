#include "User/Inc/mode.h"

void mode_entry_gnss_debug(explorer_global_states_t* states) {
    ssd1306_clear();
    ssd1306_display_string(22, 0, "GNSS Debug", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    ssd1306_display_string(14, 7, "- anyshake.org -", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);

    ssd1306_display_string(15, 3, "Output Baudrate", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    snprintf((char*)states->message_buf, sizeof(states->message_buf), "%6lu bps", states->baud_rate);
    ssd1306_display_string(32, 4, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);

    while (1) {
        if (mcu_utils_uart2_hasdata()) {
            uint8_t ch = mcu_utils_uart2_read();
            mcu_utils_uart_write(&ch, sizeof(ch), true);
        }
    }
}

void mode_entry_leveling(explorer_global_states_t* states) {
    ssd1306_clear();
    ssd1306_display_string(15, 0, "Inclinometer", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    ssd1306_display_string(14, 7, "- anyshake.org -", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);

    int16_t result_arr[3];
    float temperature = 0;
    while (1) {
        get_accel_readout(0, result_arr);
        get_env_temperature(&temperature);

        float readout_x = result_arr[1];
        float readout_y = result_arr[2];
        float readout_z = result_arr[0];
        float x_angle = quick_atan2(-readout_x, quick_sqrt(readout_y * readout_y + readout_z * readout_z)) * 180.0 / MAGIC_PI;
        float y_angle = quick_atan2(readout_y, readout_z) * 180.0 / MAGIC_PI;

        x_angle = -x_angle;

        snprintf((char*)states->message_buf, sizeof(states->message_buf), "TMP: %5.2f * C", temperature);
        ssd1306_display_string(20, 3, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
        snprintf((char*)states->message_buf, sizeof(states->message_buf), "X: %7.2f deg", x_angle);
        ssd1306_display_string(20, 4, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
        snprintf((char*)states->message_buf, sizeof(states->message_buf), "Y: %7.2f deg", y_angle);
        ssd1306_display_string(20, 5, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);

        snprintf((char*)states->message_buf, sizeof(states->message_buf), "Raw X: %d; Raw Y: %d; Raw Z: %d\r\n", result_arr[1], result_arr[2], result_arr[0]);
        mcu_utils_uart_write((uint8_t*)states->message_buf, strlen((char*)states->message_buf), false);

        mcu_utils_delay_ms(100, false);
    }
}
