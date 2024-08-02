#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "Core/Inc/main.h"
#include "cmsis_os.h"

#include "Utils/Inc/crc32.h"
#include "Utils/Inc/delay.h"
#include "Utils/Inc/gpio.h"
#include "Utils/Inc/iwdg.h"
#include "Utils/Inc/led.h"
#include "Utils/Inc/uart.h"
#include "Utils/Inc/uptime.h"

#include "User/Inc/eeprom/read.h"
#include "User/Inc/eeprom/utils.h"

#include "User/Inc/ssd1306/display.h"
#include "User/Inc/ssd1306/utils.h"

#include "User/Inc/lsm6ds3/regs/ctrl1_xl.h"
#include "User/Inc/lsm6ds3/regs/outx_xl.h"
#include "User/Inc/lsm6ds3/regs/outy_xl.h"
#include "User/Inc/lsm6ds3/regs/outz_xl.h"
#include "User/Inc/lsm6ds3/utils.h"

#include "User/Inc/ads1262/cmds/rdata.h"
#include "User/Inc/ads1262/regs/id.h"
#include "User/Inc/ads1262/regs/inpmux.h"
#include "User/Inc/ads1262/regs/mode_0.h"
#include "User/Inc/ads1262/regs/mode_1.h"
#include "User/Inc/ads1262/regs/mode_2.h"

#include "User/Inc/gnss/parse.h"
#include "User/Inc/gnss/time.h"
#include "User/Inc/gnss/utils.h"

#include "User/Inc/array.h"
#include "User/Inc/packet.h"
#include "User/Inc/settings.h"
#include "User/Inc/version.h"

typedef struct {
    // Startup settings
    uint32_t device_id;
    uint32_t baud_rate;
    uint16_t sample_rate;
    bool no_geophone;
    bool legacy_mode;
    bool adc_24bit_mode;
    // To get current GNSS time
    int64_t gnss_ref_timestamp;
    int64_t local_base_timestamp;
    // GNSS message buffer
    uint8_t gnss_message[GNSS_SENTENCE_BUFFER_SIZE];
    gnss_location_t gnss_location;
    gnss_time_t gnss_time;
    // ADC data buffer and RTOS resources
    int32_array_t* adc_channel_buffer;
    osMessageQueueId_t reader_drdy_queue;
    // Packet buffer for UART transmission
    uint8_array_t* uart_packet_buffer;
} explorer_states_t;

void task_read_adc(void* argument) {
    explorer_states_t* states = (explorer_states_t*)argument;

    // Initialize accelerometer result
    lsm6ds3_reg_outz_xl_t outz_xl;
    lsm6ds3_reg_outx_xl_t outx_xl;
    lsm6ds3_reg_outy_xl_t outy_xl;

    // Get time span (in ms) for sampling
    uint8_t time_span = 1000 / states->sample_rate;

    if (states->legacy_mode) {
        while (true) {
            int64_t current_timestamp = mcu_utils_uptime_ms();
            while (current_timestamp %
                       (time_span * LEGACY_PACKET_CHANNEL_SIZE) !=
                   0) {
                current_timestamp = mcu_utils_uptime_ms();
            }

            for (uint8_t n = 0; n < LEGACY_PACKET_CHANNEL_SIZE; n++) {
                if (states->no_geophone) {
                    // Read Z-axis accelerometer data
                    lsm6ds3_reg_get_outz_xl(&outz_xl);
                    states->adc_channel_buffer->data[n] =
                        outz_xl.outz_h_xl << 8 | outz_xl.outz_l_xl;
                    // Read E-axis accelerometer data
                    lsm6ds3_reg_get_outx_xl(&outx_xl);
                    states->adc_channel_buffer
                        ->data[n + LEGACY_PACKET_CHANNEL_SIZE] =
                        outx_xl.outx_h_xl << 8 | outx_xl.outx_l_xl;
                    // Read N-axis accelerometer data
                    lsm6ds3_reg_get_outy_xl(&outy_xl);
                    states->adc_channel_buffer
                        ->data[n + 2 * LEGACY_PACKET_CHANNEL_SIZE] =
                        outy_xl.outy_h_xl << 8 | outy_xl.outy_l_xl;
                } else {
                    ;
                }
            }

            osMessageQueuePut(states->reader_drdy_queue, &current_timestamp, 0,
                              0);
        }
    } else {
        int64_t prev_timestamp = 0;

        for (uint16_t n = 0;;) {
            int64_t current_timestamp = gnss_get_current_timestamp(
                states->local_base_timestamp, states->gnss_ref_timestamp);

            if (current_timestamp - prev_timestamp >= time_span) {
                prev_timestamp = current_timestamp;
                if (states->no_geophone) {
                    // Read Z-axis accelerometer data
                    lsm6ds3_reg_get_outz_xl(&outz_xl);
                    states->adc_channel_buffer->data[n % states->sample_rate] =
                        outz_xl.outz_h_xl << 8 | outz_xl.outz_l_xl;
                    // Read E-axis accelerometer data
                    lsm6ds3_reg_get_outx_xl(&outx_xl);
                    states->adc_channel_buffer->data[(n % states->sample_rate) +
                                                     states->sample_rate] =
                        outx_xl.outx_h_xl << 8 | outx_xl.outx_l_xl;
                    // Read N-axis accelerometer data
                    lsm6ds3_reg_get_outy_xl(&outy_xl);
                    states->adc_channel_buffer->data[(n % states->sample_rate) +
                                                     2 * states->sample_rate] =
                        outy_xl.outy_h_xl << 8 | outy_xl.outy_l_xl;
                } else {
                    ;
                }

                if (n % states->sample_rate == 0) {
                    osMessageQueuePut(states->reader_drdy_queue,
                                      &current_timestamp, 0, 0);
                }

                n++;
            }
        }
    }
}

