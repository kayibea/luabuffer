#include "hexlib.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* hex_encode(const uint8_t* data, size_t len) {
  char* hex = malloc(2 * len + 1);
  if (!hex) return NULL;

  for (size_t i = 0; i < len; i++) {
    snprintf(&hex[2 * i], 3, "%02x", data[i]);
  }

  return hex;
}

uint8_t* hex_decode(const char* data, size_t* len) {
  size_t input_len = strlen(data);
  if (input_len % 2 != 0) return NULL;

  *len = input_len / 2;
  uint8_t* decoded = malloc(*len);
  if (!decoded) return NULL;

  for (size_t i = 0; i < *len; i++) {
    sscanf(&data[2 * i], "%2hhx", &decoded[i]);
  }

  return decoded;
}