#include "ads1262/cmds/rdata.hpp"
#include "ads1262/regs/id.hpp"
#include "ads1262/regs/inpmux.hpp"
#include "ads1262/regs/mode_0.hpp"
#include "ads1262/regs/mode_1.hpp"
#include "ads1262/regs/mode_2.hpp"
#include "checksum.hpp"
#include "mcu_utils/delay.hpp"
#include "mcu_utils/led.hpp"
#include "mcu_utils/uart.hpp"
#include "packet.hpp"
#include "settings.hpp"

uint8_t should_reset() {
    adc_reg_id_t id;
    adc_reg_get_id(&id);
    uint8_t is_ads1262 = id.dev_id == ADC_ID_DEV_ID_ADS1262;
    uint8_t has_reset_cmd = uart_hasdata() && uart_readch() == RESET_WORD;
    return !is_ads1262 || has_reset_cmd;
}

void setup() {
    // Initialize MCU & Peripherals
    uart_init(MCU_UART_RATE);
    adc_init(ADC_INIT_CONTROL_TYPE_HARD);
    adc_reset(ADC_RESET_RESET_TYPE_HARD);
    led_blink(PIN_MCU_STATE, 5);

    // Configure ADC registers
    adc_reg_mode_0_t mode_0 = {.run_mode = ADC_MODE_0_RUN_MODE_ONESHOT};
    adc_reg_set_mode_0(&mode_0);
    adc_reg_mode_1_t mode_1 = {.filter = ADC_MODE_1_FILTER_SINC3};
    adc_reg_set_mode_1(&mode_1);
    adc_reg_mode_2_t mode_2 = {.dr = ADC_MODE_2_DR_400};
    adc_reg_set_mode_2(&mode_2);
}

void loop() {
    // Initialize packet, mux, and channel data
    packet_t packet;
    adc_reg_inpmux_t inpmux;
    adc_cmd_rdata_t EHZ, EHE, EHN;
    // Collect channel data
    for (uint8_t i = 0; i < PACKET_SIZE; i++) {
        // Support runtime reset
        if (should_reset()) {
            send_word_packet(ACK_WORDS, sizeof(ACK_WORDS));
            setup();
        }
        // Read EHZ channel
        inpmux = {.mux_p = ADC_INPMUX_AIN0, .mux_n = ADC_INPMUX_AIN1};
        adc_reg_set_inpmux(&inpmux);
        adc_cmd_rdata(&EHZ, ADC_INIT_CONTROL_TYPE_HARD);
        packet.EHZ[i] = EHZ.data >> 5;
        // Read EHE channel
        inpmux = {.mux_p = ADC_INPMUX_AIN2, .mux_n = ADC_INPMUX_AIN3};
        adc_reg_set_inpmux(&inpmux);
        adc_cmd_rdata(&EHE, ADC_INIT_CONTROL_TYPE_HARD);
        packet.EHE[i] = EHE.data >> 5;
        // Read EHN channel
        inpmux = {.mux_p = ADC_INPMUX_AIN4, .mux_n = ADC_INPMUX_AIN5};
        adc_reg_set_inpmux(&inpmux);
        adc_cmd_rdata(&EHN, ADC_INIT_CONTROL_TYPE_HARD);
        packet.EHN[i] = EHN.data >> 5;
    }
    // Get checksum for each channel
    packet.checksum[0] = get_checksum(packet.EHZ, PACKET_SIZE);
    packet.checksum[1] = get_checksum(packet.EHE, PACKET_SIZE);
    packet.checksum[2] = get_checksum(packet.EHN, PACKET_SIZE);
    // Send sync word and data packet
    send_word_packet(SYNC_WORDS, sizeof(SYNC_WORDS));
    send_data_packet(packet);
}
