#include "packet.h"

void send_legacy_data_packet(int32_array_t* channel_buf,
                             uint8_t channel_samples) {
    if (channel_buf == NULL) {
        return;
    }
    data_packet_t packet = {0};

    // Set packet data from channel buffer
    for (uint8_t i = 0; i < channel_samples; i++) {
        packet.z_axis[i] = channel_buf->data[i];
        packet.e_axis[i] = channel_buf->data[i + channel_samples];
        packet.n_axis[i] = channel_buf->data[i + 2 * channel_samples];
    }

    // Calculate checksums for each channel
    for (uint8_t i = 0; i < channel_samples; i++) {
        uint8_t* bytes = (uint8_t*)&packet.z_axis[i];
        for (uint8_t j = 0; j < sizeof(int32_t); j++) {
            packet.checksum[0] ^= bytes[j];
        }
        bytes = (uint8_t*)&packet.e_axis[i];
        for (uint8_t j = 0; j < sizeof(int32_t); j++) {
            packet.checksum[1] ^= bytes[j];
        }
        bytes = (uint8_t*)&packet.n_axis[i];
        for (uint8_t j = 0; j < sizeof(int32_t); j++) {
            packet.checksum[2] ^= bytes[j];
        }
    }

    uint8_t frame_header[] = {0xFC, 0x1B};
    mcu_utils_uart_write(frame_header, sizeof(frame_header), true);
    mcu_utils_uart_write((uint8_t*)&packet, sizeof(data_packet_t), true);
}

void send_data_packet(int32_array_t* channel_buf,
                      uint8_array_t* uart_buf,
                      int64_t timestamp,
                      uint16_t channel_samples) {
    if (channel_buf == NULL || uart_buf == NULL) {
        return;
    }

    // Set packet frame header field
    uart_buf->data[0] = 0xFA;
    uart_buf->data[1] = 0xDE;

    // Set packet timestamp
    uint8_t* bytes = (uint8_t*)&timestamp;
    for (uint8_t i = 0; i < sizeof(timestamp); i++) {
        uart_buf->data[2 + i] = bytes[i];
    }

    // Set variable data fields to dummy value
    uint32_t dummy_val = 19890604;  // Fixed value
    bytes = (uint8_t*)&dummy_val;
    for (uint8_t i = 0; i < sizeof(dummy_val); i++) {
        uart_buf->data[10 + i] = bytes[i];
    }

    // Set packet data from channel buffer (int32_t to uint8_t)
    for (uint8_t i = 0; i < channel_samples; i++) {
        bytes = (uint8_t*)&channel_buf->data[i];
        for (uint8_t j = 0; j < sizeof(int32_t); j++) {
            uart_buf->data[14 + i * sizeof(int32_t) + j] = bytes[j];
        }
        bytes = (uint8_t*)&channel_buf->data[i + channel_samples];
        for (uint8_t j = 0; j < sizeof(int32_t); j++) {
            uart_buf->data[14 + channel_samples * sizeof(int32_t) +
                           i * sizeof(int32_t) + j] = bytes[j];
        }
        bytes = (uint8_t*)&channel_buf->data[i + 2 * channel_samples];
        for (uint8_t j = 0; j < sizeof(int32_t); j++) {
            uart_buf->data[14 + 2 * channel_samples * sizeof(int32_t) +
                           i * sizeof(int32_t) + j] = bytes[j];
        }
    }

    // Get checksum for packet
    uint8_t xor_checksum = 0;
    for (uint8_t i = 2; i < uart_buf->size - sizeof(xor_checksum); i++) {
        xor_checksum ^= uart_buf->data[i];
    }
    uart_buf->data[uart_buf->size - sizeof(xor_checksum)] = xor_checksum;

    mcu_utils_uart_write(uart_buf->data, uart_buf->size, false);
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
