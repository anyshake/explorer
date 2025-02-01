#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "Core/Inc/main.h"
#include "cmsis_os.h"

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

#ifndef FW_REV
#define FW_REV "custombuild"
#endif

typedef struct {
    // Leveling mode
    bool leveling_mode;
    // User options
    uint32_t device_id;
    uint32_t baud_rate;
    uint8_t sample_rate;
    uint8_t channel_samples;
    bool no_geophone;
    bool use_gnss_time;
    bool legacy_mode;
    // To get current GNSS time
    int64_t gnss_ref_timestamp;
    int64_t local_base_timestamp;
    // GNSS message buffer
    uint8_t gnss_message[GNSS_SENTENCE_BUFFER_SIZE];
    gnss_location_t gnss_location;
    gnss_status_t gnss_status;
    gnss_time_t gnss_time;
    // ADC data buffer and RTOS resources
    int32_array_t* adc_channel_buffer;
    osMessageQueueId_t reader_drdy_queue;
    // Packet buffer for UART transmission
    uint8_array_t* uart_packet_buffer;
} explorer_states_t;

void task_read_adc(void* argument) {
    explorer_states_t* states = (explorer_states_t*)argument;
    uint8_t time_span = 1000 / states->sample_rate;

    for (int64_t current_timestamp = 0;;) {
        while (mcu_utils_uptime_ms() % (time_span * states->channel_samples) != 0) {
            if (states->use_gnss_time) {
                current_timestamp = gnss_get_current_timestamp(states->local_base_timestamp, states->gnss_ref_timestamp);
            } else {
                current_timestamp = mcu_utils_uptime_ms();
            }
        }

        if (states->no_geophone) {
#ifndef USE_LSM6DS3
            get_accel_readout(ICM42688_INTS_PIN, states->adc_channel_buffer, states->channel_samples);
#else
            get_accel_readout(LSM6DS3_INTS_PIN, states->adc_channel_buffer, states->channel_samples);
#endif
        } else {
            get_adc_readout(ADS1262_CTL_PIN, states->adc_channel_buffer, states->channel_samples);
        }

        osMessageQueuePut(states->reader_drdy_queue, &current_timestamp, 0, 0);
    }
}

