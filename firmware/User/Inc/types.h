#ifndef __TYPES_H
#define __TYPES_H

#include <stdbool.h>
#include <stdint.h>

#include "User/Inc/array.h"
#include "User/Inc/gnss/utils.h"
#include "cmsis_os2.h"

typedef struct {
    bool leveling_mode;

    uint32_t device_id;
    uint32_t baud_rate;
    uint8_t sample_rate;
    bool use_accelerometer;
    bool use_gnss_time;
    bool channel_6d;

    int64_t gnss_ref_timestamp;
    int64_t local_base_timestamp;

    uint8_t gnss_message_buffer[GNSS_SENTENCE_BUFFER_SIZE];
    gnss_location_t gnss_location;
    gnss_status_t gnss_status;
    gnss_time_t gnss_time;

    osMessageQueueId_t acquisition_data_queue;
    int32_array_t* adc_acquisition_channel_buffer;
    int16_array_t* accel_acquisition_channel_buffer;

    uint8_t channel_chunk_length;
    uint8_array_t* uart_packet_buffer;
} explorer_states_t;

typedef struct {
    int64_t timestamp;
    int32_t adc_data[3];
    int16_t accel_data[3];
    float temperature;
} acquisition_message_t;

#endif
