#pragma once

#include <stdint.h>
#include "ads1256/gain.hpp"
#include "ads1256/sample.hpp"

// ADC sampling rate (maximum SAMPLE_RATE_30000_SPS)
#define SAMPLE_RATE SAMPLE_RATE_2000_SPS
// ADC gain rate (maximum GAIN_RATE_64X)
#define GAIN_RATE GAIN_RATE_2X
// ADC analog input buffer
#define ADC_BUFFER ENABLE
// ADC analog input calibration
#define ADC_CALIBRATION ENABLE
// serial baud rate (maximum 921600)
#define SERIAL_BAUD 19200
// Data packet size
#define PACKET_SIZE 4
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

// Geophone data frame format
typedef struct {
    int32_t EHZ[PACKET_SIZE];  // Vertical
    int32_t EHE[PACKET_SIZE];  // East-West
    int32_t EHN[PACKET_SIZE];  // North-South
    uint8_t Checksum[3];
} sensor_t;
