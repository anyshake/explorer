#include "User/Inc/dfu/core.h"

#ifndef BL_REV
#define BL_REV "custombuild"
#endif

static struct {
    boot_reason_t reason;
    bool begun;
    uint32_t image_size;
    uint32_t image_crc;
    uint32_t next_offset;
    uint32_t first_word;  // initial SP, programmed last to commit the image
} session;

static dfu_request_t req;

static const char bl_rev[16] = BL_REV;

// Blink fast while an update is in progress.
static void set_begun(bool begun) {
    session.begun = begun;
    dfu_link_set_blink_period(begun ? 50U : 250U);
}

static void tx_frame(dfu_status_t status, const uint32_t* payload, uint32_t words) {
    dfu_link_send(req.cmd, status, payload, words);
}

static void reply(dfu_status_t status) {
    tx_frame(status, 0, 0);
}

static void reply_offset(dfu_status_t status) {
    tx_frame(status, &session.next_offset, 1);
}

static void handle_hello(void) {
    uint32_t out[8];
    out[0] = (uint32_t)session.reason | (uint32_t)boot_app_is_valid() << 8;
    out[1] = BOOT_APP_BASE;
    out[2] = BOOT_APP_MAX_SIZE;
    out[3] = BOOT_FLASH_PAGE_SIZE | DFU_MAX_CHUNK << 16;
    __builtin_memcpy(&out[4], bl_rev, sizeof(bl_rev));
    tx_frame(DFU_STATUS_OK, out, 8);
}

static void handle_begin(void) {
    if (req.len != 8U) {
        reply(DFU_STATUS_BAD_LENGTH);
        return;
    }
    uint32_t size = req.payload[0];
    if (size < BOOT_VECTOR_WORDS * 4U || size > BOOT_APP_MAX_SIZE || (size & 3U) != 0) {
        reply(DFU_STATUS_BAD_IMAGE);
        return;
    }

    set_begun(false);
    if (!dfu_flash_erase_app(size)) {
        reply(DFU_STATUS_FLASH_ERROR);
        return;
    }
    set_begun(true);
    session.image_size = size;
    session.image_crc = req.payload[1];
    session.next_offset = 0;
    session.first_word = 0xFFFFFFFFU;
    reply(DFU_STATUS_OK);
}

static void handle_data(void) {
    if (!session.begun) {
        reply(DFU_STATUS_BAD_STATE);
        return;
    }
    if (req.len < 8U) {
        reply(DFU_STATUS_BAD_LENGTH);
        return;
    }

    uint32_t offset = req.payload[0];
    uint32_t len = req.len - 4U;

    // A retransmission of the chunk we just wrote (its ACK got lost).
    if (offset < session.next_offset && offset + len == session.next_offset) {
        reply_offset(DFU_STATUS_OK);
        return;
    }
    if (offset != session.next_offset || offset + len > session.image_size) {
        reply_offset(DFU_STATUS_BAD_OFFSET);
        return;
    }

    const uint32_t* data = &req.payload[1];
    for (uint32_t i = 0; i < len; i += 4U, data++) {
        if (offset + i == 0) {
            session.first_word = *data;
        } else if (!dfu_flash_program_word(BOOT_APP_BASE + offset + i, *data)) {
            set_begun(false);
            reply(DFU_STATUS_FLASH_ERROR);
            return;
        }
    }
    session.next_offset = offset + len;
    reply_offset(DFU_STATUS_OK);
}

static void handle_end(void) {
    if (!session.begun || session.next_offset != session.image_size) {
        reply(DFU_STATUS_BAD_STATE);
        return;
    }
    const uint32_t* app = (const uint32_t*)BOOT_APP_BASE;
    if (dfu_crc_words(session.first_word, &app[1], session.image_size / 4U - 1U) != session.image_crc) {
        reply(DFU_STATUS_VERIFY_ERROR);
        return;
    }
    if (!boot_vectors_are_valid(session.first_word, session.image_size)) {
        reply(DFU_STATUS_BAD_IMAGE);
        return;
    }
    set_begun(false);
    reply(dfu_flash_program_word(BOOT_APP_BASE, session.first_word) ? DFU_STATUS_OK : DFU_STATUS_FLASH_ERROR);
}

static void handle_boot(void) {
    if (!boot_app_is_valid()) {
        reply(DFU_STATUS_BAD_IMAGE);
        return;
    }
    reply(DFU_STATUS_OK);
    boot_start_app();
}

void dfu_run(void) {
    boot_feed_watchdog();
    session.reason = boot_take_request();

    uint32_t idle_timeout;
    if (!boot_app_is_valid()) {
        session.reason = BOOT_REASON_NO_APP;
        idle_timeout = DFU_FOREVER;
    } else if (session.reason == BOOT_REASON_REQUESTED) {
        idle_timeout = DFU_SESSION_TIMEOUT_MS;
    } else {
        idle_timeout = DFU_LISTEN_WINDOW_MS;
    }

    for (uint32_t last_activity = boot_ms;;) {
        uint32_t deadline = idle_timeout == DFU_FOREVER ? DFU_FOREVER : last_activity + idle_timeout;
        dfu_rx_result_t result = dfu_link_receive(deadline, &req);
        if (result == DFU_RX_TIMEOUT) {
            if (dfu_link_expired(deadline)) {
                if (boot_app_is_valid()) {
                    boot_start_app();
                }
                // The image was invalidated during this session: stay here.
                idle_timeout = DFU_FOREVER;
            }
            continue;
        }

        if (session.reason == BOOT_REASON_NONE) {
            // Only a well-formed HELLO may hold back the application during
            // the short listen window after an ordinary reset; line noise
            // must neither extend the window nor get answered.
            if (result != DFU_RX_OK || req.cmd != DFU_CMD_HELLO) {
                continue;
            }
            session.reason = BOOT_REASON_HOST;
        }
        last_activity = boot_ms;
        if (idle_timeout != DFU_FOREVER) {
            idle_timeout = DFU_SESSION_TIMEOUT_MS;
        }

        if (result == DFU_RX_BAD_CRC) {
            reply(DFU_STATUS_BAD_CRC);
            continue;
        }
        switch (req.cmd) {
            case DFU_CMD_HELLO:
                handle_hello();
                break;
            case DFU_CMD_BEGIN:
                handle_begin();
                break;
            case DFU_CMD_DATA:
                handle_data();
                break;
            case DFU_CMD_END:
                handle_end();
                break;
            case DFU_CMD_BOOT:
                handle_boot();
                break;
            default:  // DFU_CMD_ENTER: already here
                reply(DFU_STATUS_OK);
                break;
        }
    }
}
