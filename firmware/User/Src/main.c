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
#include "Utils/Inc/iwdg.h"
#include "Utils/Inc/led.h"
#include "Utils/Inc/uart.h"
#include "Utils/Inc/uart2.h"
#include "Utils/Inc/uptime.h"

#include "User/Inc/ssd1306/display.h"
#include "User/Inc/ssd1306/font.h"

#include "User/Inc/eeprom/read.h"

#include "User/Inc/gnss/model.h"
#include "User/Inc/gnss/parse.h"
#include "User/Inc/gnss/time.h"
#include "User/Inc/gnss/utils.h"

#include "User/Inc/array.h"
#include "User/Inc/filter.h"
#include "User/Inc/leveling.h"
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

static volatile explorer_gnss_discipline_t gnss_discipline_status = {0};

static uint8_t gnss_discipline_task_stack[768];
static StaticTask_t gnss_discipline_task_cb;

static uint8_t gnss_acquire_task_stack[1024];
static StaticTask_t gnss_acquire_task_cb;
static osThreadId_t gnss_acquire_task_handle;

static uint8_t send_packet_task_stack[1024];
static StaticTask_t send_packet_task_cb;

static uint8_t sensor_acquire_task_stack[768];
static StaticTask_t sensor_acquire_task_cb;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) {
    if (htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
        uint16_t high_before = __HAL_TIM_GET_COUNTER(&htim2);
        uint16_t low = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1);
        uint16_t high_after = __HAL_TIM_GET_COUNTER(&htim2);

        int64_t updated_at_us = mcu_utils_uptime_get_us();
        uint16_t high;
        if (high_before == high_after) {
            high = high_before;
        } else {
            high = low < 0x8000 ? high_after : high_before;
        }

        gnss_discipline_status.gnss_1pps_tick = ((uint32_t)high << 16) | low;
        gnss_discipline_status.updated_at_us = updated_at_us;
        mcu_utils_uart2_flush();

        osThreadFlagsSet(gnss_acquire_task_handle, GNSS_1PPS_UPDATED);
        osThreadFlagsSet(gnss_discipline_status.task_handle, GNSS_1PPS_UPDATED);
    }
}

void set_tim3_arr(uint32_t clk_freq, uint16_t tick_step, float tick_step_size, float ppm_f, uint32_t delta_us) {
    static float acc_ppm_tick = 0.0f;

    float base_ticks = ((float)(clk_freq * tick_step)) / 1e6f;
    float ideal_arr_f = base_ticks - 1.0f;

    float ppm_tick_per_us = base_ticks * ppm_f / 1e12f;
    acc_ppm_tick += ppm_tick_per_us * (float)delta_us;

    int32_t tick_adjust = 0;
    if (acc_ppm_tick >= tick_step_size) {
        tick_adjust = (int32_t)(tick_step_size + 0.5f);
        acc_ppm_tick -= tick_step_size;
    } else if (acc_ppm_tick <= -tick_step_size) {
        tick_adjust = -(int32_t)(tick_step_size + 0.5f);
        acc_ppm_tick += tick_step_size;
    }

    float new_arr_f = ideal_arr_f + tick_adjust;
    uint32_t new_arr = (uint32_t)(new_arr_f + 0.5f);

    if (new_arr < 1) {
        new_arr = 1;
        acc_ppm_tick = 0;
    } else if (new_arr > 0xFFFF) {
        new_arr = 0xFFFF;
        acc_ppm_tick = 0;
    }

    if (new_arr != htim3.Init.Period) {
        __HAL_TIM_SET_AUTORELOAD(&htim3, new_arr);
        htim3.Instance->EGR = TIM_EGR_UG;
        htim3.Init.Period = new_arr;
    }
}

