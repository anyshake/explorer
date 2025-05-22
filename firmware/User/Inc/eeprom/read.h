#ifndef __EEPROM_READ_H
#define __EEPROM_READ_H

#include <stdint.h>

#include "User/Inc/eeprom/utils.h"
#include "Utils/Inc/i2c.h"

void eeprom_read(uint8_t* buf, uint8_t offset, uint8_t len);

#endif
