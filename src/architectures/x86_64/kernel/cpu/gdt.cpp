#include <stdint.h>

#include <cpu/gdt.hpp>

GDT::gdt_entry_t gdt_entries[5];
GDT::gdt_pointer_t gdt_pointer;

void GDT::init_gdt() {
    asm volatile("cli");
    set_entry(0, 0, 0x00000000, 0x00, 0x0);
    set_entry(1, 0, 0xFFFFF,    0x9A, 0xA);
    set_entry(2, 0, 0xFFFFF,    0x92, 0xC);
    set_entry(3, 0, 0xFFFFF,    0xFA, 0xA);
    set_entry(4, 0, 0xFFFFF,    0xF2, 0xC);
    gdt_pointer.limit = sizeof(gdt_entries) - 1;
    gdt_pointer.base = (uint64_t) &gdt_entries;
    gdt_flush((uint64_t) &gdt_pointer);
}
void GDT::set_entry(unsigned long number, unsigned long base, unsigned long limit, unsigned char access, unsigned char granularity) {
    gdt_entries[number].base_low = (base & 0xFFFF);
    gdt_entries[number].base_middle = (base >> 16) & 0xFF;
    gdt_entries[number].base_high = (base >> 24) & 0xFF;
    gdt_entries[number].limit_low = (limit & 0xFFFF);
    gdt_entries[number].granularity = (limit >> 16) & 0x0F;
    gdt_entries[number].access = access;
}