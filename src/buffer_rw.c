#include "buffer_rw.h"

#include <lauxlib.h>
#include <lua.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/param.h>

#include "buffer.h"
#include "common.h"
#include "errors.h"
#include "hexlib.h"
#include "utils.h"

static inline void buffer_check(lua_State* L, const Buffer* buf,
                                lua_Integer offset, size_t len) {
  if (offset < 0)
    luaL_error(L, "offset is out of range (expected >= 1, got %I)", offset + 1);

  if ((size_t)offset + len > buf->size)
    luaL_error(L,
               "attempt to access memory outside buffer bounds (offset=%I, "
               "size=%I, len=%I)",
               offset + 1, (lua_Integer)buf->size, (lua_Integer)len);
}

static int buffer_read_int(lua_State* L, size_t byteLength, bool littleEndian) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 1) - 1;

  buffer_check(L, buf, offset, byteLength);

  const uint8_t* p = buf->buffer + (size_t)offset;
  int64_t value = 0;

  if (littleEndian) {
    for (size_t i = 0; i < byteLength; i++) value |= (int64_t)p[i] << (8 * i);
  } else {
    for (size_t i = 0; i < byteLength; i++)
      value |= (int64_t)p[i] << (8 * (byteLength - 1 - i));
  }

  int shift = 64 - (byteLength * 8);
  value = (value << shift) >> shift;

  lua_pushinteger(L, (lua_Integer)value);
  return 1;
}

static int buffer_read_uint(lua_State* L, size_t byteLength,
                            bool littleEndian) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 1) - 1;

  buffer_check(L, buf, offset, byteLength);

  const uint8_t* p = buf->buffer + (size_t)offset;
  uint64_t value = 0;

  if (littleEndian) {
    for (size_t i = 0; i < byteLength; i++) value |= (uint64_t)p[i] << (8 * i);
  } else {
    for (size_t i = 0; i < byteLength; i++)
      value |= (uint64_t)p[i] << (8 * (byteLength - 1 - i));
  }

  lua_pushinteger(L, (lua_Integer)value);
  return 1;
}

static int buffer_write_int(lua_State* L, size_t byteLength,
                            bool littleEndian) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Number value = luaL_checknumber(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 1) - 1;

  buffer_check(L, buf, offset, byteLength);

  int64_t v = (int64_t)value;
  uint8_t* p = buf->buffer + (size_t)offset;

  if (littleEndian) {
    for (size_t i = 0; i < byteLength; i++) p[i] = (v >> (8 * i)) & 0xFF;
  } else {
    for (size_t i = 0; i < byteLength; i++)
      p[byteLength - 1 - i] = (v >> (8 * i)) & 0xFF;
  }

  lua_pushinteger(L, offset + byteLength + 1);
  return 1;
}

static int buffer_write_uint(lua_State* L, size_t byteLength,
                             bool littleEndian) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Number value = luaL_checknumber(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 1) - 1;

  buffer_check(L, buf, offset, byteLength);

  uint64_t v = (uint64_t)value;
  uint8_t* p = buf->buffer + (size_t)offset;

  if (littleEndian) {
    for (size_t i = 0; i < byteLength; i++) p[i] = (v >> (8 * i)) & 0xFF;
  } else {
    for (size_t i = 0; i < byteLength; i++)
      p[byteLength - 1 - i] = (v >> (8 * i)) & 0xFF;
  }

  lua_pushinteger(L, offset + byteLength + 1);
  return 1;
}

int l_buffer_write_f32le(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  float value = (float)luaL_checknumber(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 1) - 1;

  buffer_check(L, buf, offset, SIZE_F32);

  memcpy(buf->buffer + (size_t)offset, &value, SIZE_F32);
  lua_pushinteger(L, offset + SIZE_F32 + 1);
  return 1;
}

