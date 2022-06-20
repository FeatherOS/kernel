#include <console/vt100.hpp>

void VT100::end_of_command() {
    for (int i = 0; i < VT100_MAXIMUM_ARGUMENT_COUNT; i++) {
        this->arguments[i] = 0;
    }

    this->argcount = 0;
    this->argument_passed = false;
    this->escape_sequence_active = false;
}

void VT100::call_command(char command) {
    if (this->csi_commands[(int8_t)command] != nullptr) {
        this->csi_commands[(int8_t)command](arguments, argcount, this->command_output_handler);
    }

    this->end_of_command();
}

void VT100::putchar(char c) {
    switch (c) {
        case VT100_ESCAPE_CHARACTER: {
            this->escape_sequence_active = true;
            break;
        }

        case VT100_ARGUMENT_CSI: {
            if (this->escape_sequence_active) {
                this->command_interpret_in_progress = true;
            } else {
                this->putchar_callback(c);
            }

            break;
        }

        case VT100_ARGUMENT_DELIMITER: {
            if (this->escape_sequence_active && !this->command_interpret_in_progress) {
                this->putchar_callback(VT100_ESCAPE_CHARACTER);
                this->putchar_callback(VT100_ARGUMENT_DELIMITER);
                this->escape_sequence_active = false;
            } else {
                if (this->escape_sequence_active) {
                    this->argcount++;
                } else {
                    this->putchar_callback(c);
                }
            }

            break;
        }

        case '0' ... '9': {
            if (this->escape_sequence_active && !this->command_interpret_in_progress) {
                this->putchar_callback(VT100_ESCAPE_CHARACTER);
                this->putchar_callback(c);
                this->escape_sequence_active = false;
            } else {
                if (this->escape_sequence_active) {
                    this->arguments[this->argcount] *= 10;
                    this->arguments[this->argcount] += c - '0';
                    this->argument_passed = true;
                } else {
                    this->putchar_callback(c);
                }
            }

            break;
        }

        default: {
            if (this->escape_sequence_active && !this->command_interpret_in_progress) {
                this->putchar_callback(VT100_ESCAPE_CHARACTER);
                this->putchar_callback(c);
                this->escape_sequence_active = false;
            } else {
                if (!this->escape_sequence_active) {
                    this->putchar_callback(c);
                    return;
                }

                if (this->argument_passed) {
                    argcount++; // increment argcount to account for the argument which was not separated by a delimiter
                }

                this->command_interpret_in_progress = false;
                this->call_command(c);
            }

            break;
        }
    }
}


void VT100::add_csi_command(char command, void (*callback)(size_t arguments[VT100_MAXIMUM_ARGUMENT_COUNT], size_t argcount, void (*output)(char c))) {
    this->csi_commands[(int8_t)command] = callback;
}

void VT100::remove_csi_command(char command) {
    this->csi_commands[(int8_t)command] = nullptr;
}

void VT100::set_command_output_handler(void (*callback)(char c)) {
    this->command_output_handler = callback;
}

void VT100::set_putchar_callback(void (*callback)(char c)) {
    this->putchar_callback = callback;
}