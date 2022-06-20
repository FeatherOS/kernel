#include <cpu/IDT.hpp>

IDT::idt_entry_t idt_entries[256];
IDT::idt_pointer_t idt_pointer;

void IDT::init() {
    idt_pointer.size = sizeof(idt_entries) * 256 - 1;
    idt_pointer.offset = (uint32_t) &idt_entries;
    asm("lidt (%0)\n" :: "r" (&idt_pointer));
}
void IDT::set_gate(uint8_t number, uint32_t base, uint16_t selector, uint8_t flags) {
    idt_entries[number].base_low = base & 0xFFFF;
    idt_entries[number].base_high = base >> 16;
    idt_entries[number].selector = selector;
    idt_entries[number].zero = 0;
    idt_entries[number].flags = flags;
}
void IDT::flush() {
    asm("lidt (%0)\n" :: "r" (&idt_pointer));
}