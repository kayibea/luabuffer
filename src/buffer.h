#pragma once

#include <stddef.h>
#include <stdint.h>

#define BUFFER_MT "Buffer*"
#define BUFFER_METHODSINDEX "__methods"
#define BUFFER_INSPECT_MAX_BYTES 50

#define ENCODING_UTF8 "utf8"
#define ENCODING_HEXA "hexa"

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

typedef struct {
  size_t size;
  uint8_t *buffer;
} buf_t;
