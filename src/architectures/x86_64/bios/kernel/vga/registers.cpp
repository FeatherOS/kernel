#include <IO.hpp>

#include <vga/registers.hpp>


namespace VGA::Registers {
    uint8_t read(uint16_t port, uint16_t reg) {
        // To read from a VGA register, we need to write the register's address to the index register,
        // then read the value from the data register.

        if (port == 0x3C0) {
            // We don't know if the 0x3C0 is responsible for data or address, so we need to reset
            // it to address mode. We can do this by reading from the I/O port 0x3DA.

            inb(0x3DA);
        }

        // Write the register's address to the index register
        outb(port, reg);

        // Read the value from the data register
        return inb(port + 1);
    }

    void write(uint16_t port, uint16_t reg, uint8_t value) {
        // To write to a VGA register, we need to write the register's address to the index register,
        // then write the value to the data register.

        if (port == 0x3C0) {
            // We don't know if the 0x3C0 is responsible for data or address, so we need to reset
            // it to address mode. We can do this by reading from the I/O port 0x3DA.

            inb(0x3DA);
        }

        // Write the register's address to the index register
        outb(port, reg);

        // Write the value to the data register
        if (port == 0x3C0) {
            outb(port, value);
        } else {
            outb(port + 1, value);
        }
    }
}