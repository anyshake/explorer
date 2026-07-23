#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Core/Inc/dma.h"
#include "Core/Inc/main.h"
#include "Core/Inc/tim.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"

#include "Utils/Inc/delay.h"
#include "Utils/Inc/gpio.h"
#include "Utils/Inc/i2c1.h"
#include "Utils/Inc/iwdg.h"
#include "Utils/Inc/led.h"
#include "Utils/Inc/spi1.h"
#include "Utils/Inc/systime.h"
#include "Utils/Inc/uart/uart1.h"
#include "Utils/Inc/uart/uart2.h"

#include "User/Inc/ssd1306/display.h"
#include "User/Inc/ssd1306/font.h"

#include "User/Inc/eeprom/read.h"

#include "User/Inc/gnss/model.h"
#include "User/Inc/gnss/parse.h"
#include "User/Inc/gnss/pps.h"
#include "User/Inc/gnss/utils.h"

#include "User/Inc/array.h"
#include "User/Inc/filter.h"
#include "User/Inc/mode.h"
#include "User/Inc/packet.h"
#include "User/Inc/peripheral.h"
#include "User/Inc/reader.h"
#include "User/Inc/settings.h"
#include "User/Inc/types.h"
#include "User/Inc/utils.h"

#ifndef FW_BUILD
#define FW_BUILD "unknownbuild"
#endif

#ifndef FW_REV
#define FW_REV "custombuild"
#endif

static volatile explorer_time_sync_t time_sync_status = {0};

static uint8_t gnss_acquire_task_stack[1024];
static StaticTask_t gnss_acquire_task_cb;
static osThreadId_t gnss_acquire_task_handle;

static uint8_t send_packet_task_stack[1024];
static StaticTask_t send_packet_task_cb;

static uint8_t sensor_acquire_task_stack[768];
static StaticTask_t sensor_acquire_task_cb;

