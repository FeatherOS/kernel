#include <stdint.h>

#include <cpu/GDT.hpp>

GDT::gdt_entry_t gdt_entries[6];
GDT::gdt_pointer_t gdt_pointer;
//GDT::tss_entry_t tss;

void GDT::init() {
    gdt_pointer.size = sizeof(gdt_entries) - 1;
    gdt_pointer.offset = (uint32_t) &gdt_entries;
    set_entry(0, 0, 0, 0, 0);
    set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
    //TODO: set tss entry
    gdt_flush((uint32_t) &gdt_pointer);
    //TODO: flush tss
}

void GDT::set_entry(uint64_t number, uint64_t base, uint64_t limit, uint8_t access, uint8_t granularity) {
    gdt_entries[number].base_low = (base & 0xFFFF);
    gdt_entries[number].base_middle = (base >> 16) & 0xFF;
    gdt_entries[number].base_high = (base >> 24) & 0xFF;
    gdt_entries[number].limit_low = (limit & 0xFFFF);
    gdt_entries[number].granularity = (limit >> 16) & 0x0F;
    gdt_entries[number].granularity |= granularity & 0xF0;
    gdt_entries[number].access = access;
}

/*void GDT::write_tss(uint32_t number, uint32_t ss0, uint32_t esp0) {
    uintptr_t offset = (uintptr_t) &tss;
    uintptr_t size = offset + sizeof(tss);
    set_entry(number, offset, size, 0xE9, 0x00);
    memset(&tss, 0x00, sizeof(tss));
    tss.ss0 = ss0;
    tss.esp0 = esp0;
    tss.iomap_base = sizeof(tss);
}*/