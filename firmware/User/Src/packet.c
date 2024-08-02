#include "User/Inc/packet.h"

void send_legacy_data_packet(int32_array_t* channel_buf) {
    if (channel_buf == NULL) {
        return;
    }
    legacy_data_packet_t packet = {0};

    // Set packet data from channel buffer
    for (uint8_t i = 0; i < LEGACY_PACKET_CHANNEL_SIZE; i++) {
        packet.z_axis[i] = channel_buf->data[i];
        packet.e_axis[i] = channel_buf->data[i + LEGACY_PACKET_CHANNEL_SIZE];
        packet.n_axis[i] =
            channel_buf->data[i + 2 * LEGACY_PACKET_CHANNEL_SIZE];
    }

    // Calculate checksums for each channel
    for (uint8_t i = 0; i < LEGACY_PACKET_CHANNEL_SIZE; i++) {
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
    mcu_utils_uart_write((uint8_t*)&packet, sizeof(legacy_data_packet_t), true);
}

void send_data_packet(int32_array_t* channel_buf,
                      uint8_array_t* packet_buf,
                      int64_t timestamp,
                      uint16_t sample_rate,
                      uint32_t device_id) {
    if (channel_buf == NULL || packet_buf == NULL) {
        return;
    }

    // Calculate packet size and initialize buffer
    uint16_t packet_size =
        // Frame header [0:2]
        2 +
        // Unix timestamp [2:10]
        8 +
        // Device ID [10:14]
        4 +
        // Reserved [14:22]
        8 +
        // Sample rate [22:24]
        2 +
        // Header checksum [24:26]
        2 +
        // Z-axis data [26:sample_rate]
        sample_rate * sizeof(int32_t) +
        // E-axis data [sample_rate:2*sample_rate]
        sample_rate * sizeof(int32_t) +
        // N-axis data [2*sample_rate:3*sample_rate]
        sample_rate * sizeof(int32_t) +
        // Channle data checksum [3*sample_rate:3*sample_rate+4]
        4 +
        // Reserved [3*sample_rate+4:3*sample_rate+4+8]
        8 +
        // Tail checksum [3*sample_rate+4+8:3*sample_rate+4+8+2]
        2 +
        // Frame tail [3*sample_rate+4+8+2:packet_size]
        2;
    if (packet_buf->size != packet_size) {
        array_uint8_free(packet_buf);
        packet_buf = array_uint8_make(packet_size);
    }

    // Set packet reserved fields to 0xFF
    for (uint8_t i = 0; i < sizeof(int64_t); i++) {
        packet_buf->data[14 + i] = 0xFF;
        packet_buf->data[packet_size - sizeof(int64_t) - 2 + i] = 0xFF;
    }

    // Set packet frame header field
    packet_buf->data[0] = 0xF1;
    packet_buf->data[1] = 0xD9;

    // Set packet timestamp
    uint8_t* bytes = (uint8_t*)&timestamp;
    for (uint8_t i = 0; i < sizeof(int64_t); i++) {
        packet_buf->data[2 + i] = bytes[i];
    }

    // Set packet device ID field
    bytes = (uint8_t*)&device_id;
    for (uint8_t i = 0; i < sizeof(uint32_t); i++) {
        packet_buf->data[10 + i] = bytes[i];
    }

    // Set packet sample rate
    bytes = (uint8_t*)&sample_rate;
    for (uint8_t i = 0; i < sizeof(uint16_t); i++) {
        packet_buf->data[22 + i] = bytes[i];
    }

    // Get checksum for header
    uint16_t header_checksum = 0;
    for (uint8_t i = 1; i < 24; i++) {
        header_checksum ^= packet_buf->data[i];
    }
    bytes = (uint8_t*)&header_checksum;
    for (uint8_t i = 0; i < sizeof(uint16_t); i++) {
        packet_buf->data[24 + i] = bytes[i];
    }

    // Set packet data from channel buffer (int32_t to uint8_t)
    for (uint8_t i = 0; i < sample_rate; i++) {
        bytes = (uint8_t*)&channel_buf->data[i];
        for (uint8_t j = 0; j < sizeof(int32_t); j++) {
            packet_buf->data[24 + i * sizeof(int32_t) + j] = bytes[j];
        }
        bytes = (uint8_t*)&channel_buf->data[i + sample_rate];
        for (uint8_t j = 0; j < sizeof(int32_t); j++) {
            packet_buf->data[24 + sample_rate * sizeof(int32_t) +
                             i * sizeof(int32_t) + j] = bytes[j];
        }
        bytes = (uint8_t*)&channel_buf->data[i + 2 * sample_rate];
        for (uint8_t j = 0; j < sizeof(int32_t); j++) {
            packet_buf->data[24 + 2 * sample_rate * sizeof(int32_t) +
                             i * sizeof(int32_t) + j] = bytes[j];
        }
    }

    // Get CRC checksum for channel data
    uint32_t checksum =
        mcu_utils_crc32_get((uint32_t*)channel_buf->data, channel_buf->size);
    bytes = (uint8_t*)&checksum;
    for (uint8_t i = 0; i < sizeof(uint32_t); i++) {
        packet_buf->data[24 + 3 * sample_rate * sizeof(uint32_t) + i] =
            bytes[i];
    }

    // Get checksum for tail
    uint16_t tail_checksum = 0;
    for (uint16_t i = packet_size - 12; i < packet_size - 4; i++) {
        tail_checksum ^= packet_buf->data[i];
    }
    bytes = (uint8_t*)&tail_checksum;
    for (uint8_t i = 0; i < sizeof(uint16_t); i++) {
        packet_buf->data[packet_size - 4 + i] = bytes[i];
    }

    // Set packet frame tail
    packet_buf->data[packet_size - 2] = 0xD9;
    packet_buf->data[packet_size - 1] = 0xF1;

    mcu_utils_uart_write(packet_buf->data, packet_size, false);
}
