#include "User/Inc/utils.h"

void read_device_settings(explorer_global_states_t* states) {
    mcu_utils_gpio_mode(MCU_BOOT1_PIN, MCU_UTILS_GPIO_MODE_INPUT);
    states->leveling_mode = mcu_utils_gpio_read(MCU_BOOT1_PIN);

    mcu_utils_gpio_mode(SAMPLERATE_SELECT_P1, MCU_UTILS_GPIO_MODE_INPUT);
    mcu_utils_gpio_mode(SAMPLERATE_SELECT_P2, MCU_UTILS_GPIO_MODE_INPUT);
    switch (mcu_utils_gpio_read(SAMPLERATE_SELECT_P1) << 1 | mcu_utils_gpio_read(SAMPLERATE_SELECT_P2)) {
        case 0:
            states->sample_rate = 250;
            break;
        case 1:
            states->sample_rate = 200;
            break;
        case 2:
            states->sample_rate = 100;
            break;
        case 3:
            states->sample_rate = 50;
            break;
    }

    mcu_utils_gpio_mode(BAUDRATE_SELECT_P1, MCU_UTILS_GPIO_MODE_INPUT);
    mcu_utils_gpio_mode(BAUDRATE_SELECT_P2, MCU_UTILS_GPIO_MODE_INPUT);
    switch (mcu_utils_gpio_read(BAUDRATE_SELECT_P1) << 1 | mcu_utils_gpio_read(BAUDRATE_SELECT_P2)) {
        case 0:
            states->baud_rate = 57600;
            break;
        case 1:
            states->baud_rate = 115200;
            break;
        case 2:
            states->baud_rate = 230400;
            break;
        case 3:
            states->baud_rate = 460800;
            break;
    }

    mcu_utils_gpio_mode(OPTIONS_USE_ACCELEROMETER_PIN, MCU_UTILS_GPIO_MODE_INPUT);
    states->use_accelerometer = mcu_utils_gpio_read(OPTIONS_USE_ACCELEROMETER_PIN);

    mcu_utils_gpio_mode(OPTIONS_USE_GNSS_PIN, MCU_UTILS_GPIO_MODE_INPUT);
    states->use_gnss_time = mcu_utils_gpio_read(OPTIONS_USE_GNSS_PIN);

    mcu_utils_gpio_mode(OPTIONS_CHANNEL_6D_PIN, MCU_UTILS_GPIO_MODE_INPUT);
    states->channel_6d = mcu_utils_gpio_read(OPTIONS_CHANNEL_6D_PIN);
}

void display_device_settings(explorer_global_states_t* states) {
    snprintf((char*)states->message_buf, sizeof(states->message_buf), "SAMPLE RATE: %3hhu Hz", states->sample_rate);
    ssd1306_display_string(0, 0, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    snprintf((char*)states->message_buf, sizeof(states->message_buf), "PORT BR: %6lu bps", states->baud_rate);
    ssd1306_display_string(0, 1, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    snprintf((char*)states->message_buf, sizeof(states->message_buf), "ACCELERO ENABLE: %2s", states->use_accelerometer || states->channel_6d ? "Y" : "N");
    ssd1306_display_string(0, 2, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    snprintf((char*)states->message_buf, sizeof(states->message_buf), "GPS DATA ENABLE: %2s", states->use_gnss_time ? "Y" : "N");
    ssd1306_display_string(0, 3, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    snprintf((char*)states->message_buf, sizeof(states->message_buf), "6D CHANNEL MODE: %2s", states->channel_6d ? "Y" : "N");
    ssd1306_display_string(0, 4, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    snprintf((char*)states->message_buf, sizeof(states->message_buf), "FW REV: %s", FW_REV);
    ssd1306_display_string(0, 5, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    snprintf((char*)states->message_buf, sizeof(states->message_buf), "BUILD: %s", FW_BUILD);
    ssd1306_display_string(0, 6, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    ssd1306_display_string(14, 7, "- anyshake.org -", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);

    ssd1306_set_brightness(1);  // Set minimum brightness
}

bool parse_gnss_message(uint8_t* message_buf, gnss_status_t* gnss_status, gnss_location_t* gnss_location, gnss_time_t* gnss_time, int64_t local_timestamp, int64_t* gnss_time_diff) {
    bool got_gga = false;
    bool got_rmc = false;

    for (uint8_t attempt = 0; !got_gga || !got_rmc; attempt++) {
        uint8_t ret = gnss_get_sentence(message_buf, 1000);
        if (ret != 0) {
            return false;
        }
        gnss_padding_sentence(message_buf);

        if (!got_gga && gnss_match_keyword(message_buf, GNSS_SENTENCE_TYPE_GGA)) {
            gnss_parse_gga(message_buf, gnss_status, gnss_location);
            got_gga = true;
        } else if (!got_rmc && gnss_match_keyword(message_buf, GNSS_SENTENCE_TYPE_RMC)) {
            gnss_parse_rmc(message_buf, gnss_location, gnss_time);
            int64_t gnss_message_timestamp = gnss_get_timestamp(gnss_time);
            if (GNSS_ROUND_TIMESTAMP) {
                gnss_message_timestamp = (gnss_message_timestamp + 500) / 1000 * 1000;
            }
            *gnss_time_diff = gnss_message_timestamp - local_timestamp;
            got_rmc = true;
        }
    }

    return got_gga && got_rmc;
}

uint32_t get_tim3_clk_freq(void) {
    uint32_t pclk2 = HAL_RCC_GetPCLK2Freq();
    uint32_t ppre2 = (RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos;

    if (ppre2 >= 4) {
        return pclk2 * 2;
    }
    return pclk2;
}
