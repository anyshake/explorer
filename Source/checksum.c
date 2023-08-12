#include "checksum.h"

uint8_t get_checksum(int32_t* array, uint32_t size) {
    uint8_t checksum = 0;

    for (uint32_t i = 0; i < size; i++) {
        uint8_t* bytes = (uint8_t*)&array[i];

        for (uint8_t j = 0; j < sizeof(int32_t); j++) {
            checksum ^= bytes[j];
        }
    }

    return checksum;
}
