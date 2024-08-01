#include "User/Inc/packet.h"

void send_legacy_data_packet(int32_array_t* channel_buf) {
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
    uint8_t sync_words[] = {0xFC, 0x1B};
    mcu_utils_uart_write(sync_words, sizeof(sync_words));
    mcu_utils_uart_write((uint8_t*)&packet, sizeof(legacy_data_packet_t));
}

void send_data_packet(int32_array_t* channel_buf,
                      int64_t timestamp,
                      uint8_t sample_rate) {
    mcu_utils_uart_write((uint8_t*)"send_data_packet\r\n", 18);
}
