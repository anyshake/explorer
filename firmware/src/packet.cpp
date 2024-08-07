#include "packet.hpp"

void send_data_packet(packet_t packet) {
    uart_writebuf((uint8_t*)SYNC_WORDS, sizeof(SYNC_WORDS));
    uart_writebuf((uint8_t*)&packet, sizeof(packet_t));
}
