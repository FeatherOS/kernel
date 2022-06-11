#ifndef _CONSOLE__CONSOLE_HPP_
#define _CONSOLE__CONSOLE_HPP_

#include <stdint.h>
#include <stddef.h>

namespace Console {
    struct OCoordinates {
        size_t x;
        size_t y;
        size_t width;
        size_t height;
    };

    class KernelTerminal {
        public:
            void set_putchar_callback(void (*putchar)(char));

            // OCoordinates get_coordinates();
            void (*putchar)(char c);
            void puts(const char *str);
    };
}

#endif