#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "Core/Inc/dma.h"
#include "Core/Inc/main.h"
#include "cmsis_os2.h"

#include "Utils/Inc/delay.h"
#include "Utils/Inc/gpio.h"
#include "Utils/Inc/iwdg.h"
#include "Utils/Inc/led.h"
#include "Utils/Inc/uart.h"
#include "Utils/Inc/uptime.h"

#include "User/Inc/ssd1306/display.h"
#include "User/Inc/ssd1306/font.h"

#include "User/Inc/eeprom/read.h"

#include "User/Inc/gnss/parse.h"
#include "User/Inc/gnss/time.h"
#include "User/Inc/gnss/utils.h"

#include "User/Inc/array.h"
#include "User/Inc/calibration.h"
#include "User/Inc/magic.h"
#include "User/Inc/packet.h"
#include "User/Inc/peripheral.h"
#include "User/Inc/reader.h"
#include "User/Inc/settings.h"
#include "User/Inc/types.h"

void task_calib_gnss(void* argument) {
    explorer_states_t* states = (explorer_states_t*)argument;

    while (true) {
        int64_t timestamp_sec = gnss_get_current_timestamp(states->local_base_timestamp, states->gnss_ref_timestamp) / 1000;

        // Calibrate GNSS time at UTC 00:00:00 every day
        if (timestamp_sec % 86400 == 0) {
            uint8_t attempts = 0;
            bool success = false;

            while (attempts < 3 && !success) {
                if (gnss_get_0pps(GNSS_CTL_PIN, &states->local_base_timestamp, false)) {
                    if (gnss_get_sentence(states->gnss_message_buffer, GNSS_SENTENCE_TYPE_RMC)) {
                        gnss_padding_sentence(states->gnss_message_buffer);
                        gnss_parse_rmc(&states->gnss_location, &states->gnss_time, states->gnss_message_buffer);
                        states->gnss_ref_timestamp = gnss_get_timestamp(&states->gnss_time);
                        success = true;
                    }
                }

                if (!success) {
                    attempts++;
                    if (attempts < 3) {
                        mcu_utils_delay_ms(500, true);
                    }
                }
            }

            if (!success) {
                continue;
            }
        }

        mcu_utils_delay_ms(500, true);
    }
}

void task_send_packet(void* argument) {
    explorer_states_t* states = (explorer_states_t*)argument;

    mcu_utils_iwdg_init();

    acquisition_message_t acquisition_message;
    int64_t message_timestamp = 0;

    for (uint8_t message_idx = 0;;) {
        if (osMessageQueueGet(states->acquisition_data_queue, &acquisition_message, NULL, osWaitForever) == osOK) {
            if (message_timestamp == 0) {
                message_timestamp = acquisition_message.timestamp;
            }

            uint8_t message_idx_offset1 = message_idx + states->channel_chunk_length;
            uint8_t message_idx_offset2 = message_idx_offset1 + states->channel_chunk_length;

            if (!states->use_accelerometer || states->channel_6d) {
                states->adc_acquisition_channel_buffer->data[message_idx] = acquisition_message.adc_data[0];
                states->adc_acquisition_channel_buffer->data[message_idx_offset1] = acquisition_message.adc_data[1];
                states->adc_acquisition_channel_buffer->data[message_idx_offset2] = acquisition_message.adc_data[2];
            }

            if (states->use_accelerometer || states->channel_6d) {
                states->accel_acquisition_channel_buffer->data[message_idx] = acquisition_message.accel_data[0];
                states->accel_acquisition_channel_buffer->data[message_idx_offset1] = acquisition_message.accel_data[1];
                states->accel_acquisition_channel_buffer->data[message_idx_offset2] = acquisition_message.accel_data[2];
            }

            message_idx++;
            if (message_idx == states->channel_chunk_length) {
                send_data_packet(states, acquisition_message.temperature, message_timestamp);
                message_timestamp = 0;
                message_idx = 0;
            }
        }

        mcu_utils_iwdg_feed();
    }
}

