#ifndef _VGA__TEXTMODE_HPP_
#define _VGA__TEXTMODE_HPP_

#include <stdint.h>
#define VGA_TXT_SZ VGA::TextMode::Cursor::max_x * VGA::TextMode::Cursor::max_y

namespace VGA::TextMode {
    struct Cell {
        union {
            struct {
                uint8_t codepoint : 8;

                union {
                    struct {
                        uint8_t foreground : 4;
                        uint8_t background : 4;
                    };
                    uint8_t color : 8;
                };
            };
            uint16_t value;
        };
    } __attribute__((packed));

    namespace Color {
        enum color_t {
            Black        = 0x0,
            Blue         = 0x1,
            Green        = 0x2,
            Cyan         = 0x3,
            Red          = 0x4,
            Magenta      = 0x5,
            Brown        = 0x6,
            LightGray    = 0x7,
            DarkGray     = 0x8,
            LightBlue    = 0x9,
            LightGreen   = 0xA,
            LightCyan    = 0xB,
            LightRed     = 0xC,
            LightMagenta = 0xD,
            Yellow       = 0xE,
            White        = 0xF
        };

        color_t operator~(color_t color);

        void set(color_t foreground, color_t background, uint16_t position);
        void setc(color_t foreground, color_t background, uint8_t x, uint8_t y);
        void seta(uint8_t attribute, uint16_t position);
        void setac(uint8_t attribute, uint8_t x, uint8_t y);

        color_t get_fg(uint16_t position);
        color_t getc_fg(uint8_t x, uint8_t y);
        color_t get_bg(uint16_t position);
        color_t getc_bg(uint8_t x, uint8_t y);
        uint8_t geta(uint16_t position);
        uint8_t getac(uint8_t x, uint8_t y);
    }

    namespace Cursor {
        extern uint8_t max_x;
        extern uint8_t max_y;

        uint8_t get_max_x();
        uint8_t get_max_y();

        uint16_t from_coordinates(uint8_t x, uint8_t y);
        bool is_valid(uint16_t position);
        
        void set(uint16_t position);
        void setc(uint8_t x, uint8_t y);

        uint16_t get();
        uint8_t get_x(uint16_t position = get());
        uint8_t get_y(uint16_t position = get());

        void set_shape(uint8_t begin, uint8_t end);
        void set_shape_begin(uint8_t begin);
        void set_shape_end(uint8_t end);
        uint8_t get_shape_begin();
        uint8_t get_shape_end();

        void set_visible(bool visible);
        bool is_visible();
    }

    namespace Text {
        void set(uint8_t codepoint, uint16_t position);
        void setc(uint8_t codepoint, uint8_t x, uint8_t y);

        uint8_t get(uint16_t position);
        uint8_t getc(uint8_t x, uint8_t y);
    }

    namespace {
        __attribute__((unused)) /* Without this `-Wunused` will cause a useless warning */ Cell* buffer = (Cell *)0xB8000;
    }
}

#endif