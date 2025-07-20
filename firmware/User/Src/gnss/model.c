#include "User/Inc/gnss/model.h"

void gnss_model_reset(bool is_rtos) {
#if GNSS_MODEL == QUECTEL_LC260Z
    // set_satellite_system: set satellite system to GPS L1, BDS B1, Galileo E1, QZSS L1, SBAS L1, BDS B1C enabled
    // set_update_interval: set NMEA message update interval to 0.2s
    // reference: https://www.quectel.com.cn/download/quectel_lc26xz00lc76xz00_gnss_%E5%8D%8F%E8%AE%AE%E8%A7%84%E8%8C%83_v1-3
    const char set_satellite_system[] = {0xF1, 0xD9, 0x06, 0x0C, 0x04, 0x00, 0x57, 0x01, 0x00, 0x00, 0x6E, 0xFB};
    const char set_update_interval[] = {0xF1, 0xD9, 0x06, 0x42, 0x14, 0x00, 0x00, 0x01, 0x35, 0x32, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8C, 0xD5};

    mcu_utils_uart2_write((uint8_t*)set_satellite_system, sizeof(set_satellite_system), true);
    mcu_utils_delay_ms(1000, is_rtos);
    mcu_utils_uart2_write((uint8_t*)set_update_interval, sizeof(set_update_interval), true);
    mcu_utils_delay_ms(1000, is_rtos);
#elif GNSS_MODEL == ZHONGKEWEI_ATGM332D
    // gnss_warm_start: warm start command for AT6558R based GNSS
    // set_satellite_system: set satellite system to GPS L1, BDS B1, Galileo E1, QZSS L1, SBAS L1, BDS B1C enabled
    // reference: https://wiki.millerjs.org/atgm336h
    const char gnss_warm_start[] = "$PCAS00,0*1C\r\n";
    const char set_satellite_system[] = "$PCAS04,3*1A\r\n";

    mcu_utils_uart2_write((uint8_t*)gnss_warm_start, sizeof(set_satellite_system), true);
    mcu_utils_delay_ms(1000, is_rtos);
    mcu_utils_uart2_write((uint8_t*)set_satellite_system, sizeof(set_satellite_system), true);
    mcu_utils_delay_ms(5000, is_rtos);
#endif
}
