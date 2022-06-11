#ifndef _VGA__TEXTMODE_GOODIES_HPP_
#define _VGA__TEXTMODE_GOODIES_HPP_

#include <stdint.h>
#include <stddef.h>
#include "textmode.hpp"

namespace VGA::TextMode {
    Cell *buffer_copy(Cell *buffer_location);
    void buffer_paste(Cell *buffer_copy);

    namespace Cursor {
        void move_up(uint8_t n = 1);
        void move_down(uint8_t n = 1);
        void move_left(uint8_t n = 1);
        void move_right(uint8_t n = 1);
    }

    namespace Text {
        void set_blinking(bool enabled);
        bool is_blinking_enabled();
    }

    namespace Color {
        color_t to_intensive(color_t color);
        color_t to_extensive(color_t color);
        color_t to_inverted(color_t color);
    }

    void scroll(Color::color_t fg = Color::LightGray, Color::color_t bg = Color::Black);
    void clear_line(uint8_t line, Cell *buffer_location = buffer, Color::color_t fg = Color::LightGray, Color::color_t bg = Color::Black);
    void clear(Color::color_t default_fg = Color::LightGray, Color::color_t default_bg = Color::Black, bool reset_cursor = true);
}

#endif