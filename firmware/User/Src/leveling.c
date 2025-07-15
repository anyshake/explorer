#include "User/Inc/leveling.h"

void leveling_mode_entry(void) {
    ssd1306_clear();
    ssd1306_display_string(15, 0, "Inclinometer", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    ssd1306_display_string(14, 7, "- anyshake.org -", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);

    int16_t result_arr[3];
    char display_buf[19];
    float temperature = 0;
    while (1) {
        get_accel_readout(0, result_arr);
        get_env_temperature(&temperature);

        int16_t acc_x = result_arr[1];
        int16_t acc_y = result_arr[2];
        int16_t acc_z = result_arr[0];
        float x_angle = quick_atan2(-acc_x, quick_sqrt(acc_y * acc_y + acc_z * acc_z)) * 180.0 / MAGIC_PI;
        float y_angle = quick_atan2(acc_y, acc_z) * 180.0 / MAGIC_PI;

        x_angle = -x_angle;

        snprintf(display_buf, sizeof(display_buf), "TMP: %5.2f * C", temperature);
        ssd1306_display_string(20, 3, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
        snprintf(display_buf, sizeof(display_buf), "X: %7.2f deg", x_angle);
        ssd1306_display_string(20, 4, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
        snprintf(display_buf, sizeof(display_buf), "Y: %7.2f deg", y_angle);
        ssd1306_display_string(20, 5, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);

        mcu_utils_delay_ms(100, false);
    }
}
