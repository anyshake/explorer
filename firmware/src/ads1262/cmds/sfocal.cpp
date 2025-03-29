#include "ads1262/cmds/sfocal.h"

void ads1262_cmd_sfocal(ads1262_ctl_pin_t pin) {
    ads1262_write_cmd(pin, ADS1262_CMD_SFOCAL1, NULL, 1, ADS1262_WRITE_CMD_WAIT_DISABLE);
}
