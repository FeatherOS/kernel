#include <vga/textmode.hpp>
#include <vga/registers.hpp>
#include <vga/tm_fonts.hpp>

namespace VGA::TextMode {
    namespace Text {
        void set_font(uint8_t *font) {
            uint8_t max_scanline = get_max_scanline();

            VGA::Registers::write(0x3C4, 0x02, 0x04);
            VGA::Registers::write(0x3C4, 0x03, 0x00);

            uint8_t mem_mode = VGA::Registers::read(0x3C4, 0x04);
            VGA::Registers::write(0x3C4, 0x04, 0x06);
            
            VGA::Registers::write(0x3CE, 0x04, 0x02);
            uint8_t graphics_mode = VGA::Registers::read(0x3CE, 0x05);
            VGA::Registers::write(0x3CE, 0x05, 0x00);

            VGA::Registers::write(0x3CE, 0x06, 0x0C);

            uint8_t *font_buffer = (uint8_t *)0xB8000;
            for (int i = 0; i < 256; i++) {
                for (int j = 0; j < max_scanline; j++) {
                    *font_buffer = *font;
                    ++font_buffer;
                    ++font;
                }
                font_buffer += 32 - max_scanline;
            }

            VGA::Registers::write(0x3C4, 0x02, 0x03);
            VGA::Registers::write(0x3C4, 0x04, mem_mode);
            
            VGA::Registers::write(0x3CE, 0x04, 0x00);
            VGA::Registers::write(0x3CE, 0x05, graphics_mode);
            VGA::Registers::write(0x3CE, 0x06, 0x0C);
        }

        void set_max_scanline(uint8_t max_scanline) {
            struct {
                union {
                    struct {
                        uint8_t max_scan_line           : 5;
                        uint8_t start_vertical_blank_b5 : 1;
                        uint8_t line_compare_b5         : 1;
                        uint8_t scan_doubling           : 1;
                    };
                    uint8_t value: 8;
                };
            } max_scanline_register;
            max_scanline_register.value = VGA::Registers::read(0x3D4, 0x09);

            Cursor::max_y = (max_scanline_register.max_scan_line + 1) * Cursor::max_y / max_scanline;

            max_scanline_register.max_scan_line = (max_scanline - 1);

            VGA::Registers::write(0x3D4, 0x09, max_scanline_register.value);
        }

        uint8_t get_max_scanline() {
            struct {
                union {
                    struct {
                        uint8_t max_scan_line           : 5;
                        uint8_t start_vertical_blank_b5 : 1;
                        uint8_t line_compare_b5         : 1;
                        uint8_t scan_doubling           : 1;
                    };
                    uint8_t value: 8;
                };
            } max_scanline_register;
            max_scanline_register.value = VGA::Registers::read(0x3D4, 0x09);

            return max_scanline_register.max_scan_line + 1;
        }

        void set_pixel9_visible(bool visible) {
            struct {
                union {
                    struct {
                        bool    pix98 : 1;
                        bool    unused: 1;
                        bool    slr   : 1;
                        bool    dcr   : 1;
                        bool    s4    : 1;
                        bool    sd    : 1;
                        uint8_t unused2:2;
                    };
                    uint8_t value  : 8;
                };
            } __attribute__((packed)) clocking_mode_register;

            clocking_mode_register.value = VGA::Registers::read(0x3C4, 0x01);
            clocking_mode_register.pix98 = !visible;

            VGA::Registers::write(0x3C4, 0x01, clocking_mode_register.value);
        }

        bool is_pixel9_enabled() {
            struct {
                union {
                    struct {
                        bool    pix98 : 1;
                        bool    unused: 1;
                        bool    slr   : 1;
                        bool    dcr   : 1;
                        bool    s4    : 1;
                        bool    sd    : 1;
                        uint8_t unused2:2;
                    };
                    uint8_t value  : 8;
                };
            } __attribute__((packed)) clocking_mode_register;
            clocking_mode_register.value = VGA::Registers::read(0x3C4, 0x01);

            return !clocking_mode_register.pix98;
        }
    }
}