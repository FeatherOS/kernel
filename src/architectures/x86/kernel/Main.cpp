#include <cpu/GDT.hpp>
#include <cpu/IDT.hpp>
#include <cpu/ISR.hpp>
#include <cpu/IRQ.hpp>

#include <multiboot.hpp>
#include <vga/tm_vt100.hpp>
#include <vga/textmode.hpp>

extern "C" void kernel_main(multiboot_info_t* mbinfo, unsigned int flag) {
    asm volatile("cli");
    VGA::TextMode::initialize_vt();
    Console::KernelTerminal *k_vt = &VGA::TextMode::virtual_terminal;
    k_vt->puts("Loading \x1B[1mGDT\x1b[0m... ");
    GDT::init();
    k_vt->puts("\x1B[92mOK\x1B[0m\nLoading \x1B[1mIDT\x1b[0m... ");
    IDT::init();
    k_vt->puts("\x1B[92mOK\x1B[0m\nLoading \x1B[1mISR\x1b[0m... ");
    ISR::init();
    k_vt->puts("\x1B[92mOK\x1B[0m\nLoading \x1B[1mIRQ\x1b[0m... (not finished) ");
    IRQ::init(); //TODO: code init function
    asm volatile("sti");
    asm volatile("int $0x5");
    //asm volatile("int $0x5");
    //asm volatile("int $0x5");
    //asm volatile("int $0x5");
    //asm volatile("int $0x20");
    k_vt->puts("\x1B[92mOK\x1B[0m\nHello World :D");
}