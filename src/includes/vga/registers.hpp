#ifndef _VGA__REGISTERS_HPP_
#define _VGA__REGISTERS_HPP_

#include <stdint.h>

namespace VGA::Registers {
    uint8_t read(uint16_t port, uint16_t reg);
    void write(uint16_t port, uint16_t reg, uint8_t value);
}

#endif