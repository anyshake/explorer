#ifndef __PACKET_H
#define __PACKET_H

#include <stdbool.h>
#include <stdint.h>

#include "array.h"
#include "utils/uart.h"
#include "utils/crc32.h"
#include "utils/delay.h"

void send_data_packet(int32_array_t* channel_buf,
                      uint8_array_t* packet_buf,
                      uint16_t sample_rate,
                      uint32_t device_id);
uint16_t get_data_packet_size(uint16_t sample_rate);

#endif
