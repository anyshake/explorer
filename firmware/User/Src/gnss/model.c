#include "User/Inc/gnss/model.h"

#if GNSS_MODEL == GENERIC

void gnss_model_setup(bool is_rtos) {
    ;
}

void gnss_model_factory_reset(bool is_rtos) {
    ;
}

int64_t gnss_model_handle_timestamp(int64_t timestamp) {
    return timestamp;
}

#elif GNSS_MODEL == QUECTEL_LC260Z || GNSS_MODEL == QUECTEL_LC760Z || GNSS_MODEL == QUECTEL_LC761Z
// reference: https://archive.org/download/quectel-lc-26x-z-lc-76x-z-00-gnss-v-1.4-1/Quectel_LC26xZ%E7%B3%BB%E5%88%97LC76xZ00_GNSS_%E5%8D%8F%E8%AE%AE%E8%A7%84%E8%8C%83_V1.4-1.pdf

void gnss_model_setup(bool is_rtos) {
    // disable GLL messages
    uint8_t set_gll_disabled[] = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x0D};
    mcu_utils_uart2_write(set_gll_disabled, sizeof(set_gll_disabled), true);
    mcu_utils_delay_ms(200, is_rtos);

    // disable GSA messages
    uint8_t set_gsa_disabled[] = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x02, 0x00, 0xFC, 0x13};
    mcu_utils_uart2_write(set_gsa_disabled, sizeof(set_gsa_disabled), true);
    mcu_utils_delay_ms(200, is_rtos);

    // disable GRS messages
    uint8_t set_grs_disabled[] = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x03, 0x00, 0xFD, 0x15};
    mcu_utils_uart2_write(set_grs_disabled, sizeof(set_grs_disabled), true);
    mcu_utils_delay_ms(200, is_rtos);

    // disable GSV messages
    uint8_t set_gsv_disabled[] = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x04, 0x00, 0xFE, 0x17};
    mcu_utils_uart2_write(set_gsv_disabled, sizeof(set_gsv_disabled), true);
    mcu_utils_delay_ms(200, is_rtos);

    // disable VTG messages
    uint8_t set_vtg_disabled[] = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x06, 0x00, 0x00, 0x1B};
    mcu_utils_uart2_write(set_vtg_disabled, sizeof(set_vtg_disabled), true);
    mcu_utils_delay_ms(200, is_rtos);

    // disable ZDA messages
    uint8_t set_zda_disabled[] = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x07, 0x00, 0x01, 0x1D};
    mcu_utils_uart2_write(set_zda_disabled, sizeof(set_zda_disabled), true);
    mcu_utils_delay_ms(200, is_rtos);

    // disable GST messages
    uint8_t set_gst_disabled[] = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x08, 0x00, 0x02, 0x1F};
    mcu_utils_uart2_write(set_gst_disabled, sizeof(set_gst_disabled), true);
    mcu_utils_delay_ms(200, is_rtos);

#if GNSS_MODEL != QUECTEL_LC261Z && GNSS_MODEL != QUECTEL_LC761Z  // exclude BDS-only constellation model
    // set satellite system to GPS L1, BDS B1, Galileo E1, QZSS L1, SBAS L1, BDS B1C enabled
    uint8_t set_satellite_system[] = {0xF1, 0xD9, 0x06, 0x0C, 0x04, 0x00, 0x57, 0x01, 0x00, 0x00, 0x6E, 0xFB};
    mcu_utils_uart2_write(set_satellite_system, sizeof(set_satellite_system), true);
    mcu_utils_delay_ms(200, is_rtos);
#endif

    // set NMEA message update interval to 0.2s to fix 1 second delay of GNRMC message on LC260Z00AANR01A02 firmware
    uint8_t set_update_interval[] = {0xF1, 0xD9, 0x06, 0x42, 0x14, 0x00, 0x00, 0x01, 0x35, 0x32, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8C, 0xD5};
    mcu_utils_uart2_write(set_update_interval, sizeof(set_update_interval), true);
    mcu_utils_delay_ms(1000, is_rtos);
}

void gnss_model_factory_reset(bool is_rtos) {
    uint8_t factory_reset[] = {0xF1, 0xD9, 0x06, 0x09, 0x08, 0x00, 0x02, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x15, 0x01};
    mcu_utils_uart2_write(factory_reset, sizeof(factory_reset), true);
}

int64_t gnss_model_handle_timestamp(int64_t timestamp) {
    return (timestamp + 500) / 1000 * 1000;
}

#elif GNSS_MODEL == ZHONGKEWEI_ATGM336H || GNSS_MODEL == ZHONGKEWEI_ATGM332D || GNSS_MODEL == QUECTEL_L76K
// reference: https://wiki.millerjs.org/atgm336h
// reference: https://web.archive.org/web/20250812133550if_/https://www.ebyte.com/Uploadfiles/Files/2024-11-14/20241114151416275.pdf
// GNSS tool: https://web.archive.org/web/20241123025626if_/https://www.icofchina.com/d/file/xiazai/2020-09-22/26ac3f347aca9fc6c3a23db296b0dec0.zip