int l_buffer_write_f32be(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  float value = (float)luaL_checknumber(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 1) - 1;

  buffer_check(L, buf, offset, SIZE_F32);

  uint8_t tmp[SIZE_F32];
  memcpy(tmp, &value, SIZE_F32);
  reverse_bytes(tmp, SIZE_F32);
  memcpy(buf->buffer + (size_t)offset, tmp, SIZE_F32);

  lua_pushinteger(L, offset + SIZE_F32 + 1);
  return 1;
}

int l_buffer_read_f32le(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 1) - 1;

  buffer_check(L, buf, offset, SIZE_F32);

  float value;
  memcpy(&value, buf->buffer + (size_t)offset, SIZE_F32);
  lua_pushnumber(L, (lua_Number)value);
  return 1;
}

int l_buffer_read_f32be(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 1) - 1;

  buffer_check(L, buf, offset, SIZE_F32);

  uint8_t tmp[SIZE_F32];
  memcpy(tmp, buf->buffer + (size_t)offset, SIZE_F32);
  reverse_bytes(tmp, SIZE_F32);
  float value;
  memcpy(&value, tmp, SIZE_F32);

  lua_pushnumber(L, (lua_Number)value);
  return 1;
}

int l_buffer_write_f64le(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  double value = (double)luaL_checknumber(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 1) - 1;

  buffer_check(L, buf, offset, SIZE_F64);

  memcpy(buf->buffer + (size_t)offset, &value, SIZE_F64);
  lua_pushinteger(L, offset + SIZE_F64 + 1);
  return 1;
}

int l_buffer_write_f64be(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  double value = (double)luaL_checknumber(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 1) - 1;

  buffer_check(L, buf, offset, SIZE_F64);

  uint8_t tmp[SIZE_F64];
  memcpy(tmp, &value, SIZE_F64);
  reverse_bytes(tmp, SIZE_F64);
  memcpy(buf->buffer + (size_t)offset, tmp, SIZE_F64);

  lua_pushinteger(L, offset + SIZE_F64 + 1);
  return 1;
}

int l_buffer_read_f64le(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 1) - 1;

  buffer_check(L, buf, offset, SIZE_F64);

  double value;
  memcpy(&value, buf->buffer + (size_t)offset, SIZE_F64);
  lua_pushnumber(L, (lua_Number)value);
  return 1;
}

int l_buffer_read_f64be(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 1) - 1;

  buffer_check(L, buf, offset, SIZE_F64);

  uint8_t tmp[SIZE_F64];
  memcpy(tmp, buf->buffer + (size_t)offset, SIZE_F64);
  reverse_bytes(tmp, SIZE_F64);
  double value;
  memcpy(&value, tmp, SIZE_F64);

  lua_pushnumber(L, (lua_Number)value);
  return 1;
}

int l_buffer_read_u32be(lua_State* L) {
  return buffer_read_uint(L, SIZE_UINT32, false);
}

int l_buffer_write_u32be(lua_State* L) {
  return buffer_write_uint(L, SIZE_UINT32, false);
}

int l_buffer_read_u32le(lua_State* L) {
  return buffer_read_uint(L, SIZE_UINT32, true);
}

int l_buffer_write_u32le(lua_State* L) {
  return buffer_write_uint(L, SIZE_UINT32, true);
}

int l_buffer_read_u16le(lua_State* L) {
  return buffer_read_uint(L, SIZE_UINT16, true);
}

int l_buffer_write_u16le(lua_State* L) {
  return buffer_write_uint(L, SIZE_UINT16, true);
}

int l_buffer_write_i16be(lua_State* L) {
  return buffer_write_int(L, SIZE_INT16, false);
}

int l_buffer_read_i16be(lua_State* L) {
  return buffer_read_int(L, SIZE_INT16, false);
}

int l_buffer_read_i16le(lua_State* L) {
  return buffer_read_int(L, SIZE_INT16, false);
}

int l_buffer_write_i16le(lua_State* L) {
  return buffer_write_int(L, SIZE_INT16, false);
}

