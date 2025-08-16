#include "User/Inc/utils.h"

void read_device_settings(explorer_global_states_t* states) {
    mcu_utils_gpio_mode(SAMPLERATE_SELECT_P1, MCU_UTILS_GPIO_MODE_INPUT);
    mcu_utils_gpio_mode(SAMPLERATE_SELECT_P2, MCU_UTILS_GPIO_MODE_INPUT);
    switch (mcu_utils_gpio_read(SAMPLERATE_SELECT_P1) << 1 | mcu_utils_gpio_read(SAMPLERATE_SELECT_P2)) {
        case 0:
            states->sample_rate = 250;
#if HARDWARE_REV >= 20250804
            filter_iir_df1_new(&states->df1_filter_ch1, IIR_DF1_B_COEFFS_250_HZ, IIR_DF1_A_COEFFS_250_HZ);
            filter_iir_df1_new(&states->df1_filter_ch2, IIR_DF1_B_COEFFS_250_HZ, IIR_DF1_A_COEFFS_250_HZ);
            filter_iir_df1_new(&states->df1_filter_ch3, IIR_DF1_B_COEFFS_250_HZ, IIR_DF1_A_COEFFS_250_HZ);
#endif
            break;
        case 1:
            states->sample_rate = 200;
#if HARDWARE_REV >= 20250804
            filter_iir_df1_new(&states->df1_filter_ch1, IIR_DF1_B_COEFFS_200_HZ, IIR_DF1_A_COEFFS_200_HZ);
            filter_iir_df1_new(&states->df1_filter_ch2, IIR_DF1_B_COEFFS_200_HZ, IIR_DF1_A_COEFFS_200_HZ);
            filter_iir_df1_new(&states->df1_filter_ch3, IIR_DF1_B_COEFFS_200_HZ, IIR_DF1_A_COEFFS_200_HZ);
#endif
            break;
        case 2:
            states->sample_rate = 100;
#if HARDWARE_REV >= 20250804
            filter_iir_df1_new(&states->df1_filter_ch1, IIR_DF1_B_COEFFS_100_HZ, IIR_DF1_A_COEFFS_100_HZ);
            filter_iir_df1_new(&states->df1_filter_ch2, IIR_DF1_B_COEFFS_100_HZ, IIR_DF1_A_COEFFS_100_HZ);
            filter_iir_df1_new(&states->df1_filter_ch3, IIR_DF1_B_COEFFS_100_HZ, IIR_DF1_A_COEFFS_100_HZ);
#endif
            break;
        case 3:
#if HARDWARE_REV >= 20250804
            filter_iir_df1_new(&states->df1_filter_ch1, IIR_DF1_B_COEFFS_50_HZ, IIR_DF1_A_COEFFS_50_HZ);
            filter_iir_df1_new(&states->df1_filter_ch2, IIR_DF1_B_COEFFS_50_HZ, IIR_DF1_A_COEFFS_50_HZ);
            filter_iir_df1_new(&states->df1_filter_ch3, IIR_DF1_B_COEFFS_50_HZ, IIR_DF1_A_COEFFS_50_HZ);
#endif
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
    uint8_t use_accelerometer_bit = mcu_utils_gpio_read(OPTIONS_USE_ACCELEROMETER_PIN);
    states->use_accelerometer = use_accelerometer_bit == 1 ? true : false;

    mcu_utils_gpio_mode(OPTIONS_USE_GNSS_PIN, MCU_UTILS_GPIO_MODE_INPUT);
    uint8_t use_gnss_bit = mcu_utils_gpio_read(OPTIONS_USE_GNSS_PIN);
    states->use_gnss_time = use_gnss_bit == 1 ? true : false;

    mcu_utils_gpio_mode(OPTIONS_CHANNEL_6D_PIN, MCU_UTILS_GPIO_MODE_INPUT);
    uint8_t channel_6d_bit = mcu_utils_gpio_read(OPTIONS_CHANNEL_6D_PIN);
    states->channel_6d = channel_6d_bit == 1 ? true : false;

    mcu_utils_gpio_mode(MCU_BOOT1_PIN, MCU_UTILS_GPIO_MODE_INPUT);
    uint8_t boot1_enabled = mcu_utils_gpio_read(MCU_BOOT1_PIN);
    uint8_t options_bits = use_accelerometer_bit << 2 | use_gnss_bit << 1 | channel_6d_bit;
    states->leveling_mode = boot1_enabled && (options_bits == 0);
    states->gnss_debug_mode = boot1_enabled && (options_bits == 1);
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

bool parse_gnss_message(uint8_t* message_buf, gnss_status_t* gnss_status, gnss_location_t* gnss_location, gnss_time_t* gnss_time, int64_t local_timestamp, int64_t* gnss_time_diff, bool is_rtos) {
    bool got_gga = false;
    bool got_rmc = false;

    gnss_time_t rmc_time;
    gnss_time_t gga_time;

    while (!got_gga || !got_rmc) {
        bool ok = gnss_get_sentence(message_buf, 990, is_rtos);
        if (!ok) {
            return false;
        }
        gnss_padding_sentence(message_buf);

        if (!got_gga && gnss_match_keyword(message_buf, GNSS_SENTENCE_TYPE_GGA)) {
            gnss_parse_gga(message_buf, gnss_status, gnss_location, &gga_time);
            got_gga = true;
        } else if (!got_rmc && gnss_match_keyword(message_buf, GNSS_SENTENCE_TYPE_RMC)) {
            gnss_parse_rmc(message_buf, gnss_location, &rmc_time);
            got_rmc = true;
        }

        if (got_gga && got_rmc && gga_time.hour == rmc_time.hour && gga_time.minute == rmc_time.minute && gga_time.second == rmc_time.second && gga_time.milisecond == rmc_time.milisecond) {
            if (gnss_time != NULL && gnss_time_diff != NULL) {
                int64_t gnss_message_timestamp = gnss_get_timestamp(&rmc_time);
                // some GNSS module such as Quectel LC260Z and Quectel LC761Z have a wrong timestamp
                // with an offset of 1 second, already confirmed by Quectel support team
                gnss_message_timestamp = gnss_model_handle_timestamp(gnss_message_timestamp);
                *gnss_time_diff = gnss_message_timestamp - local_timestamp;
                *gnss_time = rmc_time;
            }
            return true;
        } else if (got_gga && got_rmc) {
            got_gga = false;
            got_rmc = false;
            mcu_utils_uart2_flush();
        }
    }

    return false;
}

uint32_t get_tim3_clk_freq(void) {
    uint32_t pclk2 = HAL_RCC_GetPCLK2Freq();
    uint32_t ppre2 = (RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos;

    if (ppre2 >= 4) {
        return pclk2 * 2;
    }
    return pclk2;
}

float get_adjust_step_size(float current_ppm, float avg_ppm, float tick_step_us, uint32_t delta_us) {
    const float tick_step_min = 4.0f;
    const float tick_step_max = 25.0f;
    const float slope = 10.0f;  // aggressiveness of adjustment

    float smoothed_ppm = 0.7f * current_ppm + 0.3f * avg_ppm;
    float tick_estimate = (smoothed_ppm > 0.0f ? smoothed_ppm : -smoothed_ppm) * delta_us / 1e6f / tick_step_us;
    float tick_step = slope * tick_estimate;

    if (tick_step < tick_step_min) {
        tick_step = tick_step_min;
    }
    if (tick_step > tick_step_max) {
        tick_step = tick_step_max;
    }

    return tick_step;
}