void handle_incoming_time_sync_data(uint64_t monotonic_time_us, uint32_t actual_freq_hz) {
    mcu_utils_uart2_flush();
    mcu_utils_systime_calib_freq(actual_freq_hz);

    uint32_t nominal_freq_hz = mcu_utils_systime_get_nominal_freq_hz();
    int64_t freq_diff_hz = (int64_t)actual_freq_hz - (int64_t)nominal_freq_hz;

    time_sync_status.monotonic_time_us = monotonic_time_us;
    time_sync_status.actual_freq_hz = actual_freq_hz;
    time_sync_status.current_ppm = ((float)freq_diff_hz * 1.0e6f) / (float)nominal_freq_hz;

    if (gnss_acquire_task_handle != NULL) {
        osThreadFlagsSet(gnss_acquire_task_handle, GNSS_1PPS_UPDATED);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t pin) {
    if (pin == GPIO_PIN_8) {
        gnss_pps_handle_timer_interrupt(handle_incoming_time_sync_data);
    }
}

void task_gnss_acquire(void* argument) {
    explorer_global_states_t* states = (explorer_global_states_t*)argument;

    if (states->use_gnss_time) {
        ssd1306_display_string(0, 0, "Parse GNSS Data", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
        ssd1306_display_string(0, 2, "Wait for 1PPS Signal", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
        mcu_utils_gpio_high(MCU_STATE_PIN);
    } else {
        mcu_utils_gpio_low(GNSS_CTL_PIN.rst);
    }

    for (bool first_sync = true; states->use_gnss_time;) {
        bool show_asterisk_symbol = true;
        bool got_valid_fix = false;
        uint8_t consecutive_valid_count = 0;
        int64_t prev_time_diff = INT64_MAX;
        int64_t current_time_diff = INT64_MAX;

        for (int64_t local_timestamp_us = 0;;) {
            uint32_t flags = osThreadFlagsWait(GNSS_1PPS_UPDATED, osFlagsWaitAny, 1500);
            if (flags & GNSS_1PPS_UPDATED) {
                __disable_irq();
                local_timestamp_us = (int64_t)time_sync_status.monotonic_time_us;
                float current_ppm = time_sync_status.current_ppm;
                __enable_irq();

                if (mcu_utils_systime_get_uptime_us() - local_timestamp_us >= 500 * 1000) {
                    continue;
                }

                gnss_status_t gnss_status;
                gnss_time_t gnss_time;
                gnss_location_t gnss_location;

                bool ok = parse_gnss_message(states->message_buf, &gnss_status, &gnss_location, &gnss_time, local_timestamp_us, &current_time_diff, true);
                if (!ok) {
                    if (first_sync) {
                        ssd1306_display_string(0, 2, "Timeout Reading GNSS!", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
                    }
                    continue;
                }

                if (first_sync) {
                    char formatted_value[16];
                    format_float_fixed(formatted_value, sizeof(formatted_value), gnss_status.hdop, 1);
                    snprintf((char*)states->message_buf, sizeof(states->message_buf), "%c SAT %2d; HDOP %5s", show_asterisk_symbol ? '*' : ' ', gnss_status.satellites, formatted_value);
                    ssd1306_display_string(0, 2, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
                    show_asterisk_symbol = !show_asterisk_symbol;

                    format_float_fixed(formatted_value, sizeof(formatted_value), current_ppm, 2);
                    snprintf((char*)states->message_buf, sizeof(states->message_buf), "Clock Dev: %s ppm", formatted_value);
                    ssd1306_display_string(0, 3, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
                }

                if (gnss_time.is_valid && gnss_status.satellites > 0) {
                    if (prev_time_diff == INT64_MAX || current_time_diff == prev_time_diff) {
                        consecutive_valid_count++;
                    } else {
                        consecutive_valid_count = 1;
                    }
                    prev_time_diff = current_time_diff;

                    if (consecutive_valid_count >= 10) {
                        consecutive_valid_count = 0;
                        got_valid_fix = true;
                        mcu_utils_systime_set_offset(current_time_diff);
                        if (gnss_location.is_valid && gnss_status.hdop <= GNSS_REQUIRED_VALID_HDOP) {
                            states->gnss_location = gnss_location;
                            if (first_sync) {
                                ssd1306_display_string(0, 0, "GNSS Data Valid", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
                                mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
                                mcu_utils_delay_ms(1000, true);
                            }
                            break;
                        } else if (!first_sync) {
                            break;
                        }
                    }
                }

                mcu_utils_led_blink(MCU_STATE_PIN, 1, true);
            } else if (!first_sync) {
                got_valid_fix = false;
                break;
            }
        }

        if (got_valid_fix && first_sync) {
            first_sync = false;
            ssd1306_clear();
            display_device_settings(states);
            osThreadFlagsSet(states->sensor_acquire_task_handle, SENSOR_ACQUIRE_ACT);
        }

        if (!first_sync) {
            int64_t current_time_ms = mcu_utils_systime_get_unix_time_ms();
            int64_t delay_until_next_sync_ms = GNSS_RESYNC_INTERVAL_MS - (current_time_ms % GNSS_RESYNC_INTERVAL_MS);
            mcu_utils_delay_ms(delay_until_next_sync_ms, true);
        }
    }

    ssd1306_clear();
    display_device_settings(states);
    osThreadFlagsSet(states->sensor_acquire_task_handle, SENSOR_ACQUIRE_ACT);
    osThreadExit();
}

void task_send_packet(void* argument) {
    explorer_global_states_t* states = (explorer_global_states_t*)argument;

    explorer_acquisition_message_t acq_msg;
    int64_t message_timestamp = 0;

    for (uint8_t message_idx = 0;;) {
        if (osMessageQueueGet(states->sensor_acquisition_queue, &acq_msg, NULL, osWaitForever) == osOK) {
            if (message_idx == 0) {
                message_timestamp = acq_msg.timestamp;
            }

            uint8_t message_idx_offset1 = message_idx + states->channel_chunk_length;
            uint8_t message_idx_offset2 = message_idx_offset1 + states->channel_chunk_length;

            if (!states->use_accelerometer || states->channel_6d) {
                states->adc_acquisition_channel_buffer->data[message_idx] = acq_msg.adc_data[0];
                states->adc_acquisition_channel_buffer->data[message_idx_offset1] = acq_msg.adc_data[1];
                states->adc_acquisition_channel_buffer->data[message_idx_offset2] = acq_msg.adc_data[2];
            }

            if (states->use_accelerometer || states->channel_6d) {
                states->accel_acquisition_channel_buffer->data[message_idx] = acq_msg.accel_data[0];
                states->accel_acquisition_channel_buffer->data[message_idx_offset1] = acq_msg.accel_data[1];
                states->accel_acquisition_channel_buffer->data[message_idx_offset2] = acq_msg.accel_data[2];
            }

            message_idx++;
            if (message_idx == states->channel_chunk_length) {
                send_data_packet(states, acq_msg.temperature, message_timestamp);
                message_idx = 0;

                if (states->prev_packet_sent_at_ms == 0 || message_timestamp - states->prev_packet_sent_at_ms == states->packet_sending_interval) {
                    mcu_utils_iwdg_feed();
                }

                states->prev_packet_sent_at_ms = message_timestamp;
            }
        }
    }
}

void task_sensor_acquire(void* argument) {
    explorer_global_states_t* states = (explorer_global_states_t*)argument;

    explorer_acquisition_message_t acq_msg;
    uint32_t time_span = 1000 / states->sample_rate;

    osThreadFlagsWait(SENSOR_ACQUIRE_ACT, osFlagsWaitAny, osWaitForever);
    mcu_utils_iwdg_init();

    for (uint32_t prev_timestamp = 0;;) {
        int64_t temp_timestamp_us = 0;

        if (!states->use_accelerometer || states->channel_6d) {
            if (states->channel_6d) {
                temp_timestamp_us = mcu_utils_systime_get_unix_time_us();
            }
            get_adc_readout(ADS1262_CTL_PIN, states->adc_calibration_offset, acq_msg.adc_data);
            if (states->channel_6d) {
                acq_msg.timestamp = ((temp_timestamp_us + mcu_utils_systime_get_unix_time_us()) / 2) / 1000;
            }
        }

        if (!states->channel_6d) {
            temp_timestamp_us = mcu_utils_systime_get_unix_time_us();
        }
        if (states->use_accelerometer || states->channel_6d) {
            get_accel_readout(states->accel_lsb_per_g, acq_msg.accel_data);
        }
        if (!states->channel_6d) {
            acq_msg.timestamp = ((temp_timestamp_us + mcu_utils_systime_get_unix_time_us()) / 2) / 1000;
        }
        get_env_temperature(&acq_msg.temperature);

#if HARDWARE_REV >= 20250804
        if (!states->use_accelerometer || states->channel_6d) {
            acq_msg.adc_data[0] = apply_data_compensation(acq_msg.adc_data[0], &states->df1_filter_ch1);
            acq_msg.adc_data[1] = apply_data_compensation(acq_msg.adc_data[1], &states->df1_filter_ch2);
            acq_msg.adc_data[2] = apply_data_compensation(acq_msg.adc_data[2], &states->df1_filter_ch3);
        }
#endif
        osMessageQueuePut(states->sensor_acquisition_queue, &acq_msg, 0, 0);

        uint64_t current_timestamp;
        do {
            current_timestamp = mcu_utils_systime_get_uptime_ms();
        } while (current_timestamp - prev_timestamp < time_span);
        prev_timestamp = current_timestamp;
    }
}

void system_setup(void) {
    mcu_utils_systime_init();

    mcu_utils_gpio_init(false);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_gpio_high(MCU_STATE_PIN);

    static explorer_global_states_t states;
    read_device_settings(&states);

    mcu_utils_i2c1_init(false);
    mcu_utils_spi1_init(false);
    mcu_utils_uart1_init(states.baud_rate, false);
    mcu_utils_uart2_init(GNSS_BAUDRATE, false);

    peri_screen_init();
    ssd1306_display_bitmap(0, 0, 128, 8, ANYSHAKE_LOGO_BITMAP_RLE, SSD1306_FONT_DISPLAY_COLOR_WHITE);
#if DEVICE_MODEL == E_C111G
    ssd1306_display_string(0, 0, "E-C111G Init...", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
#elif DEVICE_MODEL == E_C121G
    ssd1306_display_string(0, 0, "E-C121G Init...", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
#endif
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_delay_ms(1000, false);

    peri_gnss_init();
    gnss_reset(GNSS_CTL_PIN, false);
    gnss_model_setup(false);
    if (states.gnss_debug_mode) {
        gnss_model_factory_reset(false);
        mode_entry_gnss_debug(&states);
    }

    peri_eeprom_init();
    eeprom_read((uint8_t*)&states.device_id, 0, sizeof(states.device_id));

    states.adc_calibration_offset = adc_calibration_offset_new();
    uint8_t calib_factors_status = 0;
    eeprom_read(&calib_factors_status, 4, sizeof(calib_factors_status));
    if (calib_factors_status == 1) {
        eeprom_read(states.adc_calibration_offset.channel_1, 5, sizeof(states.adc_calibration_offset.channel_1));
        eeprom_read(states.adc_calibration_offset.channel_2, 11, sizeof(states.adc_calibration_offset.channel_2));
        eeprom_read(states.adc_calibration_offset.channel_3, 17, sizeof(states.adc_calibration_offset.channel_3));
    } else {
        ssd1306_display_string(0, 0, "No Calib Params", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
        mcu_utils_led_blink(MCU_STATE_PIN, 50, false);
        mcu_utils_delay_ms(1000, false);
    }

#if DEVICE_MODEL == E_C111G
#if HARDWARE_REV >= 20250804
    lsm6dsr_reset(false);
#else
    lsm6ds3_reset(false);
#endif
#elif DEVICE_MODEL == E_C121G
    icm42688_reset(false);
#endif
    states.accel_lsb_per_g = peri_imu_init(states.sample_rate);
    if (states.leveling_mode) {
        mode_entry_leveling(&states);
    }
    mcu_utils_delay_ms(1000, false);

    ads1262_init(ADS1262_CTL_PIN, ADS1262_INIT_CONTROL_TYPE_HARD);
    ads1262_reset(ADS1262_CTL_PIN, ADS1262_RESET_RESET_TYPE_HARD, false);
    if (states.adc_calibration_mode) {
        mode_entry_adc_calibration();
    }
    peri_adc_init(ADS1262_INIT_CONTROL_TYPE_HARD, states.sample_rate, states.channel_6d);

    states.packet_sending_interval = PACKET_SENDING_INTERVAL;
    states.channel_chunk_length = states.packet_sending_interval / (1000 / states.sample_rate);
    states.uart1_packet_buffer = array_uint8_make(get_data_packet_size(states.use_accelerometer, states.channel_6d, states.channel_chunk_length));

    if (!states.use_accelerometer || states.channel_6d) {
        states.adc_acquisition_channel_buffer = array_int32_make(states.channel_chunk_length * 3);
    }
    if (states.use_accelerometer || states.channel_6d) {
        states.accel_acquisition_channel_buffer = array_int16_make(states.channel_chunk_length * 3);
    }

    ssd1306_display_string(0, 0, "Starting Tasks", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_delay_ms(1000, false);

    const static osMessageQueueAttr_t sensor_acquisition_queue_attr = {.name = "sensor_acquisition_data"};
    states.sensor_acquisition_queue = osMessageQueueNew(1, sizeof(explorer_acquisition_message_t), &sensor_acquisition_queue_attr);

    const static osThreadAttr_t gnss_acquire_task_attr = {
        .name = "gnss_acquire",
        .cb_mem = &gnss_acquire_task_cb,
        .cb_size = sizeof(gnss_acquire_task_cb),
        .stack_mem = gnss_acquire_task_stack,
        .stack_size = sizeof(gnss_acquire_task_stack),
        .priority = osPriorityNormal,
    };
    gnss_acquire_task_handle = osThreadNew(task_gnss_acquire, &states, &gnss_acquire_task_attr);
    if (gnss_acquire_task_handle == NULL) {
        ssd1306_display_string(0, 0, "Error Code 0x2", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
        Error_Handler();
    }

    const static osThreadAttr_t send_packet_task_attr = {
        .name = "send_packet",
        .cb_mem = &send_packet_task_cb,
        .cb_size = sizeof(send_packet_task_cb),
        .stack_mem = send_packet_task_stack,
        .stack_size = sizeof(send_packet_task_stack),
        .priority = osPriorityNormal,
    };
    states.send_packet_task_handle = osThreadNew(task_send_packet, &states, &send_packet_task_attr);
    if (states.send_packet_task_handle == NULL) {
        ssd1306_display_string(0, 0, "Error Code 0x3", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
        Error_Handler();
    }

    const static osThreadAttr_t sensor_acquire_task_attr = {
        .name = "sensor_acquire",
        .cb_mem = &sensor_acquire_task_cb,
        .cb_size = sizeof(sensor_acquire_task_cb),
        .stack_mem = sensor_acquire_task_stack,
        .stack_size = sizeof(sensor_acquire_task_stack),
        .priority = osPriorityNormal,
    };
    states.sensor_acquire_task_handle = osThreadNew(task_sensor_acquire, &states, &sensor_acquire_task_attr);
    if (states.sensor_acquire_task_handle == NULL) {
        ssd1306_display_string(0, 0, "Error Code 0x4", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
        Error_Handler();
    }
}

int main(void) {
    SystemClock_Config();
    HAL_Init();
    MX_DMA_Init();

    osKernelInitialize();
    system_setup();
    osKernelStart();

    while (1) {
        ;
    }
}
