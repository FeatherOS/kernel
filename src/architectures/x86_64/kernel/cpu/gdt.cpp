#include <cpu/gdt.hpp>

GDT::gdt_entry_t entries[3];
GDT::gdt_pointer_t pointer;

void GDT::init_gdt() {
}
void GDT::set_entry(unsigned long number, unsigned long base, unsigned long limit, unsigned char access, unsigned char granularity) {
    entries[number].base_low = (base & 0xFFFF);
    entries[number].base_middle = (base >> 16) & 0xFF;
    entries[number].base_high = (base >> 24) & 0xFF;
    entries[number].limit_low = (limit & 0xFFFF);
    entries[number].granularity = (limit >> 16) & 0x0F;
    entries[number].access = access;
}