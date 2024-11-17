#ifndef __FPUTC_H
#define __FPUTC_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "Utils/Inc/uart.h"

int fputc(int ch, FILE* f);

#endif
