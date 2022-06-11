#include <vga/textmode.hpp>

namespace VGA::TextMode {
    namespace Text {
        void set_font(uint8_t *font);
        void set_max_scanline(uint8_t max_scanline);
        uint8_t get_max_scanline();
        void set_pixel9_visible(bool visible);
        bool is_pixel9_visible();
    }
}