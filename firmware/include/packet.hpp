#ifndef __PACKET_HPP
#define __PACKET_HPP

#include <Arduino.h>
#include <stdint.h>

#include "mcu_utils/uart.hpp"
#include "settings.hpp"

typedef struct {
    int32_t EHZ[PACKET_SIZE];  // EHZ (Vertical)
    int32_t EHE[PACKET_SIZE];  // EHE (East-West)
    int32_t EHN[PACKET_SIZE];  // EHN (North-South)
    uint8_t checksum[3];
} packet_t;

void send_data_packet(packet_t packet);
void send_word_packet(const uint8_t* word, uint8_t len);

#endif
