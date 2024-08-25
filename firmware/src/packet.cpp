#include "packet.h"

void send_data_packet(int32_array_t* channel_buf) {
    if (channel_buf == NULL) {
        return;
    }
    data_packet_t packet = {0};

    // Set packet data from channel buffer
    for (uint8_t i = 0; i < EXPLORER_CHANNEL_SIZE; i++) {
        packet.z_axis[i] = channel_buf->data[i];
        packet.e_axis[i] = channel_buf->data[i + EXPLORER_CHANNEL_SIZE];
        packet.n_axis[i] = channel_buf->data[i + 2 * EXPLORER_CHANNEL_SIZE];
    }

    // Calculate checksums for each channel
    for (uint8_t i = 0; i < EXPLORER_CHANNEL_SIZE; i++) {
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