void task_send_data(void* argument) {
    explorer_states_t* states = (explorer_states_t*)argument;
    uint32_t device_info = states->use_gnss_time ? states->device_id | 0x80000000 : states->device_id & 0x7FFFFFFF;

    while (true) {
        int64_t timestamp;
        if (osMessageQueueGet(states->reader_drdy_queue, &timestamp, NULL, 0) == osOK) {
            if (states->legacy_mode) {
                send_legacy_data_packet(states->adc_channel_buffer, states->channel_samples);
            } else {
                send_data_packet(states->adc_channel_buffer, states->uart_packet_buffer, timestamp, &states->gnss_location, device_info, states->channel_samples);
            }
        }
    }
}

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
                    if (gnss_get_sentence(states->gnss_message, GNSS_SENTENCE_TYPE_RMC)) {
                        gnss_padding_sentence(states->gnss_message);
                        gnss_parse_rmc(&states->gnss_location, &states->gnss_time, states->gnss_message);
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

void task_feed_iwdg(void* argument) {
    bool is_led_on = false;
    mcu_utils_iwdg_init(false);

    while (true) {
        is_led_on = !is_led_on;
        if (is_led_on) {
            mcu_utils_gpio_high(MCU_STATE_PIN);
        } else {
            mcu_utils_gpio_low(MCU_STATE_PIN);
        }
        mcu_utils_iwdg_feed();
        mcu_utils_delay_ms(1000, true);
    }
}

void read_device_settings(explorer_states_t* states) {
    mcu_utils_gpio_mode(MCU_BOOT1_PIN, MCU_UTILS_GPIO_MODE_INPUT);
    states->leveling_mode = mcu_utils_gpio_read(MCU_BOOT1_PIN);

    mcu_utils_gpio_mode(OPTIONS_NO_GEOPHONE_PIN, MCU_UTILS_GPIO_MODE_INPUT);
    states->no_geophone = mcu_utils_gpio_read(OPTIONS_NO_GEOPHONE_PIN);

    mcu_utils_gpio_mode(OPTIONS_USE_GNSS_PIN, MCU_UTILS_GPIO_MODE_INPUT);
    states->use_gnss_time = mcu_utils_gpio_read(OPTIONS_USE_GNSS_PIN);

    mcu_utils_gpio_mode(OPTIONS_LEGACY_MODE_PIN, MCU_UTILS_GPIO_MODE_INPUT);
    states->legacy_mode = mcu_utils_gpio_read(OPTIONS_LEGACY_MODE_PIN);

    mcu_utils_gpio_mode(SAMPLERATE_SELECT_P1, MCU_UTILS_GPIO_MODE_INPUT);
    mcu_utils_gpio_mode(SAMPLERATE_SELECT_P2, MCU_UTILS_GPIO_MODE_INPUT);
    states->channel_samples = states->legacy_mode ? LEGACY_PACKET_CHANNEL_SAMPLES : MAINLINE_PACKET_CHANNEL_SAMPLES;
    switch (mcu_utils_gpio_read(SAMPLERATE_SELECT_P1) << 1 | mcu_utils_gpio_read(SAMPLERATE_SELECT_P2)) {
        case 1:
            states->sample_rate = 100;
            break;
        case 2:
            states->sample_rate = 50;
            break;
        case 3:
            states->sample_rate = 25;
            break;
        default:
            states->sample_rate = 125;
            break;
    }

    mcu_utils_gpio_mode(BAUDRATE_SELECT_P1, MCU_UTILS_GPIO_MODE_INPUT);
    mcu_utils_gpio_mode(BAUDRATE_SELECT_P2, MCU_UTILS_GPIO_MODE_INPUT);
    switch (mcu_utils_gpio_read(BAUDRATE_SELECT_P1) << 1 | mcu_utils_gpio_read(BAUDRATE_SELECT_P2)) {
        case 1:
            states->baud_rate = 76800;
            break;
        case 2:
            states->baud_rate = 57600;
            break;
        case 3:
            states->baud_rate = 38400;
            break;
        default:
            states->baud_rate = 115200;
            break;
    }
}

void read_gnss_data(explorer_states_t* states) {
    ssd1306_display_string(0, 0, "Fetch GNSS Data", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE);

    for (char display_buf[24];;) {
        mcu_utils_led_blink(MCU_STATE_PIN, 5, false);

        // Read NMEA message for status and elevation
        if (gnss_get_sentence(states->gnss_message, GNSS_SENTENCE_TYPE_GGA)) {
            gnss_padding_sentence(states->gnss_message);
            gnss_parse_gga(&states->gnss_status, &states->gnss_location, states->gnss_message);
            snprintf(display_buf, sizeof(display_buf), "SAT: %d, HDOP: %.1f", states->gnss_status.satellites, states->gnss_status.hdop);
            ssd1306_display_string(0, 2, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
        }

        // Wait for PPS signal
        if (!gnss_get_0pps(GNSS_CTL_PIN, &states->local_base_timestamp, true)) {
            ssd1306_display_string(0, 0, "GNSS Not Ready!",
                                   SSD1306_FONT_TYPE_ASCII_8X16,
                                   SSD1306_FONT_DISPLAY_COLOR_WHITE);
            continue;
        }

        // Read NMEA message for RMC data
        if (gnss_get_sentence(states->gnss_message, GNSS_SENTENCE_TYPE_RMC)) {
            gnss_padding_sentence(states->gnss_message);
            gnss_parse_rmc(&states->gnss_location, &states->gnss_time, states->gnss_message);
            states->gnss_ref_timestamp = gnss_get_timestamp(&states->gnss_time);
        }

        // Check if GNSS data is valid
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
    int32_array_t* accel_readout_buffer = array_int32_make(3);
    ssd1306_clear();
    ssd1306_display_string(0, 0, "> Spirit Level", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    ssd1306_display_string(14, 7, "- anyshake.org -", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    char display_buf[19];
    while (1) {
#ifndef USE_LSM6DS3
        get_accel_readout(ICM42688_INTS_PIN, accel_readout_buffer, 1);
#else
        get_accel_readout(LSM6DS3_INTS_PIN, accel_readout_buffer, 1);
#endif
        int32_t acc_x = accel_readout_buffer->data[1];
        int32_t acc_y = accel_readout_buffer->data[2];
        int32_t acc_z = accel_readout_buffer->data[0];
        float x_angle = quick_atan2(-acc_x, quick_sqrt(acc_y * acc_y + acc_z * acc_z)) * 180.0 / MAGIC_PI;
        float y_angle = quick_atan2(acc_y, acc_z) * 180.0 / MAGIC_PI;

#ifndef USE_LSM6DS3
        x_angle = -x_angle;
#else
        y_angle = -y_angle;
#endif

        snprintf(display_buf, sizeof(display_buf), "X: %7.2f deg", x_angle);
        ssd1306_display_string(20, 3, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
        snprintf(display_buf, sizeof(display_buf), "Y: %7.2f deg", y_angle);
        ssd1306_display_string(20, 4, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);

        mcu_utils_delay_ms(100, false);
    }
}

void display_device_settings(explorer_states_t* states) {
    char display_buf[24];

    snprintf(display_buf, sizeof(display_buf), "SAMPLE RATE: %d Hz", (int)states->sample_rate);
    ssd1306_display_string(0, 0, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    snprintf(display_buf, sizeof(display_buf), "PORT BR: %d bps", (int)states->baud_rate);
    ssd1306_display_string(0, 1, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    snprintf(display_buf, sizeof(display_buf), "NO GEOPHONE: %s", states->no_geophone ? "YES" : "NO");
    ssd1306_display_string(0, 2, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    snprintf(display_buf, sizeof(display_buf), "GNSS ENABLE: %s", states->use_gnss_time ? "YES" : "NO");
    ssd1306_display_string(0, 3, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    snprintf(display_buf, sizeof(display_buf), "LEGACY MODE: %s", states->legacy_mode ? "YES" : "NO");
    ssd1306_display_string(0, 4, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    snprintf(display_buf, sizeof(display_buf), "DEVICE ID: %08X", (int)states->device_id);
    ssd1306_display_string(0, 5, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    snprintf(display_buf, sizeof(display_buf), "FW REV: %s", FW_REV);
    ssd1306_display_string(0, 6, display_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    ssd1306_display_string(14, 7, "- anyshake.org -", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE);
}

void system_setup(void) {
    mcu_utils_gpio_init(false);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_gpio_high(MCU_STATE_PIN);

    mcu_utils_i2c_init(false);
    mcu_utils_spi_init(false);
    mcu_utils_uart2_init(GNSS_UART_BAUDRATE, false);

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

    ads1262_init(ADS1262_CTL_PIN, ADS1262_INIT_CONTROL_TYPE_HARD);
    ads1262_reset(ADS1262_CTL_PIN, ADS1262_RESET_RESET_TYPE_HARD, false);
    peri_adc_init(ADS1262_INIT_CONTROL_TYPE_HARD, states.sample_rate);
    if (!states.no_geophone) {
        ssd1306_display_string(0, 0, "Calibrating ADC", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE);
        calibrate_adc_offset(ADS1262_CTL_PIN);
        mcu_utils_delay_ms(1000, false);
    }

#ifndef USE_LSM6DS3
    icm42688_reset(false);
#else
    lsm6ds3_reset(false);
#endif
    peri_imu_init();
    mcu_utils_delay_ms(1000, false);

    if (states.leveling_mode) {
        spirit_level_mode();
    }

    peri_gnss_init();
    if (!states.legacy_mode && states.use_gnss_time) {
        gnss_reset(GNSS_CTL_PIN, false);
        read_gnss_data(&states);
    } else {
        mcu_utils_gpio_low(GNSS_CTL_PIN.rst);
    }

    states.adc_channel_buffer = array_int32_make(3 * states.channel_samples);
    if (!states.legacy_mode) {
        uint8_t packet_size = get_data_packet_size(states.channel_samples);
        states.uart_packet_buffer = array_uint8_make(packet_size);
    }

    ssd1306_display_string(0, 0, "Device Started!", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_delay_ms(1000, false);

    ssd1306_clear();
    display_device_settings(&states);

    const osThreadAttr_t read_adc_attr = {.name = "Read ADC", .stack_size = 128 * 2, .priority = (osPriority_t)osPriorityNormal};
    osThreadNew(task_read_adc, &states, &read_adc_attr);

    const osThreadAttr_t send_data_attr = {.name = "Send Data", .stack_size = 128 * 4, .priority = (osPriority_t)osPriorityNormal};
    osThreadNew(task_send_data, &states, &send_data_attr);

    const osThreadAttr_t feed_iwdg_attr = {.name = "Feed IWDG", .stack_size = 32, .priority = (osPriority_t)osPriorityNormal};
    osThreadNew(task_feed_iwdg, NULL, &feed_iwdg_attr);

    if (!states.legacy_mode && states.use_gnss_time) {
        const osThreadAttr_t calib_gnss_attr = {.name = "GNSS Calib", .stack_size = 128 * 4, .priority = (osPriority_t)osPriorityNormal};
        osThreadNew(task_calib_gnss, &states, &calib_gnss_attr);
    }

    const osMessageQueueAttr_t reader_drdy_queue_attr = {.name = "DRDY Timestamp"};
    states.reader_drdy_queue = osMessageQueueNew(1, sizeof(int64_t), &reader_drdy_queue_attr);
}

int main(void) {
    SystemClock_Config();
    HAL_Init();

    osKernelInitialize();
    MX_DMA_Init();
    system_setup();
    osKernelStart();

    while (true) {
        ;
    }
}
