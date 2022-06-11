#ifndef _CONSOLE__VGA_VT100_HPP_
#define _CONSOLE__VGA_VT100_HPP_

#include <stdint.h>
#include <console/console.hpp>
#include <console/vt100.hpp>

namespace VGA::TextMode {
    extern void initialize_vt();
    extern Console::KernelTerminal virtual_terminal;
}

#endif