void task_gnss_discipline(void* argument) {
    explorer_global_states_t* states = (explorer_global_states_t*)argument;
    const uint32_t clk = get_tim3_clk_freq();
    const float clk_f = (float)clk;

    if (states->use_gnss_time) {
        ssd1306_display_string(0, 0, "Parse GNSS Data", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
        ssd1306_display_string(0, 2, "Wait for 1PPS Signal", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
        mcu_utils_gpio_high(MCU_STATE_PIN);
        peri_gnss_init();
        gnss_reset(GNSS_CTL_PIN, true);
        MX_TIM2_Init();
        MX_TIM1_Init();
        HAL_TIM_Base_Start(&htim2);
        HAL_TIM_Base_Start(&htim1);
        HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
    } else {
        mcu_utils_gpio_low(GNSS_CTL_PIN.rst);
    }

    uint32_t prev_gnss_1pps_tick = 0;
    uint64_t prev_updated_at_us = 0;
    uint64_t prev_called_at_us = 0;
    uint8_t ppm_avg_counter = 0;

    for (uint8_t pps_init_counter = 0; states->use_gnss_time;) {
        uint8_t flags = osThreadFlagsWait(GNSS_1PPS_UPDATED, osFlagsWaitAny, 1500);
        uint64_t current_time_us = mcu_utils_uptime_get_us();
        if (flags & 0x01) {
            if (prev_called_at_us == 0) {
                prev_called_at_us = current_time_us;
                continue;
            }

            if (gnss_discipline_status.task_disabled) {
                set_tim3_arr(clk, MCU_UTILS_UPTIME_TICK_STEP_US, 2.0f, gnss_discipline_status.avg_ppm, current_time_us - prev_called_at_us);
                prev_called_at_us = current_time_us;
                continue;
            }

            if (prev_gnss_1pps_tick == 0 || prev_updated_at_us == 0) {
                prev_gnss_1pps_tick = gnss_discipline_status.gnss_1pps_tick;
                prev_updated_at_us = gnss_discipline_status.updated_at_us;
                continue;
            }

            if (pps_init_counter < (PPM_WINDOW_SIZE * 2 + 1)) {
                pps_init_counter++;
                if (pps_init_counter == (PPM_WINDOW_SIZE * 2 + 1)) {
                    gnss_discipline_status.task_disabled = true;
                    osThreadFlagsSet(gnss_acquire_task_handle, GNSS_ACQUIRE_ACT);
                    continue;
                } else {
                    snprintf((char*)states->message_buf, sizeof(states->message_buf), "1PPS Captured: %2d/%2d", pps_init_counter, PPM_WINDOW_SIZE * 2);
                    ssd1306_display_string(0, 2, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
                }
            }

            uint32_t delta = gnss_discipline_status.gnss_1pps_tick - prev_gnss_1pps_tick;
            uint64_t pps_update_diff_ms = (gnss_discipline_status.updated_at_us - prev_updated_at_us) / 1000;

            prev_gnss_1pps_tick = gnss_discipline_status.gnss_1pps_tick;
            prev_updated_at_us = gnss_discipline_status.updated_at_us;

            if (pps_update_diff_ms <= 990 || pps_update_diff_ms >= 1010) {
                prev_called_at_us = current_time_us;
                continue;
            }

            gnss_discipline_status.current_ppm = (((float)delta - clk_f) * 1.0e6f) / clk_f;
            gnss_discipline_status.ppm_window[gnss_discipline_status.ppm_index] = gnss_discipline_status.current_ppm;
            gnss_discipline_status.ppm_index = (gnss_discipline_status.ppm_index + 1) % PPM_WINDOW_SIZE;
            if (ppm_avg_counter < PPM_WINDOW_SIZE) {
                ppm_avg_counter++;
            }
            float ppm_sum = 0.0f;
            for (uint8_t i = 0; i < ppm_avg_counter; i++) {
                ppm_sum += gnss_discipline_status.ppm_window[i];
            }
            gnss_discipline_status.avg_ppm = ppm_sum / (float)ppm_avg_counter;

            set_tim3_arr(clk, MCU_UTILS_UPTIME_TICK_STEP_US, 3.0f, gnss_discipline_status.current_ppm, current_time_us - prev_called_at_us);
            prev_called_at_us = current_time_us;
            mcu_utils_led_blink(MCU_STATE_PIN, 1, true);
        } else {
            set_tim3_arr(clk, MCU_UTILS_UPTIME_TICK_STEP_US, 2.0f, gnss_discipline_status.avg_ppm, current_time_us - prev_called_at_us);
            prev_called_at_us = current_time_us;
            mcu_utils_gpio_high(MCU_STATE_PIN);
        }
    }

    osThreadExit();
}

void task_gnss_acquire(void* argument) {
    explorer_global_states_t* states = (explorer_global_states_t*)argument;

    for (bool first_run = true; states->use_gnss_time;) {
        if (first_run) {
            osThreadFlagsWait(GNSS_ACQUIRE_ACT, osFlagsWaitAny, osWaitForever);
        }

        bool got_valid_fix = false;
        uint8_t consecutive_valid_count = 0;
        int64_t prev_time_diff = INT64_MAX;
        int64_t current_time_diff = INT64_MAX;

        for (int64_t local_timestamp_ms;;) {
            uint8_t flags = osThreadFlagsWait(GNSS_1PPS_UPDATED, osFlagsWaitAny, 1500);
            if (flags & 0x01) {
                local_timestamp_ms = (gnss_discipline_status.updated_at_us + 500) / 1000;
                if (mcu_utils_uptime_get_ms() - local_timestamp_ms >= 1000) {
                    continue;
                }

                gnss_status_t gnss_status;
                gnss_time_t gnss_time;

                bool ok = parse_gnss_message(states->message_buf, &gnss_status, &states->gnss_location, &gnss_time, local_timestamp_ms, &current_time_diff);
                if (!ok) {
                    if (first_run) {
                        ssd1306_display_string(0, 2, "Error Reading GNSS!", SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
                    }
                    continue;
                }

                if (first_run) {
                    snprintf((char*)states->message_buf, sizeof(states->message_buf), "SAT %2d, HDOP %5.1f", gnss_status.satellites, gnss_status.hdop);
                    ssd1306_display_string(0, 2, (char*)states->message_buf, SSD1306_FONT_TYPE_ASCII_8X6, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
                }

                if (gnss_time.is_valid && states->gnss_location.is_valid && gnss_status.satellites > 0 && gnss_status.hdop <= GNSS_REQUIRED_HDOP) {
                    if (prev_time_diff == INT64_MAX || current_time_diff == prev_time_diff) {
                        consecutive_valid_count++;
                    } else {
                        consecutive_valid_count = 1;
                    }
                    prev_time_diff = current_time_diff;

                    if (consecutive_valid_count >= 3) {
                        states->gnss_time_diff = current_time_diff;
                        got_valid_fix = true;
                        if (first_run) {
                            ssd1306_display_string(0, 0, "GNSS Data Valid", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
                            mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
                            mcu_utils_delay_ms(1000, false);
                        }
                        break;
                    }
                }
            } else if (!first_run) {
                got_valid_fix = false;
                break;
            }
        }

        if (got_valid_fix && first_run) {
            first_run = false;
            ssd1306_clear();
            display_device_settings(states);
            osThreadFlagsSet(states->sensor_acquire_task_handle, SENSOR_ACQUIRE_ACT);
        }

        if (!first_run) {
            gnss_discipline_status.task_disabled = false;
            int64_t current_time_ms = (mcu_utils_uptime_get_ms() + states->gnss_time_diff + 777);
            int64_t delay_until_next_hour_ms = 86400000 - (current_time_ms % 86400000);
            mcu_utils_delay_ms(delay_until_next_hour_ms, true);
            gnss_discipline_status.task_disabled = true;
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
                mcu_utils_iwdg_feed();
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
        if (!states->use_accelerometer || states->channel_6d) {
            if (states->channel_6d) {
                acq_msg.timestamp = mcu_utils_uptime_get_ms() + states->gnss_time_diff;
            }
            get_adc_readout(ADS1262_CTL_PIN, states->adc_calibration_offset, acq_msg.adc_data);

#if HARDWARE_REV >= 20250804
            acq_msg.adc_data[0] = apply_data_compensation(acq_msg.adc_data[0], &states->df1_filter_ch1);
            acq_msg.adc_data[1] = apply_data_compensation(acq_msg.adc_data[1], &states->df1_filter_ch2);
            acq_msg.adc_data[2] = apply_data_compensation(acq_msg.adc_data[2], &states->df1_filter_ch3);
#endif
        }

        if (!states->channel_6d) {
            acq_msg.timestamp = mcu_utils_uptime_get_ms() + states->gnss_time_diff;
        }
        if (states->use_accelerometer || states->channel_6d) {
            get_accel_readout(states->accel_lsb_per_g, acq_msg.accel_data);
        }
        get_env_temperature(&acq_msg.temperature);

        osMessageQueuePut(states->sensor_acquisition_queue, &acq_msg, 0, 0);

        uint64_t current_timestamp;
        do {
            current_timestamp = mcu_utils_uptime_get_ms();
        } while (current_timestamp - prev_timestamp < time_span);
        prev_timestamp = current_timestamp;
    }
}

void system_setup(void) {
    MX_DMA_Init();
    MX_TIM3_Init();
    HAL_TIM_Base_Start_IT(&htim3);

    mcu_utils_gpio_init(false);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_gpio_high(MCU_STATE_PIN);

    mcu_utils_i2c_init(false);
    mcu_utils_spi_init(false);
    mcu_utils_uart2_init(GNSS_BAUDRATE, false);

    static explorer_global_states_t states;
    read_device_settings(&states);
    mcu_utils_uart_init(states.baud_rate, false);

    peri_screen_init();
    ssd1306_display_bitmap(0, 0, 128, 8, ANYSHAKE_LOGO_BITMAP_RLE, SSD1306_FONT_DISPLAY_COLOR_WHITE);
#if DEVICE_MODEL == E_C111G
    ssd1306_display_string(0, 0, "E-C111G Init...", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
#elif DEVICE_MODEL == E_C121G
    ssd1306_display_string(0, 0, "E-C121G Init...", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
#endif
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_delay_ms(1000, false);

    peri_eeprom_init();
    eeprom_read((uint8_t*)&states.device_id, 0, sizeof(states.device_id));

    states.adc_calibration_offset = adc_calibration_offset_new();
    uint8_t calib_factors_status = 0;
    eeprom_read((uint8_t*)&calib_factors_status, 4, sizeof(calib_factors_status));
    if (calib_factors_status == 1) {
        eeprom_read(states.adc_calibration_offset.channel_1, 5, sizeof(states.adc_calibration_offset.channel_1));
        eeprom_read(states.adc_calibration_offset.channel_2, 11, sizeof(states.adc_calibration_offset.channel_2));
        eeprom_read(states.adc_calibration_offset.channel_3, 17, sizeof(states.adc_calibration_offset.channel_3));
    } else {
        ssd1306_display_string(0, 0, "No Calib Params", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
        mcu_utils_led_blink(MCU_STATE_PIN, 100, false);
        mcu_utils_delay_ms(1000, false);
    }

#if DEVICE_MODEL == E_C111G
    lsm6ds3_reset(false);
#elif DEVICE_MODEL == E_C121G
    icm42688_reset(false);
#endif
    states.accel_lsb_per_g = peri_imu_init(states.sample_rate);
    mcu_utils_delay_ms(1000, false);

    if (states.leveling_mode) {
        leveling_mode_entry();
    }

    ads1262_init(ADS1262_CTL_PIN, ADS1262_INIT_CONTROL_TYPE_HARD);
    ads1262_reset(ADS1262_CTL_PIN, ADS1262_RESET_RESET_TYPE_HARD, false);
    peri_adc_init(ADS1262_INIT_CONTROL_TYPE_HARD, states.sample_rate, states.channel_6d);
#if HARDWARE_REV >= 20250804
    switch (states.sample_rate) {
        case 50:
            filter_iir_df1_new(&states.df1_filter_ch1, IIR_DF1_B_COEFFS_50_HZ, IIR_DF1_A_COEFFS_50_HZ);
            filter_iir_df1_new(&states.df1_filter_ch2, IIR_DF1_B_COEFFS_50_HZ, IIR_DF1_A_COEFFS_50_HZ);
            filter_iir_df1_new(&states.df1_filter_ch3, IIR_DF1_B_COEFFS_50_HZ, IIR_DF1_A_COEFFS_50_HZ);
            break;
        case 100:
            filter_iir_df1_new(&states.df1_filter_ch1, IIR_DF1_B_COEFFS_100_HZ, IIR_DF1_A_COEFFS_100_HZ);
            filter_iir_df1_new(&states.df1_filter_ch2, IIR_DF1_B_COEFFS_100_HZ, IIR_DF1_A_COEFFS_100_HZ);
            filter_iir_df1_new(&states.df1_filter_ch3, IIR_DF1_B_COEFFS_100_HZ, IIR_DF1_A_COEFFS_100_HZ);
            break;
        case 200:
            filter_iir_df1_new(&states.df1_filter_ch1, IIR_DF1_B_COEFFS_200_HZ, IIR_DF1_A_COEFFS_200_HZ);
            filter_iir_df1_new(&states.df1_filter_ch2, IIR_DF1_B_COEFFS_200_HZ, IIR_DF1_A_COEFFS_200_HZ);
            filter_iir_df1_new(&states.df1_filter_ch3, IIR_DF1_B_COEFFS_200_HZ, IIR_DF1_A_COEFFS_200_HZ);
            break;
        case 250:
            filter_iir_df1_new(&states.df1_filter_ch1, IIR_DF1_B_COEFFS_250_HZ, IIR_DF1_A_COEFFS_250_HZ);
            filter_iir_df1_new(&states.df1_filter_ch2, IIR_DF1_B_COEFFS_250_HZ, IIR_DF1_A_COEFFS_250_HZ);
            filter_iir_df1_new(&states.df1_filter_ch3, IIR_DF1_B_COEFFS_250_HZ, IIR_DF1_A_COEFFS_250_HZ);
            break;
    }
#endif

    states.packet_sending_interval = PACKET_SENDING_INTERVAL;
    states.channel_chunk_length = states.packet_sending_interval / (1000 / states.sample_rate);
    states.uart_packet_buffer = array_uint8_make(get_data_packet_size(states.use_accelerometer, states.channel_6d, states.channel_chunk_length));

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

    const static osThreadAttr_t gnss_discipline_task_attr = {
        .name = "gnss_discipline",
        .cb_mem = &gnss_discipline_task_cb,
        .cb_size = sizeof(gnss_discipline_task_cb),
        .stack_mem = gnss_discipline_task_stack,
        .stack_size = sizeof(gnss_discipline_task_stack),
        .priority = osPriorityNormal,
    };
    gnss_discipline_status.task_handle = osThreadNew(task_gnss_discipline, &states, &gnss_discipline_task_attr);
    if (gnss_discipline_status.task_handle == NULL) {
        ssd1306_display_string(0, 0, "Error Code 0x1", SSD1306_FONT_TYPE_ASCII_8X16, SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
        mcu_utils_gpio_high(MCU_STATE_PIN);
        while (1) {
            ;
        }
    }

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
        mcu_utils_gpio_high(MCU_STATE_PIN);
        while (1) {
            ;
        }
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
        mcu_utils_gpio_high(MCU_STATE_PIN);
        while (1) {
            ;
        }
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
        mcu_utils_gpio_high(MCU_STATE_PIN);
        while (1) {
            ;
        }
    }
}

int main(void) {
    SystemClock_Config();
    HAL_Init();

    osKernelInitialize();
    system_setup();
    osKernelStart();

    while (1) {
        ;
    }
}
