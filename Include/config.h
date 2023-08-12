#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <stdint.h>
#include "ads1256/gain.h"
#include "ads1256/sample.h"

// ADC sampling rate (maximum SAMPLE_RATE_30000_SPS)
#define SAMPLE_RATE SAMPLE_RATE_2000_SPS
// ADC gain rate (maximum GAIN_RATE_64X)
#define GAIN_RATE GAIN_RATE_1X
// serial baud rate (maximum 921600)
#define SERIAL_BAUD 19200
// Data packet size (maximum 375)
#define PACKET_SIZE 10
// Reset word
#define RESET_WORD 0x61

// Syncing bytes
const uint8_t SYNC_WORD[] = {
    0xFC,
    0x1B,
};
// Responding bytes
const uint8_t ACK_WORD[] = {
    0xFC,
    0x2B,
};

typedef struct {
    int32_t EHZ[PACKET_SIZE];  // Vertical
    int32_t EHE[PACKET_SIZE];  // East-West
    int32_t EHN[PACKET_SIZE];  // North-South
    uint8_t Checksum[3];
} sensor_t;

#endif
