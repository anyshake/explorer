#include "packet.h"

void send_data_packet(int64_t timestamp,
                      int32_t adc_readout_z_axis[],
                      int32_t adc_readout_e_axis[],
                      int32_t adc_readout_n_axis[],
                      uint16_t channel_samples,
                      uint8_array_t* uart_packet_buffer) {
    if (adc_readout_z_axis == NULL || adc_readout_e_axis == NULL ||
        adc_readout_n_axis == NULL || uart_packet_buffer == NULL) {
        return;
    }

    uart_packet_buffer->data[0] = 0xFA;
    uart_packet_buffer->data[1] = 0xDE;

    uint8_t* bytes = (uint8_t*)&timestamp;
    for (uint8_t i = 0; i < sizeof(timestamp); i++) {
        uart_packet_buffer->data[2 + i] = bytes[i];
    }

    uint32_t dummy_val = 19890604;  // Fixed value for Explorer Gen 1
    bytes = (uint8_t*)&dummy_val;
    for (uint8_t i = 0; i < sizeof(dummy_val); i++) {
        uart_packet_buffer->data[10 + i] = bytes[i];
    }

    for (uint8_t i = 0; i < channel_samples; i++) {
        bytes = (uint8_t*)&adc_readout_z_axis[i];
        for (uint8_t j = 0; j < sizeof(int32_t); j++) {
            uart_packet_buffer->data[14 + i * sizeof(int32_t) + j] = bytes[j];
        }
        bytes = (uint8_t*)&adc_readout_e_axis[i];
        for (uint8_t j = 0; j < sizeof(int32_t); j++) {
            uart_packet_buffer->data[14 + channel_samples * sizeof(int32_t) + i * sizeof(int32_t) + j] = bytes[j];
        }
        bytes = (uint8_t*)&adc_readout_n_axis[i];
        for (uint8_t j = 0; j < sizeof(int32_t); j++) {
            uart_packet_buffer->data[14 + 2 * channel_samples * sizeof(int32_t) + i * sizeof(int32_t) + j] = bytes[j];
        }
    }

    uint8_t xor_checksum = 0;
    for (uint8_t i = 2; i < uart_packet_buffer->size - sizeof(xor_checksum); i++) {
        xor_checksum ^= uart_packet_buffer->data[i];
    }
    uart_packet_buffer->data[uart_packet_buffer->size - sizeof(xor_checksum)] = xor_checksum;

    mcu_utils_uart_write(uart_packet_buffer->data, uart_packet_buffer->size, false);
}

uint8_t get_data_packet_size(uint8_t channel_samples) {
    return  // Frame header [0:2]
        sizeof(uint8_t) + sizeof(uint8_t) +
        // Unix timestamp [2:10]
        sizeof(int64_t) +
        // Variable data [10:14]
        sizeof(uint32_t) +  // Latitude, longitude, elevation and device ID
        // Z-axis data [14:channel_samples*4]
        channel_samples * sizeof(int32_t) +
        // E-axis data [channel_samples*4:2*channel_samples*4]
        channel_samples * sizeof(int32_t) +
        // N-axis data [2*channel_samples*4:3*channel_samples*4]
        channel_samples * sizeof(int32_t) +
        // Packet checksum [3*channel_samples*4:3*channel_samples*4+1]
        sizeof(uint8_t);
}
