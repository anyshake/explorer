#include "ads1262/cmds/syocal.hpp"

void adc_cmd_syocal() {
    adc_write_cmd(ADC_CMD_SYOCAL1, NULL, 1, ADC_WRITE_CMD_WAIT_DISABLE);
}
