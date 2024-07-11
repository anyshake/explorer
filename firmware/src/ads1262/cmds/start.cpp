#include "ads1262/cmds/start.hpp"

void ads1262_cmd_start(const ads1262_ctl_pin_t* pin) {
    ads1262_write_cmd(pin, ADS1262_CMD_START1, NULL, 0, ADS1262_WRITE_CMD_WAIT_DISABLE);
}
