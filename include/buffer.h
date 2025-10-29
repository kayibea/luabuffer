#pragma once

#include <stddef.h>
#include <stdint.h>

#define BUFFER_MT "Buffer*"
#define BUFFER_METHODSINDEX "__methods"
#define BUFFER_INSPECT_MAX_BYTES 50

#define SIZE_F32 ((size_t)sizeof(float))
#define SIZE_F64 ((size_t)sizeof(double))
#define SIZE_UINT8 ((size_t)sizeof(uint8_t))
#define SIZE_INT8 ((size_t)sizeof(int8_t))
#define SIZE_UINT16 ((size_t)sizeof(uint16_t))
#define SIZE_INT16 ((size_t)sizeof(int16_t))
#define SIZE_UINT32 ((size_t)sizeof(uint32_t))
#define SIZE_INT32 ((size_t)sizeof(int32_t))
#define SIZE_UINT64 ((size_t)sizeof(uint64_t))
#define SIZE_INT64 ((size_t)sizeof(int64_t))

#define ENCODING_UTF8 "utf8"
#define ENCODING_BASE16 "hex"
#define ENCODING_BASE64 "base64"

#define SUPPORTED_ENCODINGS \
  ENCODING_UTF8 ", " ENCODING_BASE16 ", " ENCODING_BASE64

typedef struct {
  uint8_t* buffer;
  size_t size;
} Buffer;
