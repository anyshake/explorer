#ifndef __SETTINGS_HPP
#define __SETTINGS_HPP

#define MCU_STATE LED_BUILTIN

#define PACKET_SIZE 4
#define RESET_WORD 0x61

const uint8_t ACK_WORDS[] = {
    0xFC,
    0x2B,
};
const uint8_t SYNC_WORDS[] = {
    0xFC,
    0x1B,
};

#endif
