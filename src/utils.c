#include "utils.h"

#include <stddef.h>
#include <stdint.h>

void reverse_bytes(uint8_t *b, size_t n) {
  for (size_t i = 0; i < n / 2; ++i) {
    uint8_t tmp = b[i];
    b[i] = b[n - 1 - i];
    b[n - 1 - i] = tmp;
  }
}
