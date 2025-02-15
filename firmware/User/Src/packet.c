#include "User/Inc/packet.h"

uint16_t get_data_packet_size(bool use_accelerometer,
                              bool channel_6d,
                              uint8_t channel_chunk_length) {
    uint16_t base_size = sizeof(packet_header_t) +
                         sizeof(packet_timestamp_t) +
                         sizeof(packet_device_config_t) +
                         sizeof(packet_variable_data_t) +
                         sizeof(packet_checksum_t) +
                         sizeof(packet_trailer_t);

    if (!use_accelerometer || channel_6d) {
        base_size += (sizeof(int32_t) * channel_chunk_length * 3);
    }
    if (use_accelerometer || channel_6d) {
        base_size += (sizeof(int16_t) * channel_chunk_length * 3);
    }

    return base_size;
}

void send_data_packet(explorer_states_t* states, float temperature, int64_t timestamp) {
    uint16_t offset = 0;
    uint8_t* bytes;

    uint16_t uart_buf_size = states->uart_packet_buffer->size;
    states->uart_packet_buffer->data[0] = 0x01;
    states->uart_packet_buffer->data[1] = 0xFE;
    states->uart_packet_buffer->data[uart_buf_size - 2] = 0xEF;
    states->uart_packet_buffer->data[uart_buf_size - 1] = 0x10;

    bytes = (uint8_t*)&timestamp;
    for (uint8_t i = 0; i < sizeof(packet_timestamp_t); i++) {
        offset = sizeof(packet_header_t) + i;
        states->uart_packet_buffer->data[offset] = bytes[i];
    }

    uint32_t device_config = 0x00;
    switch (states->packet_sending_interval) {
        case 100:
            device_config |= PACKET_DEVICE_CONFIG_PACKET_INTERVAL_100MS << 30;
            break;
        case 200:
            device_config |= PACKET_DEVICE_CONFIG_PACKET_INTERVAL_200MS << 30;
            break;
        case 500:
            device_config |= PACKET_DEVICE_CONFIG_PACKET_INTERVAL_500MS << 30;
            break;
        case 1000:
            device_config |= PACKET_DEVICE_CONFIG_PACKET_INTERVAL_1000MS << 30;
            break;
    }
    switch (states->sample_rate) {
        case 10:
            device_config |= PACKET_DEVICE_CONFIG_SAMPLE_RATE_10HZ << 27;
            break;
        case 20:
            device_config |= PACKET_DEVICE_CONFIG_SAMPLE_RATE_20HZ << 27;
            break;
        case 50:
            device_config |= PACKET_DEVICE_CONFIG_SAMPLE_RATE_50HZ << 27;
            break;
        case 100:
            device_config |= PACKET_DEVICE_CONFIG_SAMPLE_RATE_100HZ << 27;
            break;
        case 200:
            device_config |= PACKET_DEVICE_CONFIG_SAMPLE_RATE_200HZ << 27;
            break;
        case 250:
            device_config |= PACKET_DEVICE_CONFIG_SAMPLE_RATE_250HZ << 27;
            break;
    }
    device_config |= (states->use_gnss_time ? PACKET_DEVICE_CONFIG_GNSS_AVAILABLE : PACKET_DEVICE_CONFIG_GNSS_NOT_AVAILABLE) << 26;
    if (states->channel_6d) {
        device_config |= (PACKET_DEVICE_CONFIG_CHANNEL_1_INT32 << 24) |
                         (PACKET_DEVICE_CONFIG_CHANNEL_2_INT32 << 22) |
                         (PACKET_DEVICE_CONFIG_CHANNEL_3_INT32 << 20) |
                         (PACKET_DEVICE_CONFIG_CHANNEL_4_INT16 << 18) |
                         (PACKET_DEVICE_CONFIG_CHANNEL_5_INT16 << 16) |
                         (PACKET_DEVICE_CONFIG_CHANNEL_6_INT16 << 14);
    } else {
        uint32_t type = states->use_accelerometer ? PACKET_DEVICE_CONFIG_CHANNEL_1_INT16 : PACKET_DEVICE_CONFIG_CHANNEL_1_INT32;
        device_config |= (type << 24) | (type << 22) | (type << 20);
    }
    device_config |= (PACKET_VARIABLE_CONFIG_BIT_4_ENABLED << 4) |
                     (states->use_gnss_time ? PACKET_VARIABLE_CONFIG_BIT_3_ENABLED : PACKET_VARIABLE_CONFIG_BIT_3_DISABLED << 3) |
                     (states->use_gnss_time ? PACKET_VARIABLE_CONFIG_BIT_2_ENABLED : PACKET_VARIABLE_CONFIG_BIT_2_DISABLED << 2) |
                     (states->use_gnss_time ? PACKET_VARIABLE_CONFIG_BIT_1_ENABLED : PACKET_VARIABLE_CONFIG_BIT_1_DISABLED << 1) |
                     (PACKET_VARIABLE_CONFIG_BIT_0_ENABLED);
    bytes = (uint8_t*)&device_config;
    for (uint8_t i = 0; i < sizeof(packet_device_config_t); i++) {
        offset = sizeof(packet_header_t) + sizeof(packet_timestamp_t) + i;
        states->uart_packet_buffer->data[offset] = bytes[i];
    }

    switch ((timestamp / 1000) % 10) {
        case 0:
            bytes = (uint8_t*)&states->device_id;
            break;
        case 1:
            bytes = (uint8_t*)&states->gnss_location.latitude;
            break;
        case 2:
            bytes = (uint8_t*)&states->gnss_location.longitude;
            break;
        case 3:
            bytes = (uint8_t*)&states->gnss_location.elevation;
            break;
        case 4:
            bytes = (uint8_t*)&temperature;
            break;
    }
    for (uint8_t i = 0; i < sizeof(packet_variable_data_t); i++) {
        offset = sizeof(packet_header_t) + sizeof(packet_timestamp_t) + sizeof(packet_device_config_t) + i;
        states->uart_packet_buffer->data[offset] = bytes[i];
    }

    offset = sizeof(packet_header_t) + sizeof(packet_timestamp_t) + sizeof(packet_device_config_t) + sizeof(packet_variable_data_t);
    states->uart_packet_buffer->data[offset] = 0xFF;
    for (uint8_t i = 0; i < states->channel_chunk_length; i++) {
        if (!states->use_accelerometer || states->channel_6d) {
            uint16_t adc_offset = offset + i * sizeof(int32_t);
            bytes = (uint8_t*)&states->adc_acquisition_channel_buffer->data[i];
            for (uint8_t j = 0; j < sizeof(int32_t); j++) {
                states->uart_packet_buffer->data[adc_offset + j] = bytes[j];
            }
            bytes = (uint8_t*)&states->adc_acquisition_channel_buffer->data[i + states->channel_chunk_length];
            for (uint8_t j = 0; j < sizeof(int32_t); j++) {
                states->uart_packet_buffer->data[adc_offset + states->channel_chunk_length * sizeof(int32_t) + j] = bytes[j];
            }
            bytes = (uint8_t*)&states->adc_acquisition_channel_buffer->data[i + 2 * states->channel_chunk_length];
            for (uint8_t j = 0; j < sizeof(int32_t); j++) {
                states->uart_packet_buffer->data[adc_offset + 2 * states->channel_chunk_length * sizeof(int32_t) + j] = bytes[j];
            }
        }

        if (states->use_accelerometer || states->channel_6d) {
            uint16_t accel_offset = offset + 3 * states->channel_chunk_length * sizeof(int32_t) + i * sizeof(int16_t);
            bytes = (uint8_t*)&states->accel_acquisition_channel_buffer->data[i];
            for (uint8_t j = 0; j < sizeof(int16_t); j++) {
                states->uart_packet_buffer->data[accel_offset + j] = bytes[j];
            }
            bytes = (uint8_t*)&states->accel_acquisition_channel_buffer->data[i + states->channel_chunk_length];
            for (uint8_t j = 0; j < sizeof(int16_t); j++) {
                states->uart_packet_buffer->data[accel_offset + states->channel_chunk_length * sizeof(int16_t) + j] = bytes[j];
            }
            bytes = (uint8_t*)&states->accel_acquisition_channel_buffer->data[i + 2 * states->channel_chunk_length];
            for (uint8_t j = 0; j < sizeof(int16_t); j++) {
                states->uart_packet_buffer->data[accel_offset + 2 * states->channel_chunk_length * sizeof(int16_t) + j] = bytes[j];
            }
        }
    }

    offset = uart_buf_size - sizeof(packet_trailer_t) - sizeof(packet_checksum_t);
    uint8_t xor_checksum = 0;
    for (uint16_t i = sizeof(packet_header_t); i < offset; i++) {
        xor_checksum ^= states->uart_packet_buffer->data[i];
    }
    states->uart_packet_buffer->data[offset] = xor_checksum;

    mcu_utils_uart_write(states->uart_packet_buffer->data, uart_buf_size, false);
}
