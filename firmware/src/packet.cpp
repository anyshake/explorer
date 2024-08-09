#include "packet.h"

void send_data_packet(int32_array_t* channel_buf,
                      uint8_array_t* packet_buf,
                      uint16_t sample_rate,
                      uint32_t device_id) {
    if (channel_buf == NULL || packet_buf == NULL) {
        return;
    }

    // Set packet reserved fields to 0xFF
    for (uint8_t i = 0; i < sizeof(uint64_t); i++) {
        packet_buf->data[28 + i] = 0xFF;
        packet_buf->data[packet_buf->size -
                         // Frame tail
                         sizeof(uint8_t) - sizeof(uint8_t) -
                         // Tail checksum
                         sizeof(uint8_t) -
                         // Reserved
                         sizeof(uint64_t) + i] = 0xFF;
    }

    // Set packet frame header field
    packet_buf->data[0] = 0xF1;
    packet_buf->data[1] = 0xD9;

    // Set packet sample rate
    uint8_t* bytes = (uint8_t*)&sample_rate;
    for (uint8_t i = 0; i < sizeof(uint16_t); i++) {
        packet_buf->data[2 + i] = bytes[i];
    }

    // Set packet timestamp to use fallback value
    for (uint8_t i = 0; i < sizeof(int64_t); i++) {
        packet_buf->data[4 + i] = 0;
    }

    // Set packet device ID field
    bytes = (uint8_t*)&device_id;
    for (uint8_t i = 0; i < sizeof(uint32_t); i++) {
        packet_buf->data[12 + i] = bytes[i];
    }

    // Set packet latitude to use fallback value
    for (uint8_t i = 0; i < sizeof(float); i++) {
        packet_buf->data[16 + i] = 0;
    }

    // Set packet longitude field
    for (uint8_t i = 0; i < sizeof(float); i++) {
        packet_buf->data[20 + i] = 0;
    }

    // Set packet elevation field
    for (uint8_t i = 0; i < sizeof(float); i++) {
        packet_buf->data[24 + i] = 0;
    }

    // Get checksum for header
    uint8_t header_checksum = 0;
    for (uint8_t i = 2; i < 36; i++) {
        header_checksum ^= packet_buf->data[i];
    }
    packet_buf->data[36] = header_checksum;

    // Set packet data from channel buffer (int32_t to uint8_t)
    for (uint8_t i = 0; i < sample_rate; i++) {
        bytes = (uint8_t*)&channel_buf->data[i];
        for (uint8_t j = 0; j < sizeof(int32_t); j++) {
            packet_buf->data[37 + i * sizeof(int32_t) + j] = bytes[j];
        }
        bytes = (uint8_t*)&channel_buf->data[i + sample_rate];
        for (uint8_t j = 0; j < sizeof(int32_t); j++) {
            packet_buf->data[37 + sample_rate * sizeof(int32_t) +
                             i * sizeof(int32_t) + j] = bytes[j];
        }
        bytes = (uint8_t*)&channel_buf->data[i + 2 * sample_rate];
        for (uint8_t j = 0; j < sizeof(int32_t); j++) {
            packet_buf->data[37 + 2 * sample_rate * sizeof(int32_t) +
                             i * sizeof(int32_t) + j] = bytes[j];
        }
    }

    // Get CRC checksum for channel data
    uint32_t checksum =
        mcu_utils_crc32_get((uint32_t*)channel_buf->data, channel_buf->size);
    bytes = (uint8_t*)&checksum;
    for (uint8_t i = 0; i < sizeof(uint32_t); i++) {
        packet_buf->data[37 + 3 * sample_rate * sizeof(uint32_t) + i] =
            bytes[i];
    }

    // Get checksum for tail
    uint8_t tail_checksum = 0;
    for (uint16_t i = packet_buf->size - sizeof(uint8_t) - sizeof(uint8_t) -
                      sizeof(uint8_t) - sizeof(uint64_t);
         i <
         packet_buf->size - sizeof(uint8_t) - sizeof(uint8_t) - sizeof(uint8_t);
         i++) {
        tail_checksum ^= packet_buf->data[i];
    }
    packet_buf->data[packet_buf->size - sizeof(uint8_t) - sizeof(uint8_t) -
                     sizeof(uint8_t)] = tail_checksum;

    // Set packet frame tail
    packet_buf->data[packet_buf->size - 2] = 0xD9;
    packet_buf->data[packet_buf->size - 1] = 0xF1;

    mcu_utils_uart_write(packet_buf->data, packet_buf->size, false);
}

uint16_t get_data_packet_size(uint16_t sample_rate) {
    return  // Frame header [0:2]
        sizeof(uint8_t) + sizeof(uint8_t) +
        // Sample rate [2:4]
        sizeof(uint16_t) +
        // Unix timestamp [4:12]
        sizeof(int64_t) +
        // Device ID [12:16]
        sizeof(uint32_t) +
        // Latitude [16:20]
        sizeof(float) +
        // Longitude [20:24]
        sizeof(float) +
        // Elevation [24:28]
        sizeof(float) +
        // Reserved [28:36]
        sizeof(uint64_t) +
        // Header checksum [36:37]
        sizeof(uint8_t) +
        // Z-axis data [37:sample_rate*4]
        sample_rate * sizeof(int32_t) +
        // E-axis data [sample_rate*4:2*sample_rate*4]
        sample_rate * sizeof(int32_t) +
        // N-axis data [2*sample_rate*4:3*sample_rate*4]
        sample_rate * sizeof(int32_t) +
        // Channel CRC32 checksum [3*sample_rate*4:3*sample_rate*4+4]
        sizeof(int32_t) +
        // Reserved [3*sample_rate*4+4:3*sample_rate*4+4+8]
        sizeof(uint64_t) +
        // Tail checksum [3*sample_rate*4+4+8:3*sample_rate*4+4+8+1]
        sizeof(uint8_t) +
        // Frame tail [3*sample_rate*4+4+8+1:packet_size]
        sizeof(uint8_t) + sizeof(uint8_t);
}
