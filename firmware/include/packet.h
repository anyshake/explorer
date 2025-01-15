#ifndef __PACKET_H
#define __PACKET_H

#include <stdbool.h>
#include <stdint.h>

#include "array.h"
#include "utils/delay.h"
#include "utils/uart.h"

#define MAINLINE_PACKET_CHANNEL_SAMPLES 5

void send_data_packet(int64_t timestamp,
                      int32_t adc_readout_z_axis[],
                      int32_t adc_readout_e_axis[],
                      int32_t adc_readout_n_axis[],
                      uint16_t channel_samples,
                      uint8_array_t* uart_packet_buffer);
uint8_t get_data_packet_size(uint8_t channel_samples);

#endif
