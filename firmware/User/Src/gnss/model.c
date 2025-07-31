#include "User/Inc/gnss/model.h"

void gnss_model_setup(bool is_rtos) {
#if GNSS_MODEL == QUECTEL_LC260Z
    // set_satellite_system: set satellite system to GPS L1, BDS B1, Galileo E1, QZSS L1, SBAS L1, BDS B1C enabled
    // set_update_interval: set NMEA message update interval to 0.2s to fix 1 second delay of GNRMC message on LC260Z00AANR01A02 firmware
    // reference: https://www.quectel.com.cn/download/quectel_lc26xz00lc76xz00_gnss_%E5%8D%8F%E8%AE%AE%E8%A7%84%E8%8C%83_v1-3 (login required)
    const char set_satellite_system[] = {0xF1, 0xD9, 0x06, 0x0C, 0x04, 0x00, 0x57, 0x01, 0x00, 0x00, 0x6E, 0xFB};
    const char set_update_interval[] = {0xF1, 0xD9, 0x06, 0x42, 0x14, 0x00, 0x00, 0x01, 0x35, 0x32, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8C, 0xD5};

    mcu_utils_uart2_write((uint8_t*)set_satellite_system, sizeof(set_satellite_system), true);
    mcu_utils_delay_ms(1000, is_rtos);
    mcu_utils_uart2_write((uint8_t*)set_update_interval, sizeof(set_update_interval), true);
    mcu_utils_delay_ms(1000, is_rtos);
#elif GNSS_MODEL == ZHONGKEWEI_ATGM332D
    // gnss_warm_start: warm start command for AT6558R based GNSS (simple workaround for RST doesn't work)
    // set_stationary_mode: enable stationary mode for AT6558R for higher accuracy
    // set_satellite_system: set satellite system to GPS L1, BDS B1 enabled
    // set_only_rmc_gga_enabled: enable only RMC and GGA messages to fix accumulated GNRMC time drift
    // reference: https://wiki.millerjs.org/atgm336h
    //            https://www.ebyte.com/Uploadfiles/Files/2024-11-14/20241114151416275.pdf
    //            https://web.archive.org/web/20241123025626if_/https://www.icofchina.com/d/file/xiazai/2020-09-22/26ac3f347aca9fc6c3a23db296b0dec0.zip
    const char gnss_warm_start[] = "$PCAS10,0*1C\r\n";
    const char set_stationary_mode[] = "$PCAS11,1*1C\r\n";
    const char set_satellite_system[] = "$PCAS04,3*1A\r\n";
    const char set_only_rmc_gga_enabled[] = "$PCAS03,1,0,0,0,1,0,0,1,0,0,0,0,0,0*03\r\n";

    mcu_utils_uart2_write((uint8_t*)gnss_warm_start, sizeof(gnss_warm_start), true);
    mcu_utils_delay_ms(1000, is_rtos);
    mcu_utils_uart2_write((uint8_t*)set_stationary_mode, sizeof(set_stationary_mode), true);
    mcu_utils_delay_ms(1000, is_rtos);
    mcu_utils_uart2_write((uint8_t*)set_satellite_system, sizeof(set_satellite_system), true);
    mcu_utils_delay_ms(1000, is_rtos);
    mcu_utils_uart2_write((uint8_t*)set_only_rmc_gga_enabled, sizeof(set_only_rmc_gga_enabled), true);
    mcu_utils_delay_ms(5000, is_rtos);
#endif
}
