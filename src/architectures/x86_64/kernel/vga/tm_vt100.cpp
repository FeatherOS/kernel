#include <console/console.hpp>
#include <console/vt100.hpp>
#include <console/fonts.hpp>

#include <vga/textmode.hpp>
#include <vga/tm_goodies.hpp>
#include <vga/tm_fonts.hpp>
#include <vga/tm_vt100.hpp>

#define VGA_TXT_DEFAULT_FG VGA::TextMode::Color::LightGray
#define VGA_TXT_DEFAULT_BG VGA::TextMode::Color::Black

namespace VGA::TextMode {
    namespace {
        uint8_t cursor_x = Cursor::get_x();
        uint8_t cursor_y = Cursor::get_y();

        Color::color_t vt100_colors[8] = {
            Color::Black,
            Color::Red,
            Color::Green,
            Color::Yellow,
            Color::Blue,
            Color::Magenta,
            Color::Cyan,
            Color::LightGray
        };

        struct {
            bool bold   = false;
            bool invert = false;
            bool hidden = false;

            Color::color_t hidden_fg = VGA_TXT_DEFAULT_FG;

            Color::color_t background = VGA_TXT_DEFAULT_BG;
            Color::color_t foreground = VGA_TXT_DEFAULT_FG;

            Color::color_t *get_bg_ptr() {
                if (invert) {
                    return &(hidden ? hidden_fg : foreground);
                } else {
                    return &background;
                }
            }

            Color::color_t *get_fg_ptr() {
                if (invert) {
                    return &background;
                } else {
                    return &foreground;
                }
            }
        } vt100_state = {};

        void putc(char c) {
            auto regularize_cursor_positions = []() {
                while (cursor_x >= Cursor::get_max_x()) {
                    cursor_x -= Cursor::get_max_x();
                    cursor_y++;
                }

                while (cursor_y >= Cursor::get_max_y()) {
                    VGA::TextMode::scroll();
                    cursor_y--;
                }
            };

            regularize_cursor_positions();

            switch (c) {
                case '\n': {
                    cursor_x = 0;
                    cursor_y++;
                    break;
                }

                case '\r': {
                    cursor_x = 0;
                    break;
                }

                case '\t': {
                    cursor_x += 8 - (cursor_x % 8);
                    break;
                }

                default: {
                    VGA::TextMode::buffer[Cursor::from_coordinates(cursor_x, cursor_y)].codepoint  = c;
                    VGA::TextMode::buffer[Cursor::from_coordinates(cursor_x, cursor_y)].foreground = *(vt100_state.get_fg_ptr());
                    VGA::TextMode::buffer[Cursor::from_coordinates(cursor_x, cursor_y)].background = *(vt100_state.get_bg_ptr());
                    cursor_x++;
                }
            }

            regularize_cursor_positions();

            Cursor::setc(cursor_x, cursor_y);
        }
    }

    VT100 vt100;
    Console::KernelTerminal virtual_terminal;

    void initialize_vt() {
//         uint8_t new_font[] = TEXT_MODE_FONT_8x16;
//         VGA::TextMode::Text::set_font(new_font);
        VGA::TextMode::Text::set_pixel9_visible(false);

        VGA::TextMode::Text::set_blinking(false);
        
        VGA::TextMode::clear(VGA_TXT_DEFAULT_FG, VGA_TXT_DEFAULT_BG);
        virtual_terminal.set_putchar_callback(VT100_GET_PUTCHAR_PTR(vt100));
        vt100.set_putchar_callback(putc);
        VGA::TextMode::Cursor::set_shape(0, 13);

        vt100.add_csi_command('H', [](size_t *arguments, size_t /* argcount */, void (* /* output */)(char c)) {
            size_t x, y;
            x = arguments[1];
            y = arguments[0];

            if (x > Cursor::get_max_x()) {
                x = Cursor::get_max_x();
            }

            if (y > Cursor::get_max_y()) {
                y = Cursor::get_max_y();
            }

            cursor_x = x;
            cursor_y = y;
            Cursor::setc(x, y);
        });

        vt100.add_csi_command('m', [](size_t *arguments, size_t argcount, void (* /* output */)(char c)) {
            for (size_t ac = 0; ac < argcount; ac++) {
                size_t i = arguments[ac];

                if ((i >= 90) && (i <= 97)) {
                    vt100_state.foreground = Color::to_intensive(vt100_colors[i - 90]);
                } else if ((i >= 100) && (i <= 107)) {
                    vt100_state.background = Color::to_intensive(vt100_colors[i - 100]);
                } else if ((i >= 30) && (i <= 37)) {
                    vt100_state.foreground = vt100_colors[i - 30];
                } else if ((i >= 40) && (i <= 47)) {
                    vt100_state.background = vt100_colors[i - 40];
                } else if (i == 1) {
                    vt100_state.foreground = Color::to_intensive(vt100_state.foreground);
                    vt100_state.bold = true;
                } else if (i == 21) {
                    vt100_state.foreground = Color::to_extensive(vt100_state.foreground);
                    vt100_state.bold = false;
                } else if (i == 7) {
                    if (!vt100_state.invert) {
                        vt100_state.invert = true;
                    }
                } else if (i == 27) {
                    if (vt100_state.invert) {
                        vt100_state.invert = false;
                    }
                } else if (i == 8) {
                    vt100_state.hidden_fg = vt100_state.foreground;
                    vt100_state.foreground = vt100_state.background;
                    vt100_state.hidden = true;
                } else if (i == 28) {
                    vt100_state.foreground = vt100_state.hidden_fg;
                    vt100_state.hidden = false;
                } else if (i == 0) {
                    vt100_state.foreground = VGA_TXT_DEFAULT_FG;
                    vt100_state.background = VGA_TXT_DEFAULT_BG;
                    vt100_state.bold = false;
                    vt100_state.invert = false;
                    vt100_state.hidden = false;
                    vt100_state.hidden_fg = VGA_TXT_DEFAULT_FG;
                }
            }
        });

        vt100.add_csi_command('J', [](size_t *arguments, size_t /* argcount */, void (* /* output */)(char c)) {
            size_t i = arguments[0];

            if (i == 0) {
                VGA::TextMode::clear(VGA_TXT_DEFAULT_FG, VGA_TXT_DEFAULT_BG, false);
            } else if (i == 1) {
                for (uint16_t start_pos = Cursor::get(); start_pos < VGA_TXT_SZ; start_pos++) {
                    VGA::TextMode::buffer[start_pos].codepoint = ' ';
                    VGA::TextMode::buffer[start_pos].foreground = VGA_TXT_DEFAULT_FG;
                    VGA::TextMode::buffer[start_pos].background = VGA_TXT_DEFAULT_BG;
                }
            } else if (i == 2) {
                for (uint16_t start_pos = 0; start_pos < Cursor::get(); start_pos++) {
                    VGA::TextMode::buffer[start_pos].codepoint = ' ';
                    VGA::TextMode::buffer[start_pos].foreground = VGA_TXT_DEFAULT_FG;
                    VGA::TextMode::buffer[start_pos].background = VGA_TXT_DEFAULT_BG;
                }
            }
        });
    }
}
