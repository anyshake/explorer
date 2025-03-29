#include "ads1262/regs/interface.h"

ads1262_reg_interface_t ads1262_reg_new_interface(void) {
    ads1262_reg_interface_t interface;
    interface.timeout = ADS1262_REG_INTERFACE_TIMEOUT_DISABLED;
    interface.status = ADS1262_REG_INTERFACE_STATUS_ENABLED;
    interface.crc = ADS1262_REG_INTERFACE_CRC_CHECKSUM;
    return interface;
}

void ads1262_reg_set_interface(ads1262_reg_interface_t* interface) {
    uint8_t interface_data = __ADS1262_REG_INTERFACE_DEFAULT_VALUE;
    interface_data &= ~(0x01 << 3);
    interface_data |= (interface->timeout & 0x01) << 3;
    interface_data &= ~(0x01 << 2);
    interface_data |= (interface->status & 0x01) << 2;
    interface_data &= ~0x03;
    interface_data |= (interface->crc & 0x03);
    ads1262_write_reg(ADS1262_REG_INTERFACE, &interface_data);
}

void ads1262_reg_get_interface(ads1262_reg_interface_t* interface) {
    uint8_t interface_data = 0;
    ads1262_read_reg(ADS1262_REG_INTERFACE, &interface_data);
    interface->timeout = (interface_data >> 3) & 0x01;
    interface->status = (interface_data >> 2) & 0x01;
    interface->crc = interface_data & 0x03;
}
