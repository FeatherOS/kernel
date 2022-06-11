#ifndef _MEM__BASIC_HPP_
#define _MEM__BASIC_HPP_

#include <stdint.h>
#include <stddef.h>

void memcpy(void *dest, const void *src, size_t n);
void memset(void *dest, uint8_t value, size_t n);

#endif