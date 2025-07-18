#ifndef __PACKET_H
#define __PACKET_H

#include <stdbool.h>
#include <stdint.h>

#include "User/Inc/array.h"
#include "User/Inc/gnss/utils.h"
#include "User/Inc/types.h"
#include "Utils/Inc/uart.h"

typedef uint8_t packet_header_t[2];
typedef uint8_t packet_timestamp_t[8];
typedef uint8_t packet_device_config_t[4];
typedef uint8_t packet_variable_data_t[4];
typedef uint8_t packet_checksum_t;
typedef uint8_t packet_trailer_t[2];

#define PACKET_DEVICE_CONFIG_PACKET_INTERVAL_100MS 0x00
#define PACKET_DEVICE_CONFIG_PACKET_INTERVAL_200MS 0x01
#define PACKET_DEVICE_CONFIG_PACKET_INTERVAL_500MS 0x02
#define PACKET_DEVICE_CONFIG_PACKET_INTERVAL_1000MS 0x03

#define PACKET_DEVICE_CONFIG_SAMPLE_RATE_10HZ 0x00
#define PACKET_DEVICE_CONFIG_SAMPLE_RATE_20HZ 0x01
#define PACKET_DEVICE_CONFIG_SAMPLE_RATE_50HZ 0x02
#define PACKET_DEVICE_CONFIG_SAMPLE_RATE_100HZ 0x03
#define PACKET_DEVICE_CONFIG_SAMPLE_RATE_200HZ 0x04
#define PACKET_DEVICE_CONFIG_SAMPLE_RATE_250HZ 0x05
#define PACKET_DEVICE_CONFIG_SAMPLE_RATE_500HZ 0x06
#define PACKET_DEVICE_CONFIG_SAMPLE_RATE_1000HZ 0x07

#define PACKET_DEVICE_CONFIG_GNSS_NOT_AVAILABLE 0x00
#define PACKET_DEVICE_CONFIG_GNSS_AVAILABLE 0x01

#define PACKET_DEVICE_CONFIG_CHANNEL_1_DISABLED 0x00
#define PACKET_DEVICE_CONFIG_CHANNEL_1_INT16 0x01
#define PACKET_DEVICE_CONFIG_CHANNEL_1_INT24 0x02
#define PACKET_DEVICE_CONFIG_CHANNEL_1_INT32 0x03

#define PACKET_DEVICE_CONFIG_CHANNEL_2_DISABLED 0x00
#define PACKET_DEVICE_CONFIG_CHANNEL_2_INT16 0x01
#define PACKET_DEVICE_CONFIG_CHANNEL_2_INT24 0x02
#define PACKET_DEVICE_CONFIG_CHANNEL_2_INT32 0x03

#define PACKET_DEVICE_CONFIG_CHANNEL_3_DISABLED 0x00
#define PACKET_DEVICE_CONFIG_CHANNEL_3_INT16 0x01
#define PACKET_DEVICE_CONFIG_CHANNEL_3_INT24 0x02
#define PACKET_DEVICE_CONFIG_CHANNEL_3_INT32 0x03

#define PACKET_DEVICE_CONFIG_CHANNEL_4_DISABLED 0x00
#define PACKET_DEVICE_CONFIG_CHANNEL_4_INT16 0x01
#define PACKET_DEVICE_CONFIG_CHANNEL_4_INT24 0x02
#define PACKET_DEVICE_CONFIG_CHANNEL_4_INT32 0x03

#define PACKET_DEVICE_CONFIG_CHANNEL_5_DISABLED 0x00
#define PACKET_DEVICE_CONFIG_CHANNEL_5_INT16 0x01
#define PACKET_DEVICE_CONFIG_CHANNEL_5_INT24 0x02
#define PACKET_DEVICE_CONFIG_CHANNEL_5_INT32 0x03

#define PACKET_DEVICE_CONFIG_CHANNEL_6_DISABLED 0x00
#define PACKET_DEVICE_CONFIG_CHANNEL_6_INT16 0x01
#define PACKET_DEVICE_CONFIG_CHANNEL_6_INT24 0x02
#define PACKET_DEVICE_CONFIG_CHANNEL_6_INT32 0x03

#define PACKET_DEVICE_CONFIG_CHANNEL_7_DISABLED 0x00
#define PACKET_DEVICE_CONFIG_CHANNEL_7_INT16 0x01
#define PACKET_DEVICE_CONFIG_CHANNEL_7_INT24 0x02
#define PACKET_DEVICE_CONFIG_CHANNEL_7_INT32 0x03

#define PACKET_DEVICE_CONFIG_CHANNEL_8_DISABLED 0x00
#define PACKET_DEVICE_CONFIG_CHANNEL_8_INT16 0x01
#define PACKET_DEVICE_CONFIG_CHANNEL_8_INT24 0x02
#define PACKET_DEVICE_CONFIG_CHANNEL_8_INT32 0x03

#define PACKET_VARIABLE_CONFIG_BIT_0_DISABLED 0x00
#define PACKET_VARIABLE_CONFIG_BIT_0_ENABLED 0x01

#define PACKET_VARIABLE_CONFIG_BIT_1_DISABLED 0x00
#define PACKET_VARIABLE_CONFIG_BIT_1_ENABLED 0x01

#define PACKET_VARIABLE_CONFIG_BIT_2_DISABLED 0x00
#define PACKET_VARIABLE_CONFIG_BIT_2_ENABLED 0x01

#define PACKET_VARIABLE_CONFIG_BIT_3_DISABLED 0x00
#define PACKET_VARIABLE_CONFIG_BIT_3_ENABLED 0x01

#define PACKET_VARIABLE_CONFIG_BIT_4_DISABLED 0x00
#define PACKET_VARIABLE_CONFIG_BIT_4_ENABLED 0x01

#define PACKET_VARIABLE_CONFIG_BIT_5_DISABLED 0x00
#define PACKET_VARIABLE_CONFIG_BIT_5_ENABLED 0x01

#define PACKET_VARIABLE_CONFIG_BIT_6_DISABLED 0x00
#define PACKET_VARIABLE_CONFIG_BIT_6_ENABLED 0x01

#define PACKET_VARIABLE_CONFIG_BIT_7_DISABLED 0x00
#define PACKET_VARIABLE_CONFIG_BIT_7_ENABLED 0x01

#define PACKET_VARIABLE_CONFIG_BIT_8_DISABLED 0x00
#define PACKET_VARIABLE_CONFIG_BIT_8_ENABLED 0x01

#define PACKET_VARIABLE_CONFIG_BIT_9_DISABLED 0x00
#define PACKET_VARIABLE_CONFIG_BIT_9_ENABLED 0x01

uint16_t get_data_packet_size(bool use_accelerometer, bool channel_6d, uint8_t channel_chunk_length);
void send_data_packet(explorer_global_states_t* states, float temperature, int64_t timestamp);

#endif
