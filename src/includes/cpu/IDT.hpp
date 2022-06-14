#pragma once

#include <stdint.h>

namespace IDT {
    struct idt_entry_t {
        uint16_t base_low;
        uint16_t selector;
        uint8_t zero;
        uint8_t flags;
        uint16_t base_high;
    }__attribute__((packed));
    struct idt_pointer_t {
        uint16_t size;
        uint32_t offset;
    }__attribute__((packed));
    
    void init();
    void flush();
    void set_gate(uint8_t number, uint32_t base, uint16_t selector, uint8_t flags);
}