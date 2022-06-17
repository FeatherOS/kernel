#include <console/console.hpp>

namespace Console {
    void KernelTerminal::set_putchar_callback(void (*putchar)(char)) {
        this->putchar = putchar;
    }

    void KernelTerminal::puts(const char *str) {
        while (*str) {
            this->putchar(*str);
            str++;
        }
    }
}