void task_send_data(void* argument) {
    explorer_states_t* states = (explorer_states_t*)argument;

    while (true) {
        int64_t timestamp;
        if (osMessageQueueGet(states->reader_drdy_queue, &timestamp, NULL, 0) ==
            osOK) {
            if (states->legacy_mode) {
                send_legacy_data_packet(states->adc_channel_buffer);
            } else {
                send_data_packet(states->adc_channel_buffer,
                                 states->uart_packet_buffer,
                                 &states->gnss_location, timestamp,
                                 states->sample_rate, states->device_id);
            }
        }
    }
}

void task_calib_gnss(void* argument) {
    explorer_states_t* states = (explorer_states_t*)argument;
    bool has_succeed = true;

    while (true) {
        int64_t timestamp =
            gnss_get_current_timestamp(states->local_base_timestamp,
                                       states->gnss_ref_timestamp) /
            1000;

        // Calibrate GNSS time at UTC 00:00:00 every day
        if (timestamp % 86400 == 0 || !has_succeed) {
            has_succeed = false;

            if (!gnss_get_0pps(GNSS_CTL_PIN, &states->local_base_timestamp,
                               false)) {
                continue;
            }

            if (gnss_get_sentence(states->gnss_message,
                                  GNSS_SENTENCE_TYPE_RMC)) {
                gnss_padding_sentence(states->gnss_message);
                gnss_parse_rmc(&states->gnss_location, &states->gnss_time,
                               states->gnss_message);
                states->gnss_ref_timestamp =
                    gnss_get_timestamp(&states->gnss_time);
                has_succeed = true;
            }
        }

        mcu_utils_delay_ms(100, true);
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

void peripherals_init(explorer_states_t* states) {
    // Initialize CRC32 module
    mcu_utils_crc32_init();

    // Initialize state LED pin
    mcu_utils_gpio_init(false);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);

    // Initialize SSD1306 display
    ssd1306_init(false);
    ssd1306_enable();
    ssd1306_clear();
    const char version[] = FW_VERSION;
    ssd1306_display_bitmap(0, 0, 128, 8, ANYSHAKE_LOGO_BITMAP,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE);
    ssd1306_display_string(0, 0, version, SSD1306_FONT_TYPE_ASCII_8X16,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE);
    mcu_utils_delay_ms(1000, false);

    // Get user options from DIP switches
    mcu_utils_gpio_mode(OPTIONS_NO_GEOPHONE_PIN, MCU_UTILS_GPIO_MODE_INPUT);
    states->no_geophone = mcu_utils_gpio_read(OPTIONS_NO_GEOPHONE_PIN);
    mcu_utils_gpio_mode(OPTIONS_LEGACY_MODE_PIN, MCU_UTILS_GPIO_MODE_INPUT);
    states->legacy_mode = mcu_utils_gpio_read(OPTIONS_LEGACY_MODE_PIN);
    mcu_utils_gpio_mode(OPTIONS_ADC_24BIT_MODE_PIN, MCU_UTILS_GPIO_MODE_INPUT);
    if (states->legacy_mode) {
        states->adc_24bit_mode = false;
    } else {
        states->adc_24bit_mode =
            mcu_utils_gpio_read(OPTIONS_ADC_24BIT_MODE_PIN);
    }

    // Get sample rate from DIP switches
    mcu_utils_gpio_mode(SAMPLERATE_SELECT_P1, MCU_UTILS_GPIO_MODE_INPUT);
    mcu_utils_gpio_mode(SAMPLERATE_SELECT_P2, MCU_UTILS_GPIO_MODE_INPUT);
    switch (mcu_utils_gpio_read(SAMPLERATE_SELECT_P1) << 1 |
            mcu_utils_gpio_read(SAMPLERATE_SELECT_P2)) {
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

    // Get baud rate from DIP switches
    mcu_utils_gpio_mode(BAUDRATE_SELECT_P1, MCU_UTILS_GPIO_MODE_INPUT);
    mcu_utils_gpio_mode(BAUDRATE_SELECT_P2, MCU_UTILS_GPIO_MODE_INPUT);
    switch (mcu_utils_gpio_read(BAUDRATE_SELECT_P1) << 1 |
            mcu_utils_gpio_read(BAUDRATE_SELECT_P2)) {
        case 1:
            states->baud_rate = 57600;
            break;
        case 2:
            states->baud_rate = 38400;
            break;
        case 3:
            states->baud_rate = 19200;
            break;
        default:
            states->baud_rate = 115200;
            break;
    }

    // Display startup screen
    ssd1306_display_string(0, 0, "Peripheral Init",
                           SSD1306_FONT_TYPE_ASCII_8X16,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE);
    mcu_utils_delay_ms(1000, false);

    // Read device ID from EEPROM
    eeprom_init(EEPROM_WP_PIN, false);
    eeprom_read((uint8_t*)&states->device_id, sizeof(states->device_id));

    // Initialize LSM6DS3 accelerometer
    lsm6ds3_init(LSM6DS3_INTS_PIN, false);
    lsm6ds3_reset(false);
    lsm6ds3_reg_ctrl1_xl_t ctrl1_xl = {
        .odr_xl = LSM6DS3_CTRL1_XL_ODR_XL_6660HZ,
        .fs_xl = LSM6DS3_CTRL1_XL_FS_XL_2G,
    };
    lsm6ds3_reg_set_ctrl1_xl(&ctrl1_xl);

    // Initialize ADS1262 ADC
    ads1262_init(ADS1262_CTL_PIN, ADS1262_INIT_CONTROL_TYPE_HARD, false);
    ads1262_reg_mode_0_t mode_0 = {.run_mode = ADS1262_MODE_0_RUN_MODE_ONESHOT};
    ads1262_reg_set_mode_0(&mode_0);
    ads1262_reg_mode_1_t mode_1 = {.filter = ADS1262_MODE_1_FILTER_SINC1};
    ads1262_reg_set_mode_1(&mode_1);
    ads1262_reg_mode_2_t mode_2;
    switch (states->sample_rate) {
        case 25:
            mode_2.dr = ADS1262_MODE_2_DR_100;
            break;
        case 50:
            mode_2.dr = ADS1262_MODE_2_DR_400;
            break;
        case 100:
        case 125:
            mode_2.dr = ADS1262_MODE_2_DR_1200;
            break;
        default:
            mode_2.dr = ADS1262_MODE_2_DR_38400;
            break;
    }
    ads1262_reg_set_mode_2(&mode_2);

    // Initialize serial port
    mcu_utils_uart_init(states->baud_rate, false);
    mcu_utils_uart_flush();

    // Initialize GNSS module
    gnss_init(GNSS_CTL_PIN, GNSS_UART_BAUDRATE, false);
    gnss_reset(GNSS_CTL_PIN, false);

    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
}

void get_gnss_data(explorer_states_t* states) {
    ssd1306_display_string(0, 0, "Fetch GNSS Data",
                           SSD1306_FONT_TYPE_ASCII_8X16,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE);

    for (bool has_elevation = false;;) {
        // Wait for PPS signal
        if (!gnss_get_0pps(GNSS_CTL_PIN, &states->local_base_timestamp, true)) {
            ssd1306_display_string(0, 0, "GNSS No Signal!",
                                   SSD1306_FONT_TYPE_ASCII_8X16,
                                   SSD1306_FONT_DISPLAY_COLOR_WHITE);
            mcu_utils_led_blink(MCU_STATE_PIN, 5, false);
            continue;
        }

        if (!has_elevation) {
            // Read NMEA message for elevation data
            if (gnss_get_sentence(states->gnss_message,
                                  GNSS_SENTENCE_TYPE_GGA)) {
                gnss_padding_sentence(states->gnss_message);
                gnss_parse_gga(&states->gnss_location, states->gnss_message);
                has_elevation = states->gnss_location.is_valid;
            }
        } else {
            // Read NMEA message for RMC data
            if (gnss_get_sentence(states->gnss_message,
                                  GNSS_SENTENCE_TYPE_RMC)) {
                gnss_padding_sentence(states->gnss_message);
                gnss_parse_rmc(&states->gnss_location, &states->gnss_time,
                               states->gnss_message);
                states->gnss_ref_timestamp =
                    gnss_get_timestamp(&states->gnss_time);
            }
        }

        // Check if GNSS data is valid
        if (states->gnss_time.is_valid && states->gnss_location.is_valid &&
            has_elevation) {
            ssd1306_display_string(0, 0, "GNSS Data Valid",
                                   SSD1306_FONT_TYPE_ASCII_8X16,
                                   SSD1306_FONT_DISPLAY_COLOR_WHITE);
            mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
            mcu_utils_delay_ms(1000, false);
            break;
        }
    }
}

void display_settings(explorer_states_t* states) {
    char display_buf[24];

    snprintf(display_buf, sizeof(display_buf), "BAUD RATE: B%d",
             (int)states->baud_rate);
    ssd1306_display_string(0, 0, display_buf, SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE);
    snprintf(display_buf, sizeof(display_buf), "SAMPLE RATE: %d Hz",
             (int)states->sample_rate);
    ssd1306_display_string(0, 1, display_buf, SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE);
    snprintf(display_buf, sizeof(display_buf), "NO GEOPHONE: %s",
             (int)states->no_geophone ? "YES" : "NO");
    ssd1306_display_string(0, 2, display_buf, SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE);
    snprintf(display_buf, sizeof(display_buf), "LEGACY MODE: %s",
             (int)states->legacy_mode ? "YES" : "NO");
    ssd1306_display_string(0, 3, display_buf, SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE);
    snprintf(display_buf, sizeof(display_buf), "24-BIT MODE: %s",
             (int)states->adc_24bit_mode ? "YES" : "NO");
    ssd1306_display_string(0, 4, display_buf, SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE);
    snprintf(display_buf, sizeof(display_buf), "DEVICE ID: %08X",
             (int)states->device_id);
    ssd1306_display_string(0, 5, display_buf, SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE);
    ssd1306_display_string(28, 6, "anyshake.org", SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE);
    ssd1306_display_string(13, 7, "AnyShake Explorer",
                           SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE);
}

void setup(void) {
    // Initialize peripherals
    static explorer_states_t states;
    peripherals_init(&states);
    if (!states.legacy_mode) {
        get_gnss_data(&states);
    }

    // Allocate memory for ADC data buffer
    states.adc_channel_buffer =
        array_int32_make(states.legacy_mode ? 3 * LEGACY_PACKET_CHANNEL_SIZE
                                            : 3 * states.sample_rate);
    // Pre-allocate memory for UART packet buffer
    if (!states.legacy_mode) {
        states.uart_packet_buffer = array_uint8_make(1);
    }

    // Display device settings
    ssd1306_display_string(0, 0, "Device Started!",
                           SSD1306_FONT_TYPE_ASCII_8X16,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE);
    mcu_utils_delay_ms(1000, false);
    ssd1306_clear();
    display_settings(&states);

    // Create task threads
    const osThreadAttr_t read_adc_attr = {
        .name = "Read ADC",
        .stack_size = 128 * 2,
        .priority = (osPriority_t)osPriorityNormal,
    };
    osThreadNew(task_read_adc, &states, &read_adc_attr);
    const osThreadAttr_t send_data_attr = {
        .name = "Send Data",
        .stack_size = 128 * 4,
        .priority = (osPriority_t)osPriorityNormal,
    };
    osThreadNew(task_send_data, &states, &send_data_attr);
    const osThreadAttr_t calib_gnss_attr = {
        .name = "GNSS Calib",
        .stack_size = 128 * 4,
        .priority = (osPriority_t)osPriorityNormal,
    };
    osThreadNew(task_calib_gnss, &states, &calib_gnss_attr);
    const osThreadAttr_t feed_iwdg_attr = {
        .name = "Feed IWDG",
        .stack_size = 32,
        .priority = (osPriority_t)osPriorityNormal,
    };
    osThreadNew(task_feed_iwdg, NULL, &feed_iwdg_attr);

    // Create RTOS resources
    const osMessageQueueAttr_t reader_drdy_queue_attr = {
        .name = "DRDY Timestamp",
    };
    states.reader_drdy_queue =
        osMessageQueueNew(1, sizeof(int64_t), &reader_drdy_queue_attr);
}

int main(void) {
    HAL_Init();
    SystemClock_Config();

    osKernelInitialize();
    setup();
    osKernelStart();

    while (true) {
        ;
    }
}
