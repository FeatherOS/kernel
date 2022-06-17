#include <cpu/GDT.hpp>
#include <cpu/IDT.hpp>
#include <cpu/ISR.hpp>
#include <cpu/IRQ.hpp>

#include <multiboot.hpp>
#include <vga/tm_vt100.hpp>
#include <vga/textmode.hpp>

#include <multiboot.hpp>

const char *itoa(int value, char *buffer, int radix, const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz") {
    if(value < 0) {
        *buffer++ = '-';
        value = -value;
    }
    int i = 0;
    do {
        buffer[i++] = digits[value % radix];
    } while ((value /= radix) > 0);
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char c = buffer[j];
        buffer[j] = buffer[k];
        buffer[k] = c;
    }
    return (const char *)(buffer + i - 1);
}

extern "C" void kernel_main(multiboot_info_t* mbinfo) {
    asm volatile("cli");
    VGA::TextMode::initialize_vt();
    Console::KernelTerminal *k_vt = &VGA::TextMode::virtual_terminal;
    k_vt->puts("Loading \x1B[1mGDT\x1b[0m... ");
    GDT::init();
    k_vt->puts("\x1B[92mOK\x1B[0m\nLoading \x1B[1mIDT\x1b[0m... ");
    IDT::init();
    k_vt->puts("\x1B[92mOK\x1B[0m\nLoading \x1B[1mISR\x1b[0m... (not finished)");
    ISR::init();
    k_vt->puts("\x1B[92mOK\x1B[0m\nLoading \x1B[1mIRQ\x1b[0m... (not finished) ");
    IRQ::init(); //TODO: code init function
    asm volatile("sti");
    /*asm volatile("mov ah, 0x0");
    asm volatile("mov al, 0x13");
    asm volatile("int 0x10");*/
    unsigned int address_of_module = mbinfo->mods_addr;
    char modCount[2] = {0};
    itoa(mbinfo->mods_count, modCount, 10);
    if(mbinfo->mods_count == 1) {
        k_vt->puts("\x1B[92mOK\x1B[0m\n1 Module loaded.");
    } else {
        k_vt->puts("\x1B[92mOK\x1B[0m\n");
        k_vt->puts(modCount);
        k_vt->puts(" Modules loaded.");
    }
    /*typedef void (*call_module_t)(void);
    call_module_t start_module = (call_module_t) address_of_module;
    start_module();*/
    k_vt->puts("\nHello World :D");
}