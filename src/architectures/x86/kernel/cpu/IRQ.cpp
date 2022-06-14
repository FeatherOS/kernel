#include <cpu/IRQ.hpp>
#include <IO.hpp>
#include <cpu/IDT.hpp>

#include <vga/tm_vt100.hpp>
#include <vga/textmode.hpp>

void IRQ::init() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
    //TODO: IRQ init method
}

extern "C" void IRQ::irq_handler(ISR::registers_t registers) {
    //TODO: code handler
}