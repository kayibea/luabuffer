#pragma once

#include <stddef.h>
#include <stdint.h>

#define BUFFER_MT "Buffer*"
#define BUFFER_METHODSINDEX "__methods"
#define BUFFER_INSPECT_MAX_BYTES 50

#define ENCODING_UTF8 "utf8"
#define ENCODING_BASE16 "hex"
#define ENCODING_BASE64 "base64"

#define SUPPORTED_ENCODINGS \
  ENCODING_UTF8 ", " ENCODING_BASE16 ", " ENCODING_BASE64

typedef struct {
  uint8_t* buffer;
  size_t size;
} Buffer;