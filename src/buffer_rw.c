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
#include "utils.h"

static inline void buffer_check(lua_State *L, const buf_t *buf,
                                lua_Integer offset, size_t len) {
  if (offset < 0)
    luaL_error(L, "offset is out of range (expected >= 1, got %I)", offset + 1);

  if ((size_t)offset + len > buf->size)
    luaL_error(L,
               "attempt to access memory outside buffer bounds (offset=%I, "
               "size=%I, len=%I)",
               offset + 1, (lua_Integer)buf->size, (lua_Integer)len);
}

int l_buffer_write_f32le(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
  float value = (float)luaL_checknumber(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 1) - 1;

  buffer_check(L, buf, offset, SIZE_F32);

  memcpy(buf->buffer + (size_t)offset, &value, SIZE_F32);
  lua_pushinteger(L, offset + SIZE_F32 + 1);
  return 1;
}

int l_buffer_write_f32be(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
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

int l_buffer_read_f32le(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 1) - 1;

  buffer_check(L, buf, offset, SIZE_F32);

  float value;
  memcpy(&value, buf->buffer + (size_t)offset, SIZE_F32);
  lua_pushnumber(L, (lua_Number)value);
  return 1;
}

int l_buffer_read_f32be(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
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

int l_buffer_write_f64le(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
  double value = (double)luaL_checknumber(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 1) - 1;

  buffer_check(L, buf, offset, SIZE_F64);

  memcpy(buf->buffer + (size_t)offset, &value, SIZE_F64);
  lua_pushinteger(L, offset + SIZE_F64 + 1);
  return 1;
}

int l_buffer_write_f64be(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
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

int l_buffer_read_f64le(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 1) - 1;

  buffer_check(L, buf, offset, SIZE_F64);

  double value;
  memcpy(&value, buf->buffer + (size_t)offset, SIZE_F64);
  lua_pushnumber(L, (lua_Number)value);
  return 1;
}

int l_buffer_read_f64be(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
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

int l_buffer_read_u32be(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 1) - 1;

  buffer_check(L, buf, offset, SIZE_UINT32);

  const uint8_t *p = buf->buffer + offset;
  uint32_t value = ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
                   ((uint32_t)p[2] << 8) | ((uint32_t)p[3]);

  lua_pushinteger(L, (lua_Integer)value);
  return 1;
}

int l_buffer_write_u32be(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Number value = luaL_checknumber(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 1) - 1;

  buffer_check(L, buf, offset, SIZE_UINT32);

  uint8_t *p = buf->buffer + (size_t)offset;
  uint32_t v = (uint32_t)value;

  p[0] = (v >> 24) & 0xFF;
  p[1] = (v >> 16) & 0xFF;
  p[2] = (v >> 8) & 0xFF;
  p[3] = v & 0xFF;

  lua_pushinteger(L, offset + SIZE_UINT32 + 1);
  return 1;
}

int l_buffer_read_u32le(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 1) - 1;

  buffer_check(L, buf, offset, SIZE_UINT32);

  const uint8_t *p = buf->buffer + (size_t)offset;
  uint32_t value = ((uint32_t)p[0]) | ((uint32_t)p[1] << 8) |
                   ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);

  lua_pushinteger(L, (lua_Integer)value);
  return 1;
}

int l_buffer_write_u32le(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Number value = luaL_checknumber(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 1) - 1;

  buffer_check(L, buf, offset, SIZE_UINT32);

  uint8_t *p = buf->buffer + (size_t)offset;
  uint32_t v = (uint32_t)value;

  p[0] = v & 0xFF;
  p[1] = (v >> 8) & 0xFF;
  p[2] = (v >> 16) & 0xFF;
  p[3] = (v >> 24) & 0xFF;

  lua_pushinteger(L, offset + SIZE_UINT32 + 1);
  return 1;
}

int l_buffer_read_u16le(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 1) - 1;

  buffer_check(L, buf, offset, SIZE_UINT16);

  const uint8_t *p = buf->buffer + (size_t)offset;
  uint16_t value = ((uint16_t)p[0]) | ((uint16_t)p[1] << 8);

  lua_pushinteger(L, (lua_Integer)value);
  return 1;
}

int l_buffer_write_u16le(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Number value = luaL_checknumber(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 1) - 1;

  buffer_check(L, buf, offset, SIZE_UINT16);

  uint8_t *p = buf->buffer + (size_t)offset;
  uint16_t v = (uint16_t)value;

  p[0] = v & 0xFF;
  p[1] = (v >> 8) & 0xFF;

  lua_pushinteger(L, offset + SIZE_UINT16 + 1);
  return 1;
}

int l_buffer_write_i16be(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Number value = luaL_checknumber(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 1) - 1;

  buffer_check(L, buf, offset, SIZE_INT16);

  uint8_t *p = buf->buffer + (size_t)offset;
  int16_t v = (int16_t)value;

  p[0] = (v >> 8) & 0xFF;
  p[1] = v & 0xFF;

  lua_pushinteger(L, offset + SIZE_INT16 + 1);
  return 1;
}

int l_buffer_read_i16be(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 1) - 1;

  buffer_check(L, buf, offset, SIZE_INT16);

  const uint8_t *p = buf->buffer + (size_t)offset;
  int16_t value = (int16_t)(((uint16_t)p[0] << 8) | p[1]);

  lua_pushinteger(L, (lua_Integer)value);
  return 1;
}

int l_buffer_read_i16le(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 1) - 1;

  buffer_check(L, buf, offset, SIZE_INT16);

  const uint8_t *p = buf->buffer + (size_t)offset;
  int16_t value = (int16_t)(p[0] | ((uint16_t)p[1] << 8));

  lua_pushinteger(L, (lua_Integer)value);
  return 1;
}

int l_buffer_write_i16le(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Number value = luaL_checknumber(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 1) - 1;

  buffer_check(L, buf, offset, SIZE_INT16);

  uint8_t *p = buf->buffer + (size_t)offset;
  int16_t v = (int16_t)value;

  p[0] = v & 0xFF;
  p[1] = (v >> 8) & 0xFF;

  lua_pushinteger(L, offset + SIZE_INT16 + 1);
  return 1;
}

int l_buffer_tostring(lua_State *L) {
  buf_t *buf = luaL_checkudata(L, 1, BUFFER_MT);
  const char *encoding = luaL_optstring(L, 2, ENCODING_UTF8);
  lua_Integer start = luaL_optinteger(L, 3, 0);
  lua_Integer end = luaL_optinteger(L, 4, buf->size);

  if (start < 0)
    start = (lua_Integer)buf->size + start + 1;
  if (end < 0)
    end = (lua_Integer)buf->size + end + 1;

  if (start < 1)
    start = 1;
  if (end > (lua_Integer)buf->size)
    end = buf->size;

  if (end < start) {
    lua_pushliteral(L, "");
    return 1;
  }

  size_t slice_len = (size_t)(end - start + 1);
  const uint8_t *slice_buf = buf->buffer + (start - 1);

  if (strcasecmp(encoding, ENCODING_UTF8) == 0) {
    lua_pushlstring(L, (const char *)slice_buf, slice_len);
  } else {
    return luaL_error(L, "Unsupported encoding: %s", encoding);
  }

  return 1;
}
