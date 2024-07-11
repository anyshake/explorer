#include "ads1262/cmds/rdata.hpp"
#include "ads1262/regs/id.hpp"
#include "ads1262/regs/inpmux.hpp"
#include "ads1262/regs/mode_0.hpp"
#include "ads1262/regs/mode_1.hpp"
#include "ads1262/regs/mode_2.hpp"

#include "mcu_utils/delay.hpp"
#include "mcu_utils/led.hpp"
#include "mcu_utils/uart.hpp"

#include "settings.hpp"

const ads1262_ctl_pin_t pin = {
    .start = PA12,
    .drdy = PA11,
    .rst = PA1,
};

float getVoltage(ads1262_cmd_rdata_t* count) {
    return (float)(count->data * 3.3 / pow(2, (32 - 1)));
}

void setup() {
    // Initialize serial port
    mcu_utils_uart_init(115200);
    mcu_utils_led_blink(MCU_STATE, 5, false);

    // Initialize ADC
    ads1262_init(&pin, ADS1262_INIT_CONTROL_TYPE_HARD);
    ads1262_reset(&pin, ADS1262_RESET_RESET_TYPE_HARD);

    // Configure ADC registers
    ads1262_reg_mode_0_t mode_0 = {.run_mode = ADS1262_MODE_0_RUN_MODE_ONESHOT};
    ads1262_reg_set_mode_0(&mode_0);
    ads1262_reg_mode_1_t mode_1 = {.filter = ADS1262_MODE_1_FILTER_SINC1};
    ads1262_reg_set_mode_1(&mode_1);
    ads1262_reg_mode_2_t mode_2 = {.dr = ADS1262_MODE_2_DR_100};
    ads1262_reg_set_mode_2(&mode_2);
}

void loop() {
    // Initialize packet, mux, and channel data
    ads1262_reg_inpmux_t inpmux;
    ads1262_cmd_rdata_t count;

    // Read AIN0 - AIN1 channel
    inpmux = {.mux_p = ADS1262_INPMUX_AIN0, .mux_n = ADS1262_INPMUX_AIN1};
    ads1262_reg_set_inpmux(&inpmux);
    ads1262_cmd_rdata(&pin, &count, ADS1262_INIT_CONTROL_TYPE_HARD);
    Serial.print("AIN0 - AIN1: ");
    Serial.print(getVoltage(&count));
    Serial.print(" | ");

    // Read AIN2 - AIN3 channel
    inpmux = {.mux_p = ADS1262_INPMUX_AIN2, .mux_n = ADS1262_INPMUX_AIN3};
    ads1262_reg_set_inpmux(&inpmux);
    ads1262_cmd_rdata(&pin, &count, ADS1262_INIT_CONTROL_TYPE_HARD);
    Serial.print("AIN2 - AIN3: ");
    Serial.print(getVoltage(&count));
    Serial.print(" | ");

    // Read AIN4 - AIN5 channel
    inpmux = {.mux_p = ADS1262_INPMUX_AIN4, .mux_n = ADS1262_INPMUX_AIN5};
    ads1262_reg_set_inpmux(&inpmux);
    ads1262_cmd_rdata(&pin, &count, ADS1262_INIT_CONTROL_TYPE_HARD);
    Serial.print("AIN4 - AIN5: ");
    Serial.print(getVoltage(&count));
    Serial.print(" | ");

    // Read AIN9 - AINCOM channel
    inpmux = {.mux_p = ADS1262_INPMUX_AIN9, .mux_n = ADS1262_INPMUX_AINCOM};
    ads1262_reg_set_inpmux(&inpmux);
    ads1262_cmd_rdata(&pin, &count, ADS1262_INIT_CONTROL_TYPE_HARD);
    Serial.print("AIN9 - AINCOM: ");
    Serial.print(getVoltage(&count));
    Serial.println();
}
