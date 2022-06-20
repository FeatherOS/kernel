#include <basic.hpp>

void memcpy(void *dest, const void *src, size_t n) {
    // We need to copy the memory from src to dest and do it as fast as possible.
    // To do this, we use the assembly language in x86 and compatible (x86-64) architectures.
    // In this case, we use the "rep movsb" instruction.

    // If we aren't running x86, we may use a fallback method.
    // In this case the fallback is a simple for loop.
    // This is not the fastest method, but it is cross-platform.

    #if defined(__i386__) || defined(__x86_64__)
        asm volatile("rep movsb" : "+D"(dest), "+S"(src), "+c"(n) : : "memory");
    #else
        #warning "memcpy is not implemented for this architecture. Using a fallback method."
        for (size_t i = 0; i < n; i++) {
            ((uint8_t *)dest)[i] = ((uint8_t *)src)[i];
        }
    #endif
}


void memset(void *dest, uint8_t value, size_t n) {
    // We need to set the memory to a specific value.
    // To do this, we use the assembly language in x86 and compatible (x86-64) architectures.
    // In this case, we use the "rep stosb" instruction.

    // If we aren't running x86, we may use a fallback method.
    // In this case the fallback is a simple for loop.
    // This is not the fastest method, but it is cross-platform.

    #if defined(__i386__) || defined(__x86_64__)
        asm volatile("rep stosb" : "+D"(dest), "+c"(n) : "a"(value) : "memory");
    #else
        #warning "memset is not implemented for this architecture. Using a fallback method."
        for (size_t i = 0; i < n; i++) {
            ((uint8_t *)dest)[i] = value;
        }
    #endif
}