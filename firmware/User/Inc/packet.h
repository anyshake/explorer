#ifndef __PACKET_H
#define __PACKET_H

#include <stdbool.h>
#include <stdint.h>

#include "User/Inc/array.h"
#include "Utils/Inc/delay.h"
#include "Utils/Inc/uart.h"

#define LEGACY_DATA_PACKET_SIZE 4

typedef struct {
    int32_t z_axis[LEGACY_DATA_PACKET_SIZE];
    int32_t e_axis[LEGACY_DATA_PACKET_SIZE];
    int32_t n_axis[LEGACY_DATA_PACKET_SIZE];
    uint8_t checksum[3];
} legacy_data_packet_t;

void send_legacy_data_packet(array_int32_t* channel_buf,
                             int64_t timestamp,
                             bool is_rtos);
void send_data_packet(array_int32_t* channel_buf,
                      int64_t timestamp,
                      uint8_t sample_rate);

#endif
