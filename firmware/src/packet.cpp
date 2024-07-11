#include "packet.hpp"

void send_data_packet(packet_t packet) {
    uint8_t* packet_bytes = (uint8_t*)&packet;
    for (uint8_t i = 0; i < sizeof(packet_t); i++) {
        mcu_utils_uart_writech(packet_bytes[i]);
    }
}

void send_word_packet(const uint8_t* word, uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        mcu_utils_uart_writech(word[i]);
    }
}
