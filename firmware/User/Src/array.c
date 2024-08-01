#include "User/Inc/array.h"

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

void array_int32_clear(int32_array_t* arr) {
    free(arr);
}