int l_buffer_tostring(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  const char* encoding = luaL_optstring(L, 2, ENCODING_UTF8);
  lua_Integer start = luaL_optinteger(L, 3, 0);
  lua_Integer end = luaL_optinteger(L, 4, buf->size);

  if (start < 0) start = (lua_Integer)buf->size + start + 1;
  if (end < 0) end = (lua_Integer)buf->size + end + 1;

  if (start < 1) start = 1;
  if (end > (lua_Integer)buf->size) end = buf->size;

  if (end < start) {
    lua_pushliteral(L, "");
    return 1;
  }

  size_t slice_len = (size_t)(end - start + 1);
  const uint8_t* slice_buf = buf->buffer + (start - 1);

  if (strcasecmp(encoding, ENCODING_UTF8) == 0) {
    lua_pushlstring(L, (const char*)slice_buf, slice_len);
  } else if (strcasecmp(encoding, ENCODING_BASE16) == 0) {
    char* hexstr = hex_encode(slice_buf, slice_len);
    if (!hexstr) return throw_luaoom(L, slice_len);
    lua_pushstring(L, hexstr);
    FREE(hexstr);
  } else {
    return luaL_error(L, "Unsupported encoding: %s", encoding);
  }

  return 1;
}

// FIXME:  This pile of shit need more reviews (and maybe rewriten)
int l_buffer_write_string(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  size_t str_len;
  const char* str = luaL_checklstring(L, 2, &str_len);

  lua_Integer offset = 1;   // Lua 1-based default
  lua_Integer length = -1;  // -1 == unspecified
  const char* encoding = ENCODING_UTF8;

  if (!lua_isnoneornil(L, 3)) offset = luaL_checkinteger(L, 3);
  if (!lua_isnoneornil(L, 4)) length = luaL_checkinteger(L, 4);
  if (!lua_isnoneornil(L, 5)) encoding = luaL_checkstring(L, 5);

  // validate offset: 1 .. buf->size (inclusive)
  if (offset < 1 || offset > (lua_Integer)buf->size)
    return luaL_error(L, ERR_OFFSET_OUT_OF_RANGE);

  // compute remaining space starting from this 1-based offset:
  // remaining = buf->size - (offset - 1)
  size_t remaining = buf->size - (size_t)(offset - 1);

  // default length: min(string length, buffer length) per your request
  if (lua_isnoneornil(L, 4)) {
    length = (lua_Integer)MIN(str_len, buf->size);
    // length =
    //     (lua_Integer)((str_len < (size_t)buf->size) ? (lua_Integer)str_len
    //                                                 :
    //                                                 (lua_Integer)buf->size);
  }

  // validate length: 0..buf->size (0 allowed -> write nothing)
  if (length < 0 || length > (lua_Integer)buf->size)
    return luaL_error(L, "Error here");

  // if length == 0 or nothing remains to write, return 0
  if (length == 0 || remaining == 0) {
    lua_pushinteger(L, 0);
    return 1;
  }

  // 0-based write offset in C:
  size_t write_offset = (size_t)(offset - 1);

  // compute final write_len = min(length, str_len, remaining)
  size_t write_len = (size_t)length;
  if (write_len > str_len) write_len = str_len;
  if (write_len > remaining) write_len = remaining;

  if (strcasecmp(encoding, ENCODING_UTF8) == 0) {
    memcpy(buf->buffer + write_offset, str, write_len);
  } else if (strcasecmp(encoding, ENCODING_BASE16) == 0) {
    size_t decoded_len = 0;
    uint8_t* decoded = hex_decode(str, &decoded_len);
    if (!decoded) return luaL_error(L, ERR_INVALID_HEX_STRING);

    // decoded_len might be larger than we can write, cap it
    if (decoded_len > write_len) decoded_len = write_len;

    memcpy(buf->buffer + write_offset, decoded, decoded_len);
    write_len = decoded_len;
    FREE(decoded);
  } else {
    return luaL_error(L, ERR_UNSUPPORTED_ENCODING, encoding);
  }

  lua_pushinteger(L, (lua_Integer)write_len);
  return 1;
}
