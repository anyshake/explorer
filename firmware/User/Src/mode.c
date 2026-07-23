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
            mcu_utils_uart1_write(&ch, sizeof(ch), true);
        }
    }
}

void mode_entry_leveling(explorer_global_states_t* states) {
    ssd1306_clear();
    ssd1306_display_string(15, 0, "Inclinometer", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    ssd1306_display_string(14, 7, "- anyshake.org -", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);

    int16_t result_arr[3];
    float temperature = 0.0f;
    while (1) {
        get_accel_readout(0, result_arr);
        get_env_temperature(&temperature);

        float readout_x = result_arr[1];
        float readout_y = result_arr[2];
        float readout_z = result_arr[0];
        float x_angle = quick_atan2(-readout_x, quick_sqrt(readout_y * readout_y + readout_z * readout_z)) * 180.0f / MAGIC_PI;
        float y_angle = quick_atan2(readout_y, readout_z) * 180.0f / MAGIC_PI;

        x_angle = -x_angle;

        char formatted_value[16];
        format_float_fixed(formatted_value, sizeof(formatted_value), temperature, 2);
        snprintf((char*)states->message_buf, sizeof(states->message_buf), "TMP: %5s * C", formatted_value);
        ssd1306_display_string(20, 3, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
        format_float_fixed(formatted_value, sizeof(formatted_value), x_angle, 2);
        snprintf((char*)states->message_buf, sizeof(states->message_buf), "X: %7s deg", formatted_value);
        ssd1306_display_string(20, 4, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
        format_float_fixed(formatted_value, sizeof(formatted_value), y_angle, 2);
        snprintf((char*)states->message_buf, sizeof(states->message_buf), "Y: %7s deg", formatted_value);
        ssd1306_display_string(20, 5, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);

        snprintf((char*)states->message_buf, sizeof(states->message_buf), "Raw X: %d; Raw Y: %d; Raw Z: %d\r\n", result_arr[1], result_arr[2], result_arr[0]);
        mcu_utils_uart1_write((uint8_t*)states->message_buf, strlen((char*)states->message_buf), false);

        mcu_utils_delay_ms(100, false);
    }
}

static void adc_calibration_wait_for_pin(mcu_utils_gpio_t pin, uint8_t level) {
    while (mcu_utils_gpio_read(pin) != level) {
        ;
    }

    mcu_utils_led_blink(MCU_STATE_PIN, 5, false);
    mcu_utils_delay_ms(500, false);
}

static void adc_calibration_read_offset(uint8_t mux_p, uint8_t mux_n, uint8_t offset[3]) {
    ads1262_reg_ofcal_t ofcal = ads1262_reg_new_ofcal();
    ofcal.ofcal_0 = 0;
    ofcal.ofcal_1 = 0;
    ofcal.ofcal_2 = 0;
    ads1262_reg_set_ofcal(&ofcal);

    ads1262_reg_inpmux_t inpmux = ads1262_reg_new_inpmux();
    inpmux.mux_p = mux_p;
    inpmux.mux_n = mux_n;
    ads1262_reg_set_inpmux(&inpmux);

    ads1262_cmd_start(ADS1262_CTL_PIN);

    float round_averages[50] = {0};
    for (uint8_t round = 0; round < 50; round++) {
        float sum = 0.0f;
        for (uint8_t sample = 0; sample < 100; sample++) {
            ads1262_cmd_rdata_t rdata;
            ads1262_wait(ADS1262_CTL_PIN);
            ads1262_cmd_rdata(ADS1262_CTL_PIN, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
            sum += rdata.data;
        }
        round_averages[round] = sum / 100.0f;
    }

    ads1262_cmd_stop(ADS1262_CTL_PIN);

    float sum = 0.0f;
    for (uint8_t round = 0; round < 50; round++) {
        sum += round_averages[round];
    }
    float average = sum / 50.0f;

    int32_t rounded_average = (int32_t)((average >= 0.0f) ? (average + 250.0f) : (average - 250.0f));
    rounded_average = (rounded_average / 500) * 500;
    uint32_t ofcal_value = ((uint32_t)rounded_average >> 8) & 0xFFFFFF;

    offset[0] = ofcal_value & 0xFF;
    offset[1] = (ofcal_value >> 8) & 0xFF;
    offset[2] = (ofcal_value >> 16) & 0xFF;
}

static void adc_calibration_show_channel(uint8_t step, const char* channel) {
    char title[] = "Step 3/6:";
    title[5] = '0' + step;

    char message[] = "Calibrating EHZ...";
    message[12] = channel[0];
    message[13] = channel[1];
    message[14] = channel[2];

    ssd1306_clear();
    ssd1306_display_string(0, 0, title, SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    ssd1306_display_string(0, 4, "DO NOT TOUCH DEVICE", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    ssd1306_display_string(0, 5, message, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
}

static void adc_calibration_wait_for_channel_switch(mcu_utils_gpio_t pin, uint8_t switch_number) {
    char message[] = "Set OPTIONS B1 to 0";
    message[13] = '0' + switch_number;

    ssd1306_clear();
    ssd1306_display_string(0, 0, "INFO:", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    ssd1306_display_string(0, 4, "Complete in 10 sec!", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    ssd1306_display_string(0, 5, message, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    adc_calibration_wait_for_pin(pin, 0);
}

static void adc_calibration_write_data(adc_calibration_data_t* calibration_data) {
    eeprom_write(EEPROM_WP_PIN, calibration_data->channel_1, EEPROM_OFFSET_OFCAL_CHANNEL_1, sizeof(calibration_data->channel_1));
    mcu_utils_delay_ms(500, false);
    eeprom_write(EEPROM_WP_PIN, calibration_data->channel_2, EEPROM_OFFSET_OFCAL_CHANNEL_2, sizeof(calibration_data->channel_2));
    mcu_utils_delay_ms(500, false);
    eeprom_write(EEPROM_WP_PIN, calibration_data->channel_3, EEPROM_OFFSET_OFCAL_CHANNEL_3, sizeof(calibration_data->channel_3));
    mcu_utils_delay_ms(500, false);

    calibration_data->status = 1;
    eeprom_write(EEPROM_WP_PIN, &calibration_data->status, EEPROM_OFFSET_CALIB_STATUS, sizeof(calibration_data->status));
    mcu_utils_delay_ms(500, false);
}

void mode_entry_adc_calibration(void) {
    ads1262_reg_interface_t interface = ads1262_reg_new_interface();
    interface.status = ADS1262_REG_INTERFACE_STATUS_ENABLED;
    interface.crc = ADS1262_REG_INTERFACE_CRC_CRC;
    ads1262_reg_set_interface(&interface);

    ads1262_reg_mode_0_t mode_0 = ads1262_reg_new_mode_0();
    mode_0.run_mode = ADS1262_REG_MODE_0_RUN_MODE_CONTINUOUS;
    ads1262_reg_set_mode_0(&mode_0);

    ads1262_reg_mode_2_t mode_2 = ads1262_reg_new_mode_2();
    mode_2.dr = ADS1262_REG_MODE_2_DR_1200;
    ads1262_reg_set_mode_2(&mode_2);

    uint8_t calibration_status = 0;
    eeprom_read(&calibration_status, EEPROM_OFFSET_CALIB_STATUS, sizeof(calibration_status));
    if (calibration_status == 1) {
        ssd1306_clear();
        ssd1306_display_string(0, 0, "> Calib Data Exists", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
        ssd1306_display_string(0, 1, "> Continue in 5 sec", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
        mcu_utils_delay_ms(5000, false);
    }

    ssd1306_clear();
    ssd1306_display_string(0, 0, "Step 1/6:", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    ssd1306_display_string(0, 4, "Short channel inputs,", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    ssd1306_display_string(0, 5, "Set BOOT1 to right", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    adc_calibration_wait_for_pin(MCU_BOOT1_PIN, 1);

    ssd1306_clear();
    ssd1306_display_string(0, 0, "Step 2/6:", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    ssd1306_display_string(0, 4, "Dip switch OPTIONS", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    ssd1306_display_string(0, 5, "Set all bits to 1", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    while (!mcu_utils_gpio_read(OPTIONS_USE_ACCELEROMETER_PIN) ||
           !mcu_utils_gpio_read(OPTIONS_USE_GNSS_PIN) ||
           !mcu_utils_gpio_read(OPTIONS_CHANNEL_6D_PIN)) {
        ;
    }
    mcu_utils_led_blink(MCU_STATE_PIN, 5, false);
    mcu_utils_delay_ms(500, false);

    adc_calibration_data_t calibration_data = {0};

    adc_calibration_show_channel(3, "EHZ");
    adc_calibration_read_offset(ADS1262_REG_INPMUX_AIN1, ADS1262_REG_INPMUX_AIN0, calibration_data.channel_1);
    adc_calibration_wait_for_channel_switch(OPTIONS_USE_ACCELEROMETER_PIN, 1);

    adc_calibration_show_channel(4, "EHE");
    adc_calibration_read_offset(ADS1262_REG_INPMUX_AIN3, ADS1262_REG_INPMUX_AIN2, calibration_data.channel_2);
    adc_calibration_wait_for_channel_switch(OPTIONS_USE_GNSS_PIN, 2);

    adc_calibration_show_channel(5, "EHN");
    adc_calibration_read_offset(ADS1262_REG_INPMUX_AIN5, ADS1262_REG_INPMUX_AIN4, calibration_data.channel_3);
    adc_calibration_wait_for_channel_switch(OPTIONS_CHANNEL_6D_PIN, 3);

    ssd1306_clear();
    ssd1306_display_string(0, 0, "Step 6/6:", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    ssd1306_display_string(0, 4, "Set BOOT1 to left", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    ssd1306_display_string(0, 5, "To apply new data", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    while (mcu_utils_gpio_read(MCU_BOOT1_PIN)) {
        ;
    }

    adc_calibration_write_data(&calibration_data);
    mcu_utils_led_blink(MCU_STATE_PIN, 5, false);

    ssd1306_clear();
    ssd1306_display_string(0, 0, "INFO:", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    ssd1306_display_string(0, 4, "All Complete!", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, false);

    while (1) {
        mcu_utils_gpio_high(MCU_STATE_PIN);
        mcu_utils_delay_ms(1000, false);
        mcu_utils_gpio_low(MCU_STATE_PIN);
        mcu_utils_delay_ms(1000, false);
    }
}
