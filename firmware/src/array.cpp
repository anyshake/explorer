#include "array.h"

int8_array_t* array_int8_make(uint16_t size) {
    int8_array_t* arr =
        (int8_array_t*)malloc(sizeof(int8_array_t) + sizeof(int8_t) * size);
    if (arr == NULL) {
        return NULL;
    }

    arr->size = size;
    memset(arr->data, 0, sizeof(int8_t) * size);

    return arr;
}

void array_int8_free(int8_array_t* arr) {
    free(arr);
}

uint8_array_t* array_uint8_make(uint16_t size) {
    uint8_array_t* arr =
        (uint8_array_t*)malloc(sizeof(uint8_array_t) + sizeof(uint8_t) * size);
    if (arr == NULL) {
        return NULL;
    }

    arr->size = size;
    memset(arr->data, 0, sizeof(uint8_t) * size);

    return arr;
}

void array_uint8_free(uint8_array_t* arr) {
    free(arr);
}

int16_array_t* array_int16_make(uint16_t size) {
    int16_array_t* arr =
        (int16_array_t*)malloc(sizeof(int16_array_t) + sizeof(int16_t) * size);
    if (arr == NULL) {
        return NULL;
    }

    arr->size = size;
    memset(arr->data, 0, sizeof(int16_t) * size);

    return arr;
}

void array_int16_free(int16_array_t* arr) {
    free(arr);
}

uint16_array_t* array_uint16_make(uint16_t size) {
    uint16_array_t* arr = (uint16_array_t*)malloc(sizeof(uint16_array_t) +
                                                  sizeof(uint16_t) * size);
    if (arr == NULL) {
        return NULL;
    }

    arr->size = size;
    memset(arr->data, 0, sizeof(uint16_t) * size);

    return arr;
}

void array_uint16_free(uint16_array_t* arr) {
    free(arr);
}

int32_array_t* array_int32_make(uint16_t size) {
    int32_array_t* arr =
        (int32_array_t*)malloc(sizeof(int32_array_t) + sizeof(int32_t) * size);
    if (arr == NULL) {
        return NULL;
    }

    arr->size = size;
    memset(arr->data, 0, sizeof(int32_t) * size);

    return arr;
}

void array_int32_free(int32_array_t* arr) {
    free(arr);
}

uint32_array_t* array_uint32_make(uint16_t size) {
    uint32_array_t* arr = (uint32_array_t*)malloc(sizeof(uint32_array_t) +
                                                  sizeof(uint32_t) * size);
    if (arr == NULL) {
        return NULL;
    }

    arr->size = size;
    memset(arr->data, 0, sizeof(uint32_t) * size);

    return arr;
}

void array_uint32_free(uint32_array_t* arr) {
    free(arr);
}
