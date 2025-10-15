#ifndef __HEXLIB__
#define __HEXLIB__

#include <stddef.h>
#include <stdint.h>

char* hex_encode(const uint8_t* data, size_t len);
uint8_t* hex_decode(const char* data, size_t* len);

#endif