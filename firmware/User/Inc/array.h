#ifndef __ARRAY_H
#define __ARRAY_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint16_t size;
    int32_t data[];
} int32_array_t;

int32_array_t* array_int32_make(uint16_t size);
void array_int32_clear(int32_array_t* arr);

#endif
