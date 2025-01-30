#include "User/Inc/calibration.h"

void calibrate_adc_offset(ads1262_ctl_pin_t ctl_pin) {
    // Switch to continuous mode
    ads1262_reg_mode_0_t mode_0;
    ads1262_reg_get_mode_0(&mode_0);
    uint8_t orig_run_mode = mode_0.run_mode;
    mode_0.run_mode = ADS1262_REG_MODE_0_RUN_MODE_CONTINUOUS;
    ads1262_reg_set_mode_0(&mode_0);

    // Set input multiplexer to float mode
    ads1262_reg_inpmux_t ads1262_reg_inpmux = ads1262_reg_new_inpmux();
    ads1262_reg_inpmux.mux_n = ADS1262_REG_INPMUX_FLOAT;
    ads1262_reg_inpmux.mux_p = ADS1262_REG_INPMUX_FLOAT;
    ads1262_reg_set_inpmux(&ads1262_reg_inpmux);

    // Send command and wait for complete
    ads1262_cmd_start(ctl_pin);
    ads1262_cmd_sfocal(ctl_pin);
    while (mcu_utils_gpio_read(ctl_pin.drdy)) {
        ;
    }

    // Restore original configuration
    ads1262_cmd_stop(ctl_pin);
    mode_0.run_mode = orig_run_mode;
    ads1262_reg_set_mode_0(&mode_0);
}
