#ifndef __ARRAY_H
#define __ARRAY_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint16_t size;
    int32_t data[];
} array_int32_t;

array_int32_t* array_int32_make(uint16_t size);
void array_int32_clear(array_int32_t* arr);

#endif