void task_acquire_data(void* argument) {
    explorer_states_t* states = (explorer_states_t*)argument;
    uint32_t time_span = 1000 / states->sample_rate;
    uint32_t last_tick = osKernelGetTickCount();

    acquisition_message_t acq_msg;
    while (true) {
        acq_msg.timestamp = states->use_gnss_time ? gnss_get_current_timestamp(states->local_base_timestamp, states->gnss_ref_timestamp) : mcu_utils_uptime_ms();

        if (!states->use_accelerometer || states->channel_6d) {
            get_adc_readout(ADS1262_CTL_PIN, acq_msg.adc_data);
        }

        if (states->use_accelerometer || states->channel_6d) {
            get_accel_readout(acq_msg.accel_data);
        }
        get_env_temperature(&acq_msg.temperature);

        osMessageQueuePut(states->acquisition_data_queue, &acq_msg, 0, 0);

        uint32_t elapsed_time = osKernelGetTickCount() - last_tick;
        if (elapsed_time < time_span) {
            mcu_utils_delay_ms(time_span - elapsed_time, true);
        }
        last_tick = osKernelGetTickCount();
    }
}

void read_device_settings(explorer_states_t* states) {
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

void read_gnss_data(explorer_states_t* states) {
    ssd1306_display_string(0, 0, "Fetch GNSS Data", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE);

    for (char display_buf[20];;) {
        mcu_utils_led_blink(MCU_STATE_PIN, 5, false);

        if (gnss_get_sentence(states->gnss_message_buffer, GNSS_SENTENCE_TYPE_GGA)) {
            gnss_padding_sentence(states->gnss_message_buffer);
            gnss_parse_gga(&states->gnss_status, &states->gnss_location, states->gnss_message_buffer);
            snprintf(display_buf, sizeof(display_buf), "SAT: %d, HDOP: %.1f", states->gnss_status.satellites, states->gnss_status.hdop);
            ssd1306_display_string(0, 2, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
        }

        if (!gnss_get_0pps(GNSS_CTL_PIN, &states->local_base_timestamp, true)) {
            ssd1306_display_string(0, 0, "GNSS Not Ready!", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE);
            continue;
        }

        if (gnss_get_sentence(states->gnss_message_buffer, GNSS_SENTENCE_TYPE_RMC)) {
            gnss_padding_sentence(states->gnss_message_buffer);
            gnss_parse_rmc(&states->gnss_location, &states->gnss_time, states->gnss_message_buffer);
            states->gnss_ref_timestamp = gnss_get_timestamp(&states->gnss_time);
        }

        if (states->gnss_time.is_valid && states->gnss_location.is_valid &&
            states->gnss_status.hdop <= 1) {
            ssd1306_display_string(0, 0, "GNSS Data Valid", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE);
            mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
            mcu_utils_delay_ms(1000, false);
            break;
        }
    }
}

void spirit_level_mode(void) {
    ssd1306_clear();
    ssd1306_display_string(0, 0, "> Spirit Level", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    ssd1306_display_string(14, 7, "- anyshake.org -", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);

    int16_t result_arr[3];
    char display_buf[19];
    float temperature = 0;
    while (1) {
        get_accel_readout(result_arr);
        get_env_temperature(&temperature);

        int16_t acc_x = result_arr[1];
        int16_t acc_y = result_arr[2];
        int16_t acc_z = result_arr[0];
        float x_angle = quick_atan2(-acc_x, quick_sqrt(acc_y * acc_y + acc_z * acc_z)) * 180.0 / MAGIC_PI;
        float y_angle = quick_atan2(acc_y, acc_z) * 180.0 / MAGIC_PI;

#ifndef USE_LSM6DS3
        x_angle = -x_angle;
#else
        y_angle = -y_angle;
#endif

        snprintf(display_buf, sizeof(display_buf), "TMP: %5.2f * C", temperature);
        ssd1306_display_string(20, 3, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
        snprintf(display_buf, sizeof(display_buf), "X: %7.2f deg", x_angle);
        ssd1306_display_string(20, 4, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
        snprintf(display_buf, sizeof(display_buf), "Y: %7.2f deg", y_angle);
        ssd1306_display_string(20, 5, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);

        mcu_utils_delay_ms(100, false);
    }
}

void display_device_settings(explorer_states_t* states) {
    char display_buf[20];

    snprintf(display_buf, sizeof(display_buf), "SAMPLE RATE: %hhu Hz", states->sample_rate);
    ssd1306_display_string(0, 0, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    snprintf(display_buf, sizeof(display_buf), "PORT BR: %lu bps", states->baud_rate);
    ssd1306_display_string(0, 1, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    snprintf(display_buf, sizeof(display_buf), "ACCEL ENABLE: %s", states->use_accelerometer || states->channel_6d ? "YES" : "NO");
    ssd1306_display_string(0, 2, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    snprintf(display_buf, sizeof(display_buf), "GNSS ENABLE: %s", states->use_gnss_time ? "YES" : "NO");
    ssd1306_display_string(0, 3, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    snprintf(display_buf, sizeof(display_buf), "CHANNEL 6D: %s", states->channel_6d ? "YES" : "NO");
    ssd1306_display_string(0, 4, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    snprintf(display_buf, sizeof(display_buf), "DEVICE ID: %08lX", states->device_id);
    ssd1306_display_string(0, 5, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    snprintf(display_buf, sizeof(display_buf), "FW REV: %s", FW_REV);
    ssd1306_display_string(0, 6, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    ssd1306_display_string(14, 7, "- anyshake.org -", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
}

void system_setup(void) {
    MX_DMA_Init();

    mcu_utils_gpio_init(false);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_gpio_high(MCU_STATE_PIN);

    mcu_utils_i2c_init(false);
    mcu_utils_spi_init(false);
    mcu_utils_uart2_init(GNSS_BAUDRATE, false);

    static explorer_states_t states;
    read_device_settings(&states);
    mcu_utils_uart_init(states.baud_rate, false);

    peri_screen_init();
    ssd1306_display_bitmap(0, 0, 128, 8, ANYSHAKE_LOGO_BITMAP, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    ssd1306_display_string(0, 0, "Peripheral Init", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_delay_ms(1000, false);

    peri_eeprom_init();
    eeprom_read((uint8_t*)&states.device_id, sizeof(states.device_id));

#ifndef USE_LSM6DS3
    icm42688_reset(false);
#else
    lsm6ds3_reset(false);
#endif
    peri_imu_init(states.sample_rate);
    mcu_utils_delay_ms(1000, false);

    if (states.leveling_mode) {
        spirit_level_mode();
    }

    ads1262_init(ADS1262_CTL_PIN, ADS1262_INIT_CONTROL_TYPE_HARD);
    ads1262_reset(ADS1262_CTL_PIN, ADS1262_RESET_RESET_TYPE_HARD, false);
    peri_adc_init(ADS1262_INIT_CONTROL_TYPE_HARD, states.sample_rate);
    if (!states.use_accelerometer || states.channel_6d) {
        ssd1306_display_string(0, 0, "ADC Calibrating", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE);
        calibrate_adc_offset(ADS1262_CTL_PIN);
        mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
        mcu_utils_delay_ms(1000, false);
    }

    peri_gnss_init();
    if (states.use_gnss_time) {
        gnss_reset(GNSS_CTL_PIN, false);
        read_gnss_data(&states);
    } else {
        mcu_utils_gpio_low(GNSS_CTL_PIN.rst);
    }

    states.packet_sending_interval = PACKET_INTERVAL;
    states.channel_chunk_length = states.packet_sending_interval / (1000 / states.sample_rate);
    states.uart_packet_buffer = array_uint8_make(get_data_packet_size(states.use_accelerometer, states.channel_6d, states.channel_chunk_length));

    if (!states.use_accelerometer || states.channel_6d) {
        states.adc_acquisition_channel_buffer = array_int32_make(states.channel_chunk_length * 3);
    }
    if (states.use_accelerometer || states.channel_6d) {
        states.accel_acquisition_channel_buffer = array_int16_make(states.channel_chunk_length * 3);
    }

    ssd1306_display_string(0, 0, "Device Starting", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_delay_ms(1000, false);

    if (states.use_gnss_time) {
        const osThreadAttr_t task_calib_gnss_attr = {.name = "calibrate_gnss", .stack_size = 128 * 4};
        osThreadNew(task_calib_gnss, &states, &task_calib_gnss_attr);
    }

    const osMessageQueueAttr_t acquisition_data_queue_attr = {.name = "acquisition_data"};
    states.acquisition_data_queue = osMessageQueueNew(states.channel_chunk_length, sizeof(acquisition_message_t), &acquisition_data_queue_attr);

    const osThreadAttr_t task_send_packet_attr = {.name = "send_packet", .stack_size = 128 * 4};
    osThreadNew(task_send_packet, &states, &task_send_packet_attr);

    const osThreadAttr_t task_acquire_data_attr = {.name = "acquire_data", .stack_size = 128 * 4};
    osThreadNew(task_acquire_data, &states, &task_acquire_data_attr);

    ssd1306_clear();
    display_device_settings(&states);
}

int main(void) {
    SystemClock_Config();
    HAL_Init();

    osKernelInitialize();
    system_setup();
    osKernelStart();

    while (true) {
        ;
    }
}
