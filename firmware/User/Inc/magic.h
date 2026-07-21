#ifndef __MAGIC_H
#define __MAGIC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAGIC_PI 3.14159265358979323846f

float quick_sqrt(float x);
float quick_atan2(float y, float x);
void format_float_fixed(char* buffer, size_t buffer_size, float value, uint8_t fractional_digits);

#endif
