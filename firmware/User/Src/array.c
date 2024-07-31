#include "User/Inc/array.h"

array_int32_t* array_int32_make(uint16_t size) {
    array_int32_t* arr =
        (array_int32_t*)malloc(sizeof(array_int32_t) + sizeof(int32_t) * size);
    if (arr == NULL) {
        return NULL;
    }

    arr->size = size;
    memset(arr->data, 0, sizeof(int32_t) * size);

    return arr;
}

void array_int32_clear(array_int32_t* arr) {
    free(arr);
}
