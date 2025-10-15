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

int l_buffer_write_f32le(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  float value = (float)luaL_checknumber(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 0);

  if (offset < 0 || (size_t)offset + 4 > buf->size)
    return luaL_error(L, ERR_OFFSET_OUT_OF_RANGE);

  memcpy(buf->buffer + (size_t)offset, &value, 4);
  lua_pushinteger(L, offset + 4);
  return 1;
}

int l_buffer_write_f32be(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  float value = (float)luaL_checknumber(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 0);

  if (offset < 0 || (size_t)offset + 4 > buf->size)
    return luaL_error(L, ERR_OFFSET_OUT_OF_RANGE);

  uint8_t tmp[4];
  memcpy(tmp, &value, 4);
  reverse_bytes(tmp, 4);
  memcpy(buf->buffer + (size_t)offset, tmp, 4);

  lua_pushinteger(L, offset + 4);
  return 1;
}

int l_buffer_read_f32le(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 0);

  if (offset < 0 || (size_t)offset + 4 > buf->size)
    return luaL_error(L, ERR_OFFSET_OUT_OF_RANGE);

  float value;
  memcpy(&value, buf->buffer + (size_t)offset, 4);
  lua_pushnumber(L, (lua_Number)value);
  return 1;
}

int l_buffer_read_f32be(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 0);

  if (offset < 0 || (size_t)offset + 4 > buf->size)
    return luaL_error(L, ERR_OFFSET_OUT_OF_RANGE);

  uint8_t tmp[4];
  memcpy(tmp, buf->buffer + (size_t)offset, 4);
  reverse_bytes(tmp, 4);
  float value;
  memcpy(&value, tmp, 4);

  lua_pushnumber(L, (lua_Number)value);
  return 1;
}

int l_buffer_write_f64le(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  double value = (double)luaL_checknumber(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 0);

  if (offset < 0 || (size_t)offset + 4 > buf->size)
    return luaL_error(L, ERR_OFFSET_OUT_OF_RANGE);

  memcpy(buf->buffer + (size_t)offset, &value, 8);
  lua_pushinteger(L, offset + 8);
  return 1;
}

int l_buffer_write_f64be(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  double value = (double)luaL_checknumber(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 0);

  if (offset < 0 || (size_t)offset + 4 > buf->size)
    return luaL_error(L, ERR_OFFSET_OUT_OF_RANGE);

  uint8_t tmp[8];
  memcpy(tmp, &value, 8);
  reverse_bytes(tmp, 8);
  memcpy(buf->buffer + (size_t)offset, tmp, 8);

  lua_pushinteger(L, offset + 8);
  return 1;
}

int l_buffer_read_f64le(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 0);

  if (offset < 0 || (size_t)offset + 4 > buf->size)
    return luaL_error(L, ERR_OFFSET_OUT_OF_RANGE);

  double value;
  memcpy(&value, buf->buffer + (size_t)offset, 8);
  lua_pushnumber(L, (lua_Number)value);
  return 1;
}

int l_buffer_read_f64be(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 0);

  if (offset < 0 || (size_t)offset + 4 > buf->size)
    return luaL_error(L, ERR_OFFSET_OUT_OF_RANGE);

  uint8_t tmp[8];
  memcpy(tmp, buf->buffer + (size_t)offset, 8);
  reverse_bytes(tmp, 8);
  double value;
  memcpy(&value, tmp, 8);

  lua_pushnumber(L, (lua_Number)value);
  return 1;
}

int l_buffer_read_u32be(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 0);

  if (offset < 0 || (size_t)offset + 4 > buf->size)
    return luaL_error(L, ERR_OFFSET_OUT_OF_RANGE);

  const uint8_t* p = buf->buffer + offset;
  uint32_t value = ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
                   ((uint32_t)p[2] << 8) | ((uint32_t)p[3]);

  lua_pushinteger(L, (lua_Integer)value);
  return 1;
}

int l_buffer_write_u32be(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer value = luaL_checkinteger(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 0);

  if (offset < 0 || (size_t)offset + 4 > buf->size)
    return luaL_error(L, ERR_OFFSET_OUT_OF_RANGE);

  uint8_t* p = buf->buffer + (size_t)offset;
  uint32_t v = (uint32_t)value;

  p[0] = (v >> 24) & 0xFF;
  p[1] = (v >> 16) & 0xFF;
  p[2] = (v >> 8) & 0xFF;
  p[3] = v & 0xFF;

  lua_pushinteger(L, offset + 4);
  return 1;
}

int l_buffer_read_u32le(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer offset = luaL_optinteger(L, 2, 0);

  if (offset < 0 || (size_t)offset + 4 > buf->size)
    return luaL_error(L, ERR_OFFSET_OUT_OF_RANGE);

  const uint8_t* p = buf->buffer + (size_t)offset;
  uint32_t value = ((uint32_t)p[0]) | ((uint32_t)p[1] << 8) |
                   ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);

  lua_pushinteger(L, (lua_Integer)value);
  return 1;
}

int l_buffer_write_u32le(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_Integer value = luaL_checkinteger(L, 2);
  lua_Integer offset = luaL_optinteger(L, 3, 0);

  if (offset < 0 || (size_t)offset + 4 > buf->size)
    return luaL_error(L, ERR_OFFSET_OUT_OF_RANGE);

  uint8_t* p = buf->buffer + (size_t)offset;
  uint32_t v = (uint32_t)value;

  p[0] = v & 0xFF;
  p[1] = (v >> 8) & 0xFF;
  p[2] = (v >> 16) & 0xFF;
  p[3] = (v >> 24) & 0xFF;

  lua_pushinteger(L, offset + 4);
  return 1;
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
    if (!hexstr) return throw_lua_oom(L, slice_len);
    lua_pushstring(L, hexstr);
    FREE(hexstr);

  } else {
    return luaL_error(L, "Unsupported encoding: %s", encoding);
  }

  return 1;
}
