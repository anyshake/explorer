#pragma once

#include <Arduino.h>
#include <stdint.h>

uint8_t getChecksum(int32_t* array, size_t size);
