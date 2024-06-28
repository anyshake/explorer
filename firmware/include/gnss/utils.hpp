#ifndef __GNSS_UTILS__
#define __GNSS_UTILS__

#include <stdint.h>
#include "gnss/types.hpp"
#include "mcu_utils/delay.hpp"
#include "mcu_utils/uart2.hpp"

void gnss_init(uint32_t baudrate);
uint8_t gnss_get_sentence(uint8_t* str_buf, const char* keyword);
uint8_t gnss_padding_sentence(uint8_t* str_buf);
uint8_t gnss_check_checksum(uint8_t* str_buf);
int64_t gnss_get_timestamp(gnss_time_t* time);

#endif
