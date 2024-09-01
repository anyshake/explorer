#include "ads1262/regs/tdacp.h"

void ads1262_reg_set_tdacp(ads1262_reg_tdacp_t* tdacp) {
    uint8_t tdacp_data = __ADS1262_TDACP_DEFAULT_VALUE;
    tdacp_data &= ~(0x01 << 7);
    tdacp_data |= (tdacp->out_p & 0x01) << 7;
    tdacp_data &= ~0x1F;
    tdacp_data |= tdacp->mag_p & 0x1F;
    ads1262_write_reg(ADS1262_REG_TDACP, &tdacp_data);
}

void ads1262_reg_get_tdacp(ads1262_reg_tdacp_t* tdacp) {
    uint8_t tdacp_data = 0;
    ads1262_read_reg(ADS1262_REG_TDACP, &tdacp_data);
    tdacp->out_p = (tdacp_data & 0x80) >> 7;
    tdacp->mag_p = (tdacp_data & 0x1F);
}
