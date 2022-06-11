#pragma once


namespace GDT {
    void init_gdt();
    void set_entry(unsigned long number, unsigned long base, unsigned long limit, unsigned char access, unsigned char granularity);
        
    struct gdt_entry_t {
            unsigned short limit_low;
            unsigned short base_low;
            unsigned char base_middle;
            unsigned char access;
            unsigned char granularity;
            unsigned char base_high;
    } __attribute__((packed));

    struct gdt_pointer_t {
        unsigned short limit;
        unsigned int base;
    } __attribute__((packed));
        
    extern "C" {
        void gdt_flush(unsigned int);
    };
};
