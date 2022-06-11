#ifndef _CONSOLE__VT100_HPP_
#define _CONSOLE__VT100_HPP_

#include <stdint.h>
#include <stddef.h>

#define VT100_MAXIMUM_ARGUMENT_COUNT 10
#define VT100_ESCAPE_CHARACTER       0x1B
#define VT100_ARGUMENT_DELIMITER     ';'
#define VT100_ARGUMENT_CSI           '['

#define VT100_GET_PUTCHAR_PTR(vt100_instance) (void (*)(char))([](char c) { vt100_instance.putchar(c); })

class VT100 {
    private:
        size_t arguments[VT100_MAXIMUM_ARGUMENT_COUNT] = {0};
        size_t argcount = 0;

        bool escape_sequence_active = false;
        bool argument_passed = false;
        bool command_interpret_in_progress = false;
        
        void (*command_output_handler)(char c) = nullptr;
        void (*putchar_callback)(char c)       = nullptr;

        void (*csi_commands[INT8_MAX])(size_t arguments[VT100_MAXIMUM_ARGUMENT_COUNT], size_t argcount, void (*output)(char c)) = {nullptr};

        void end_of_command();
        void call_command(char command);
    public:
        void putchar(char c);

        void add_csi_command(char command, void (*callback)(size_t arguments[VT100_MAXIMUM_ARGUMENT_COUNT], size_t argcount, void (*output)(char c)));
        void remove_csi_command(char command);
        void add_csi_command_alias(char command, char alias);

        void set_command_output_handler(void (*callback)(char c));
        void set_putchar_callback(void (*callback)(char c));
};

#endif