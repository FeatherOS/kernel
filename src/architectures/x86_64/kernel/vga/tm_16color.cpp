#include <vga/textmode.hpp>
#include <vga/registers.hpp>
#include <IO.hpp>
#include <basic.hpp>
#include <stddef.h>

namespace VGA::TextMode {
    namespace Color {
        color_t operator~(color_t color) {
            return (color_t)(color ^ 0x0F);
        }


        void set(color_t foreground, color_t background, uint16_t position) {
            buffer[position].color = (background << 4) | foreground;
        }

        void setc(color_t foreground, color_t background, uint8_t x, uint8_t y) {
            set(foreground, background, Cursor::from_coordinates(x, y));
        }

        void seta(uint8_t attribute, uint16_t position) {
            buffer[position].color = attribute;
        }

        void setac(uint8_t attribute, uint8_t x, uint8_t y) {
            seta(attribute, Cursor::from_coordinates(x, y));
        }


        color_t get_fg(uint16_t position) {
            return (color_t)(buffer[position].foreground);
        }

        color_t getc_fg(uint8_t x, uint8_t y) {
            return get_fg(Cursor::from_coordinates(x, y));
        }

        color_t get_bg(uint16_t position) {
            return (color_t)(buffer[position].background);
        }

        color_t getc_bg(uint8_t x, uint8_t y) {
            return get_bg(Cursor::from_coordinates(x, y));
        }

        uint8_t geta(uint16_t position) {
            return buffer[position].color;
        }

        uint8_t getac(uint8_t x, uint8_t y) {
            return geta(Cursor::from_coordinates(x, y));
        }
    }

    namespace Cursor {
        uint8_t max_x = 80;
        uint8_t max_y = 25;

        uint8_t get_max_x() {
            return max_x;
        }

        uint8_t get_max_y() {
            return max_y;
        }

        uint16_t from_coordinates(uint8_t x, uint8_t y) {
            return y * max_x + x;
        }

        bool is_valid(uint16_t position) {
            return position < max_x * max_y;
        }


        void set(uint16_t position) {
            VGA::Registers::write(0x3D4, 0x0F, position & 0xFF);
            VGA::Registers::write(0x3D4, 0x0E, (position >> 8) & 0xFF);
        }
        
        void setc(uint8_t x, uint8_t y) {
            set(from_coordinates(x, y));
        }


        uint16_t get() {
            // Thanks to https://wiki.osdev.org/Text_Mode_Cursor
            uint8_t lower = VGA::Registers::read(0x3D4, 0x0F);
            uint8_t upper = VGA::Registers::read(0x3D4, 0x0E);

            return lower | (upper << 8);
        }

        uint8_t get_x(uint16_t position) {
            return position % max_x;
        }

        uint8_t get_y(uint16_t position) {
            return position / max_x;
        }

        void set_shape_begin(uint8_t begin) {
            uint8_t reg = VGA::Registers::read(0x3D4, 0x0A);
            reg = (reg & 0xE0) | (begin & 0x1F);

            VGA::Registers::write(0x3D4, 0x0A, reg);
        }

        void set_shape_end(uint8_t end) {
            uint8_t reg = VGA::Registers::read(0x3D4, 0x0B);
            reg = (reg & 0xE0) | (end & 0x1F);

            VGA::Registers::write(0x3D4, 0x0B, reg);
        }

        void set_shape(uint8_t begin, uint8_t end) {
            set_shape_begin(begin);
            set_shape_end(end);
        }

        uint8_t get_shape_begin() {
            return VGA::Registers::read(0x3D4, 0x0A) & 0x1F;
        }

        uint8_t get_shape_end() {
            return VGA::Registers::read(0x3D4, 0x0B) & 0x1F;
        }

        void set_visible(bool visible) {
            uint8_t reg = VGA::Registers::read(0x3D4, 0x0A);
            reg = (reg & 0xDF) | (!visible << 5);

            VGA::Registers::write(0x3D4, 0x0A, reg);
        }

        bool is_visible() {
            return !(VGA::Registers::read(0x3D4, 0x0A) & 0x20);
        }
    }

    namespace Text {
        void set(char c, uint16_t position) {
            buffer[position].codepoint = c;
        }

        void setc(char c, uint8_t x, uint8_t y) {
            set(c, Cursor::from_coordinates(x, y));
        }

        uint8_t get(uint16_t position) {
            return buffer[position].codepoint;
        }

        uint8_t getc(uint8_t x, uint8_t y) {
            return get(Cursor::from_coordinates(x, y));
        }
    }
}