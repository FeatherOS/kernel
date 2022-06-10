#include <IO.h>
#include <multiboot.h>

#define VGA_MEMORY (unsigned char*) 0xb8000
#define VGA_WIDTH 80

unsigned short CursorPosition;

void setCursorPositionRaw(unsigned short position) {
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position & 0xFF));
    outb(0x3D4, 0x0e);
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
    CursorPosition = position;
}
void setCursorPosition(int x, int y) {
    unsigned short position;
    position = y * VGA_WIDTH + x;
    setCursorPositionRaw(position);
    return;
}

void scrollPageUp(){
    /*for(int i = 160*2; i < 4000 - 160; i++) {
        //*(VGA_MEMORY + i - 160) = *(VGA_MEMORY + i);
        *(VGA_MEMORY + i - 160) = *(VGA_MEMORY + i);
    }*/
    for(int i = 0; i < 4000 - 160; i++) {
        *(VGA_MEMORY + i) = *(VGA_MEMORY + i + 160);
    }
    //for(int i = 160*2; i < 4000 - 160; i++) *(VGA_MEMORY + i - 160) = *(VGA_MEMORY + i);
}

void print(const char* string) {
    unsigned char* charPtr = (unsigned char*) string;
        unsigned short index = CursorPosition;
        while(*charPtr != 0) {
            switch(*charPtr) {
                case 10:
                    index += VGA_WIDTH - index % VGA_WIDTH;
                break;
                case 13:
                    index -= index % VGA_WIDTH;
                break;
                default:
                    if(index < 1840) {
                        *(VGA_MEMORY + index * 2) = *charPtr;
                        //*(VGA_MEMORY + index * 2 + 1) = backgroundColor | foregroundColor;
                    } else {
                        scrollPageUp();
                        index = 1760;
                    }
                    index++;
                break;
            }
            charPtr++;
        }
        setCursorPositionRaw(index);
}
void println(const char* string) {
    print(string);
    print("\n");
}


extern "C" void main(unsigned int ebx) {
    //TODO clear Screen
    println("Welcome to FeatherOS.\n");
    //Wenn alles geht
    println("Checking kernel validity... OK");
    println("\n---= Kernel Informations BEGIN =---");
    println("Kernel Version: CUSTOM");
    println("---= Kernel Informations END =---");
    println("\n[WARNING] You are using a custom FeatherOS kernel.");
    println("Ich verfasse diesen Text (1)");
    println("Ich verfasse diesen Text (2)");
    println("Ich verfasse diesen Text (3)");
    println("Ich verfasse diesen Text (4)");
    println("Ich verfasse diesen Text (5)");
    println("Ich verfasse diesen Text (6)");
    println("Ich verfasse diesen Text (7)");
    println("Ich verfasse diesen Text (8)");
    println("Ich verfasse diesen Text (9)");
    println("Ich verfasse diesen Text (10)");
    println("Ich verfasse diesen Text (11)");
    println("Ich verfasse diesen Text (12)");
    println("Ich verfasse diesen Text (13)");
    println("Ich verfasse diesen Text (14)");
    println("Ich verfasse diesen Text (15)");
    println("Ich verfasse diesen Text (16)");
    println("Ich verfasse diesen Text (17)");
    println("Ich verfasse diesen Text (18)");
    println("Ich verfasse diesen Text (19)");
    println("Ich verfasse diesen Text (20)");
    //println("Ich verfasse diesen Text (21)");

    /*multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;
    unsigned int address_of_module = mbinfo->mods_addr;
    unsigned int mod_count = mbinfo->mods_count;
    if(mod_count > 0) {
        println("Mod loaded");
    } else {
        println("nothing loaded");
    }
    typedef void (*call_module_t)(void);
    call_module_t start_module = (call_module_t) address_of_module;
    start_module();*/
    return;
}