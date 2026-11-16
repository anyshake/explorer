#include "User/Inc/dfu/link.h"

static uint32_t rx_words[1U + DFU_MAX_PAYLOAD / 4U + 1U];
static uint32_t blink_period = 250U;

bool dfu_link_expired(uint32_t deadline) {
    return deadline != DFU_FOREVER && (int32_t)(boot_ms - deadline) >= 0;
}

void dfu_link_set_blink_period(uint32_t ms) {
    blink_period = ms;
}

static void idle(void) {
    static uint32_t last_toggle;
    boot_feed_watchdog();
    if (boot_ms - last_toggle >= blink_period) {
        last_toggle = boot_ms;
        LL_GPIO_TogglePin(MCU_STATE_GPIO_Port, MCU_STATE_Pin);
    }
}

// Returns the received byte, or -1 once `deadline` has passed.
static int rx_byte(uint32_t deadline) {
    while (1) {
        if (LL_USART_IsActiveFlag_ORE(USART1) || LL_USART_IsActiveFlag_FE(USART1) || LL_USART_IsActiveFlag_NE(USART1)) {
            // SR then DR read: clears ORE, FE and NE together and drops the byte.
            LL_USART_ClearFlag_ORE(USART1);
            continue;
        }
        if (LL_USART_IsActiveFlag_RXNE(USART1)) {
            return LL_USART_ReceiveData8(USART1);
        }
        idle();
        if (dfu_link_expired(deadline)) {
            return -1;
        }
    }
}

dfu_rx_result_t dfu_link_receive(uint32_t deadline, dfu_request_t* req) {
    // Slide a 4-byte window over the input until it holds a plausible
    // header; anything else on the line (e.g. the firmware's data stream)
    // is discarded.
    uint32_t hdr = 0;
    while (1) {
        int ch = rx_byte(deadline);
        if (ch < 0) {
            return DFU_RX_TIMEOUT;
        }
        hdr = hdr >> 8 | (uint32_t)ch << 24;
        uint32_t len = hdr >> 16;
        uint32_t cmd = hdr >> 8 & 0xFFU;
        if ((hdr & 0xFFU) == DFU_HOST_SYNC && cmd - 1U < 6U && len <= DFU_MAX_PAYLOAD && (len & 3U) == 0) {
            rx_words[0] = hdr;
            req->cmd = (uint8_t)cmd;
            req->len = len;
            req->payload = &rx_words[1];
            break;
        }
    }

    uint8_t* dst = (uint8_t*)&rx_words[1];
    for (uint32_t i = 0; i < req->len + 4U; i++) {
        int ch = rx_byte(boot_ms + DFU_INTER_BYTE_TIMEOUT_MS);
        if (ch < 0) {
            return DFU_RX_TIMEOUT;
        }
        dst[i] = (uint8_t)ch;
    }
    uint32_t words = req->len / 4U;
    return dfu_crc_words(hdr, &rx_words[1], words) == rx_words[1 + words] ? DFU_RX_OK : DFU_RX_BAD_CRC;
}

static void tx_word(uint32_t w) {
    for (uint32_t i = 0; i < 4U; i++) {
        while (!LL_USART_IsActiveFlag_TXE(USART1)) {
        }
        LL_USART_TransmitData8(USART1, (uint8_t)(w >> (8U * i)));
    }
}

void dfu_link_send(uint8_t cmd, dfu_status_t status, const uint32_t* payload, uint32_t words) {
    uint32_t hdr = DFU_DEVICE_SYNC | (uint32_t)cmd << 8 | (uint32_t)status << 16 | (words * 4U) << 24;
    tx_word(hdr);
    for (uint32_t i = 0; i < words; i++) {
        tx_word(payload[i]);
    }
    tx_word(dfu_crc_words(hdr, payload, words));
    while (!LL_USART_IsActiveFlag_TC(USART1)) {
    }
}
