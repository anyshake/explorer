#include "checksum.hpp"

uint8_t getChecksum(int32_t* array, size_t size) {
    uint8_t checksum = 0;

    for (size_t i = 0; i < size; i++) {
        uint8_t* bytes = (uint8_t*)&array[i];

        for (size_t j = 0; j < sizeof(int32_t); j++) {
            checksum ^= bytes[j];
        }
    }

    return checksum;
}
