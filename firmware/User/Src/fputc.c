#include "User/Inc/fputc.h"

int fputc(int ch, FILE* f) {
    mcu_utils_uart_write((uint8_t*)&ch, 1, true);
    return ch;
}
