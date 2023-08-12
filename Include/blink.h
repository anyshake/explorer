#ifndef APP_BLINK_H
#define APP_BLINK_H

#include <stdint.h>
#include "gd32f30x.h"
#include "delay.h"

void blink_led(uint8_t times, uint16_t interval);

#endif
