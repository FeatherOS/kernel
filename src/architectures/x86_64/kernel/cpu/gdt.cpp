#include <cpu/gdt.hpp>

GDT::gdt_entry_t entries[5];
GDT::gdt_pointer_t pointer;

void GDT::init_gdt() {
    asm volatile ("cli"); // disable interrupts
    
    set_entry(0, 0, 0x00000000, 0x00, 0x0);
    set_entry(1, 0, 0xFFFFF,    0x9A, 0xA);
    set_entry(2, 0, 0xFFFFF,    0x92, 0xC);
    set_entry(3, 0, 0xFFFFF,    0xFA, 0xA);
    set_entry(4, 0, 0xFFFFF,    0xF2, 0xC);
    
    pointer.limit = sizeof(entries) - 1;
    pointer.base = (uint64_t) &entries;

    gdt_flush((uint64_t) &pointer);
}

void GDT::set_entry(unsigned long number, unsigned long base, unsigned long limit, unsigned char access, unsigned char granularity) {
    entries[number].base_low = (base & 0xFFFF);
    entries[number].base_middle = (base >> 16) & 0xFF;
    entries[number].base_high = (base >> 24) & 0xFF;
    entries[number].limit_low = (limit & 0xFFFF);
    entries[number].granularity = (limit >> 16) & 0x0F;
    entries[number].access = access;
}
