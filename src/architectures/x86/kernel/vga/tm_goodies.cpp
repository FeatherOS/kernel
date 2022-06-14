#include <vga/tm_goodies.hpp>
#include <vga/registers.hpp>

#include <basic.hpp>

namespace VGA::TextMode {
    Cell *buffer_copy(Cell *buffer_location) {
        memcpy(buffer_location, (void *)0xB8000, VGA_TXT_SZ * sizeof(Cell));
        return buffer_location;
    }

    void buffer_paste(Cell *buffer_copy) {
        memcpy(buffer, buffer_copy, VGA_TXT_SZ * sizeof(Cell));
    }

    void clear_line(uint8_t y, Cell *buffer_location, Color::color_t fg, Color::color_t bg) {
        uint8_t max_x = Cursor::get_max_x();
        uint16_t position = Cursor::from_coordinates(0, y);

        for (uint8_t x = 0; x < max_x; x++) {
            if (position == VGA_TXT_SZ - 1) break;
            buffer_location[position].codepoint = ' ';
            buffer_location[position].foreground = fg;
            buffer_location[position].background = bg;
            position++;
        }
    }

    void scroll(Color::color_t fg, Color::color_t bg) {
        // To clear the screen smoothly, we need to copy the screen content into a buffer
        // with a size of VGA_TXT_SZ + Cursor::max_x.

        // We need to set the starting position of the buffer to the second line,
        // then we need to clear the last line of the buffer.

        // After that, we need to copy the buffer into the screen.


        // First, we need to copy the screen content into a buffer with a size of VGA_TXT_SZ + Cursor::max_x.
        Cell copy_buffer[VGA_TXT_SZ + Cursor::max_x];
        buffer_copy(copy_buffer);

        // Then, we need to move the beginning of the buffer to the second line.
        Cell *new_buffer = copy_buffer + Cursor::max_x;

        // Then, we need to clear the last line of the buffer.
        clear_line(Cursor::max_y - 1, new_buffer, fg, bg);

        // Finally, we need to copy the buffer into the screen.
        buffer_paste(new_buffer);
    }

    void clear(Color::color_t default_fg, Color::color_t default_bg, bool reset_cursor) {
        Cell clean_cell;
        clean_cell.codepoint = ' ';
        clean_cell.background = default_bg;
        clean_cell.foreground = default_fg;

        for (uint16_t i = 0; i < VGA_TXT_SZ; i++) {
            buffer[i] = clean_cell;
        }

        if (reset_cursor) {
            Cursor::set(0);
        }
    }

    namespace Cursor {
        void move_up(uint8_t n) {
            uint8_t x = get_x();
            uint8_t y = get_y();
            if (y > n) {
                y -= n;
            } else {
                y = 0;
            }
            setc(x, y);
        }

        void move_down(uint8_t n) {
            uint8_t x = get_x();
            uint8_t y = get_y();
            if (y > n) {
                y += n;
            } else {
                y = get_max_y() - 1;
            }
            setc(x, y);
        }

        void move_left(uint8_t n) {
            uint8_t x = get_x();
            uint8_t y = get_y();
            if (y > n) {
                x -= n;
            } else {
                x = 0;
            }
            setc(x, y);
        }

        void move_right(uint8_t n) {
            uint8_t x = get_x();
            uint8_t y = get_y();
            if (y > n) {
                x -= n;
            } else {
                x = get_max_x() - 1;
            }
            setc(x, y);
        }
    }

    namespace Text {
        void set_blinking(bool enabled) {
            uint8_t reg = Registers::read(0x3C0, 0x30);

            // Set bit 3 to the value of the `enabled` parameter
            reg = (reg & 0xF7) | (enabled << 3);

            Registers::write(0x3C0, 0x30, reg);
        }

        bool is_blinking_enabled() {
            uint8_t reg = Registers::read(0x3C0, 0x30);

            // Check if bit 3 is set
            return (reg & 0x08) >> 3;
        }
    }

    namespace Color {
        color_t to_intensive(color_t color) {
            if (color <= 7) {
                return (color_t)(color + 8);
            } else {
                return color;
            }
        }

        color_t to_extensive(color_t color) {
            if (color >= 7) {
                return (color_t)(color - 8);
            } else {
                return color;
            }
        }

        color_t to_inverted(color_t color) {
            return ~color;
        }
    }
}