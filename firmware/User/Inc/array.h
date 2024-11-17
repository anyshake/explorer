#ifndef __ARRAY_H
#define __ARRAY_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint16_t size;
    int8_t data[];
} int8_array_t;

typedef struct {
    uint16_t size;
    uint8_t data[];
} uint8_array_t;

typedef struct {
    uint16_t size;
    int16_t data[];
} int16_array_t;

typedef struct {
    uint16_t size;
    uint16_t data[];
} uint16_array_t;

typedef struct {
    uint16_t size;
    int32_t data[];
} int32_array_t;

typedef struct {
    uint16_t size;
    uint32_t data[];
} uint32_array_t;

int8_array_t* array_int8_make(uint16_t size);
void array_int8_free(int8_array_t* arr);

uint8_array_t* array_uint8_make(uint16_t size);
void array_uint8_free(uint8_array_t* arr);

int16_array_t* array_int16_make(uint16_t size);
void array_int16_free(int16_array_t* arr);

uint16_array_t* array_uint16_make(uint16_t size);
void array_uint16_free(uint16_array_t* arr);

int32_array_t* array_int32_make(uint16_t size);
void array_int32_free(int32_array_t* arr);

uint32_array_t* array_uint32_make(uint16_t size);
void array_uint32_free(uint32_array_t* arr);

#endif