void gnss_model_setup(bool is_rtos) {
    // warm start command for AT6558R based GNSS (simple workaround for RST doesn't work)
    uint8_t gnss_warm_start[] = "$PCAS10,0*1C\r\n";
    mcu_utils_uart2_write(gnss_warm_start, sizeof(gnss_warm_start), true);
    mcu_utils_delay_ms(500, is_rtos);

    // enable stationary mode for AT6558R for higher accuracy
    uint8_t set_stationary_mode[] = "$PCAS11,1*1C\r\n";
    mcu_utils_uart2_write(set_stationary_mode, sizeof(set_stationary_mode), true);
    mcu_utils_delay_ms(500, is_rtos);

    // set satellite system to GPS L1, BDS B1 enabled
    uint8_t set_satellite_system[] = "$PCAS04,3*1A\r\n";
    mcu_utils_uart2_write(set_satellite_system, sizeof(set_satellite_system), true);
    mcu_utils_delay_ms(500, is_rtos);

    // enable only RMC and GGA messages to fix accumulated GNRMC time drift
    uint8_t set_only_rmc_gga_enabled[] = "$PCAS03,1,0,0,0,1,0,0,1,0,0,0,0,0,0*03\r\n";
    mcu_utils_uart2_write(set_only_rmc_gga_enabled, sizeof(set_only_rmc_gga_enabled), true);
    mcu_utils_delay_ms(1000, is_rtos);
}

void gnss_model_factory_reset(bool is_rtos) {
    uint8_t factory_reset[] = "$PCAS10,3*1F\r\n";
    mcu_utils_uart2_write(factory_reset, sizeof(factory_reset), true);
}

int64_t gnss_model_handle_timestamp(int64_t timestamp) {
    return timestamp;
}

#elif GNSS_MODEL == ALLYSTAR_TAU812S || GNSS_MODEL == ALLYSTAR_TAU1113 || GNSS_MODEL == ALLYSTAR_TAU1114
// reference: https://archive.org/download/4c-6k-4oyd-63bikyc-9c-33jgbz-19kc-1aois/4c6k4oyd63bikyc9c33jgbz19kc1aois.pdf
// reference: https://archive.org/download/tau-1113/TAU1113%E6%A8%A1%E7%BB%84%E8%A7%84%E6%A0%BC%E4%B9%A6_%E6%B6%82%E9%B8%A6%E5%BC%80%E5%8F%91%E8%80%85%E5%B9%B3%E5%8F%B0_%E6%B6%82%E9%B8%A6%E5%BC%80%E5%8F%91%E8%80%85%E5%B9%B3%E5%8F%B0.pdf
// note: the command format of TAU812S, TAU1113, and TAU1114 are very similar to those of the Quectel LC26xZ series, and it has been verified that most of them are universal.

void gnss_model_setup(bool is_rtos) {
    // disable GLL messages
    uint8_t set_gll_disabled[] = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x0D};
    mcu_utils_uart2_write(set_gll_disabled, sizeof(set_gll_disabled), true);
    mcu_utils_delay_ms(200, is_rtos);

    // disable GSA messages
    uint8_t set_gsa_disabled[] = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x02, 0x00, 0xFC, 0x13};
    mcu_utils_uart2_write(set_gsa_disabled, sizeof(set_gsa_disabled), true);
    mcu_utils_delay_ms(200, is_rtos);

    // disable GRS messages
    uint8_t set_grs_disabled[] = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x03, 0x00, 0xFD, 0x15};
    mcu_utils_uart2_write(set_grs_disabled, sizeof(set_grs_disabled), true);
    mcu_utils_delay_ms(200, is_rtos);

    // disable GSV messages
    uint8_t set_gsv_disabled[] = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x04, 0x00, 0xFE, 0x17};
    mcu_utils_uart2_write(set_gsv_disabled, sizeof(set_gsv_disabled), true);
    mcu_utils_delay_ms(200, is_rtos);

    // disable VTG messages
    uint8_t set_vtg_disabled[] = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x06, 0x00, 0x00, 0x1B};
    mcu_utils_uart2_write(set_vtg_disabled, sizeof(set_vtg_disabled), true);
    mcu_utils_delay_ms(200, is_rtos);

    // disable ZDA messages
    uint8_t set_zda_disabled[] = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x07, 0x00, 0x01, 0x1D};
    mcu_utils_uart2_write(set_zda_disabled, sizeof(set_zda_disabled), true);
    mcu_utils_delay_ms(200, is_rtos);

    // disable GST messages
    uint8_t set_gst_disabled[] = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x08, 0x00, 0x02, 0x1F};
    mcu_utils_uart2_write(set_gst_disabled, sizeof(set_gst_disabled), true);
    mcu_utils_delay_ms(200, is_rtos);

    // set GPS L1, BDS B1 enabled
    uint8_t set_satellite_system[] = {0xF1, 0xD9, 0x06, 0x0C, 0x04, 0x00, 0x05, 0x00, 0x00, 0x00, 0x1B, 0xB0};
    mcu_utils_uart2_write(set_satellite_system, sizeof(set_satellite_system), true);
    mcu_utils_delay_ms(500, is_rtos);
}

void gnss_model_factory_reset(bool is_rtos) {
    uint8_t factory_reset[] = {0xF1, 0xD9, 0x06, 0x09, 0x08, 0x00, 0x02, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x15, 0x01};
    mcu_utils_uart2_write(factory_reset, sizeof(factory_reset), true);
}

int64_t gnss_model_handle_timestamp(int64_t timestamp) {
    return timestamp;
}

#else
#error "Unknown GNSS model"
#endif
