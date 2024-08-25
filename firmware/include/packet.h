#ifndef __PACKET_H
#define __PACKET_H

#include <stdbool.h>
#include <stdint.h>

#include "array.h"
#include "utils/delay.h"
#include "utils/uart.h"

#define EXPLORER_CHANNEL_SIZE 5

typedef struct {
    int32_t z_axis[EXPLORER_CHANNEL_SIZE];
    int32_t e_axis[EXPLORER_CHANNEL_SIZE];
    int32_t n_axis[EXPLORER_CHANNEL_SIZE];
    uint8_t checksum[3];
} data_packet_t;

void send_data_packet(int32_array_t* channel_buf);

#endif
