#ifndef APP_SERIAL_H
#define APP_SERIAL_H

#include <stdint.h>
#include "gd32f30x_usart.h"

void serial_begin(uint32_t baudRate);
void serial_write(uint8_t ch);
void serial_print(uint8_t* str);
uint8_t serial_available(void);
uint8_t serial_read(void);

#endif
