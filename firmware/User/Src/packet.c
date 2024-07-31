#include "User/Inc/packet.h"

void send_legacy_data_packet(array_int32_t* channel_buf,
                             int64_t timestamp,
                             bool is_rtos) {
    mcu_utils_uart_write("send_legacy_data_packet\r\n", 25);
}

void send_data_packet(array_int32_t* channel_buf,
                      int64_t timestamp,
                      uint8_t sample_rate) {
    mcu_utils_uart_write("send_data_packet\r\n", 18);
}
