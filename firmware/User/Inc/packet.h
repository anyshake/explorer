#ifndef __PACKET_H
#define __PACKET_H

#include <stdbool.h>
#include <stdint.h>

#include "User/Inc/array.h"
#include "Utils/Inc/crc32.h"
#include "Utils/Inc/delay.h"
#include "Utils/Inc/uart.h"

#define LEGACY_PACKET_CHANNEL_SIZE 5

typedef struct {
    int32_t z_axis[LEGACY_PACKET_CHANNEL_SIZE];
    int32_t e_axis[LEGACY_PACKET_CHANNEL_SIZE];
    int32_t n_axis[LEGACY_PACKET_CHANNEL_SIZE];
    uint8_t checksum[3];
} legacy_data_packet_t;

void send_legacy_data_packet(int32_array_t* channel_buf);
void send_data_packet(int32_array_t* channel_buf,
                      uint8_array_t* packet_buf,
                      int64_t timestamp,
                      uint16_t sample_rate,
                      uint32_t device_id);

#endif
