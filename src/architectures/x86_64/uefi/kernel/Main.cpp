#include <stdint.h>

typedef unsigned long long size_t;

struct Framebuffer {
    void* BaseAddress;
    size_t BufferSize;
    unsigned int Width;
    unsigned int Height;
    unsigned int PixelsPerScanLine;
};


typedef struct {
    Framebuffer* framebuffer;
} BootInfo;

Framebuffer* targetFramebuffer;
void drawPix(uint32_t x, uint32_t y, uint32_t color) {
    *(uint32_t*)((uint64_t)targetFramebuffer->BaseAddress + (x*4) + (y * targetFramebuffer->PixelsPerScanLine * 4)) = color;

}
extern "C" void kernel_main(BootInfo* BootInfo) {
    targetFramebuffer = BootInfo->framebuffer;
    drawPix(5, 5, 0xff0000);
    while(true) {
        asm("hlt");
    }
}