#include "includes/IO.h"

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
                    *(VGA_MEMORY + index * 2) = *charPtr;
                    //*(VGA_MEMORY + index * 2 + 1) = backgroundColor | foregroundColor;
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

extern "C" void main() {
    //TODO clear Screen
    println("Welcome to FeatherOS.\n");
    //Wenn alles geht
    println("Checking kernel validity... OK");
    println("\n---= Kernel Informations BEGIN =---");
    println("Kernel Version: CUSTOM");
    println("---= Kernel Informations END =---");
    println("\n[WARNING] You are using a custom FeatherOS kernel.");
    //Wenn alles geht ENDE


    //*(char*)0xb8000 = 'B';
    //println("Detecting kernel...");
    //println("Kernel detected - Version: UNKNOWN");
    //println("[WARNING]: You are not using the official FeatherOS kernel.");
    //println("Dies ist der zweite Test String");
    //setCursorPosition(5, 5);
    return;
}