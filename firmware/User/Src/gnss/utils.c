#include "User/Inc/gnss/utils.h"

void gnss_init(gnss_ctl_pin_t pin) {
    // 1PPS pin has already been configured in STM32CubeMX
    // mcu_utils_gpio_mode(pin.pps, MCU_UTILS_GPIO_MODE_INPUT);
    mcu_utils_gpio_mode(pin.rst, MCU_UTILS_GPIO_MODE_OUTPUT);
}

void gnss_reset(gnss_ctl_pin_t pin, bool is_rtos) {
    mcu_utils_gpio_low(pin.rst);
    mcu_utils_delay_ms(100, is_rtos);
    mcu_utils_gpio_high(pin.rst);
    mcu_utils_delay_ms(100, is_rtos);
    gnss_model_setup(is_rtos);
}

uint8_t gnss_get_sentence(uint8_t* str_buf, uint16_t timeout_ms) {
    uint8_t line_buf[GNSS_SENTENCE_BUFFER_SIZE];
    uint8_t line_idx = 0;

    for (int64_t start_time = mcu_utils_uptime_get_ms();;) {
        if (mcu_utils_uart2_hasdata()) {
            uint8_t ch = mcu_utils_uart2_read();

            if (ch >= 32 && ch <= 126) {
                if (ch == '$') {
                    line_idx = 0;
                }

                if (line_idx < GNSS_SENTENCE_BUFFER_SIZE - 1) {
                    line_buf[line_idx++] = ch;
                }
            }

            if (ch == '\n' || line_idx >= GNSS_SENTENCE_BUFFER_SIZE - 1) {
                line_buf[line_idx] = '\0';

                if (gnss_verify_checksum(line_buf)) {
                    strncpy((char*)str_buf, (char*)line_buf, line_idx + 1);
                    return 0;
                }

                line_idx = 0;
            }
        }

        if ((mcu_utils_uptime_get_ms() - start_time) >= timeout_ms) {
            return -1;
        }
    }
}

uint8_t gnss_padding_sentence(uint8_t* str_buf) {
    uint8_t str_len = strlen((char*)str_buf);
    uint8_t new_str[GNSS_SENTENCE_BUFFER_SIZE];
    uint8_t new_len = 0;

    for (uint8_t i = 0; i < str_len; i++) {
        new_str[new_len++] = str_buf[i];
        if (str_buf[i] == ',' && i < str_len - 1 && str_buf[i + 1] == ',') {
            new_str[new_len++] = GNSS_SENTENCE_PADDING_CHAR;
        }
    }

    strncpy((char*)str_buf, (char*)new_str, new_len);
    str_buf[new_len] = '\0';

    return new_len;
}

bool gnss_verify_checksum(uint8_t* str_buf) {
    const char* start = strchr((char*)str_buf, '$');
    const char* end = strchr(start, '*');

    if (start && end) {
        uint8_t checksum = 0;
        for (const char* ch = start + 1; ch < end; ch++) {
            checksum ^= *ch;
        }

        uint8_t msg_checksum = 0;
        for (uint8_t i = 1; i <= 2; i++) {
            char ch = *(end + i);
            if (isdigit(ch)) {
                msg_checksum = (msg_checksum << 4) + (ch - '0');
            } else if (isxdigit(ch)) {
                msg_checksum = (msg_checksum << 4) + (toupper(ch) - 'A' + 10);
            } else {
                return false;
            }
        }

        return checksum == msg_checksum;
    }

    return false;
}

bool gnss_match_keyword(uint8_t* sentence, const char* keyword) {
    if (!sentence || !keyword) {
        return false;
    }
    if (sentence[0] != '$') {
        return false;
    }
    return (strstr((char*)sentence, keyword) != NULL);
}
