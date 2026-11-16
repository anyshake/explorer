#ifndef __USER_DFU_LINK_H
#define __USER_DFU_LINK_H

#include <stdbool.h>
#include <stdint.h>

#include "Core/Inc/main.h"

#include "User/Inc/boot.h"
#include "User/Inc/dfu/crc.h"
#include "User/Inc/dfu/protocol.h"

#define DFU_FOREVER UINT32_MAX
#define DFU_INTER_BYTE_TIMEOUT_MS 200U

typedef enum {
    DFU_RX_OK,
    DFU_RX_TIMEOUT,
    DFU_RX_BAD_CRC,
} dfu_rx_result_t;

typedef struct {
    uint8_t cmd;
    uint32_t len;             // payload bytes, % 4 == 0
    const uint32_t* payload;  // valid until the next dfu_link_receive()
} dfu_request_t;

dfu_rx_result_t dfu_link_receive(uint32_t deadline, dfu_request_t* req);
void dfu_link_send(uint8_t cmd, dfu_status_t status, const uint32_t* payload, uint32_t words);
void dfu_link_set_blink_period(uint32_t ms);
bool dfu_link_expired(uint32_t deadline);

#endif
