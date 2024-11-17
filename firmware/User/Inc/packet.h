#ifndef __PACKET_H
#define __PACKET_H

#include <stdbool.h>
#include <stdint.h>

#include "User/Inc/array.h"
#include "User/Inc/gnss/utils.h"
#include "Utils/Inc/delay.h"
#include "Utils/Inc/uart.h"

#define LEGACY_PACKET_CHANNEL_SAMPLES 5
#define MAINLINE_PACKET_CHANNEL_SAMPLES 5

typedef struct {
    int32_t z_axis[LEGACY_PACKET_CHANNEL_SAMPLES];
    int32_t e_axis[LEGACY_PACKET_CHANNEL_SAMPLES];
    int32_t n_axis[LEGACY_PACKET_CHANNEL_SAMPLES];
    uint8_t checksum[3];
} legacy_data_packet_t;

void send_legacy_data_packet(int32_array_t* channel_buf,
                             uint8_t channel_samples);
void send_data_packet(int32_array_t* channel_buf,
                      uint8_array_t* uart_buf,
                      int64_t timestamp,
                      gnss_location_t* location,
                      uint32_t device_info,
                      uint16_t channel_samples);
uint8_t get_data_packet_size(uint8_t channel_samples);

#endif
