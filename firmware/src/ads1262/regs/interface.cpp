#include "ads1262/regs/interface.hpp"

void adc_reg_set_interface(adc_reg_interface_t* interface) {
    uint8_t interface_data = __ADC_INTERFACE_DEFAULT_VALUE;
    interface_data &= ~(0x01 << 3);
    interface_data |= (interface->timeout & 0x01) << 3;
    interface_data &= ~(0x01 << 2);
    interface_data |= (interface->status & 0x01) << 2;
    interface_data &= ~0x03;
    interface_data |= interface->crc & 0x03;
    adc_write_reg(ADC_REG_INTERFACE, &interface_data);
}

void adc_reg_get_interface(adc_reg_interface_t* interface) {
    uint8_t interface_data = 0;
    adc_read_reg(ADC_REG_INTERFACE, &interface_data);
    interface->timeout = (interface_data & 0x08) >> 3;
    interface->status = (interface_data & 0x04) >> 2;
    interface->crc = interface_data & 0x03;
}
