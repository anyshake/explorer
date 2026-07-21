#ifndef __TYPES_H
#define __TYPES_H

#include <stdbool.h>
#include <stdint.h>

#include "User/Inc/array.h"
#include "User/Inc/filter.h"
#include "User/Inc/gnss/utils.h"
#include "cmsis_os2.h"

#define E_C111G 0
#define E_C121G 1

typedef struct {
    volatile float current_ppm;
    volatile uint64_t monotonic_time_us;
    volatile uint32_t actual_freq_hz;
    osThreadId_t task_handle;
} explorer_time_sync_t;

typedef struct {
    int64_t timestamp;
    int32_t adc_data[3];
    int16_t accel_data[3];
    float temperature;
} explorer_acquisition_message_t;

typedef struct {
    uint8_t channel_1[3];
    uint8_t channel_2[3];
    uint8_t channel_3[3];
} explorer_adc_calibration_offset_t;

typedef struct {
    bool leveling_mode;
    bool gnss_debug_mode;

    uint16_t accel_lsb_per_g;
    uint32_t device_id;

    uint32_t baud_rate;
    uint8_t sample_rate;

    bool use_accelerometer;
    bool use_gnss_time;
    bool channel_6d;

    gnss_location_t gnss_location;
    uint8_t message_buf[GNSS_SENTENCE_BUFFER_SIZE];

    explorer_adc_calibration_offset_t adc_calibration_offset;
    int32_array_t* adc_acquisition_channel_buffer;
    int16_array_t* accel_acquisition_channel_buffer;

    filter_iir_df1_t df1_filter_ch1;
    filter_iir_df1_t df1_filter_ch2;
    filter_iir_df1_t df1_filter_ch3;

    uint8_t channel_chunk_length;
    uint16_t packet_sending_interval;
    int64_t prev_packet_sent_at_ms;
    uint8_array_t* uart1_packet_buffer;

    osMessageQueueId_t sensor_acquisition_queue;
    osThreadId_t send_packet_task_handle;
    osThreadId_t sensor_acquire_task_handle;
} explorer_global_states_t;

enum explorer_thread_flags_t {
    GNSS_1PPS_UPDATED = 0x01,
    SENSOR_ACQUIRE_ACT,
};

#endif
