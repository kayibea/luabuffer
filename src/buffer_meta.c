#include "buffer_meta.h"

#include <lauxlib.h>
#include <lua.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#include "buffer.h"
#include "common.h"
#include "errors.h"

int l_buffer__gc(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  if (buf->buffer) FREE(buf->buffer);
  return 0;
}

int l_buffer__concat(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  Buffer* other = luaL_checkudata(L, 2, BUFFER_MT);

  Buffer* newbuf = lua_newuserdata(L, sizeof(Buffer));
  newbuf->size = buf->size + other->size;
  newbuf->buffer = malloc(newbuf->size);
  if (!newbuf->buffer) return throw_lua_oom(L, newbuf->size);

  memcpy(newbuf->buffer, buf->buffer, buf->size);
  memcpy(newbuf->buffer + buf->size, other->buffer, other->size);

  luaL_getmetatable(L, BUFFER_MT);
  lua_setmetatable(L, -2);

  return 1;
}

int l_buffer__eq(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  Buffer* other = luaL_checkudata(L, 2, BUFFER_MT);

  lua_pushboolean(L, buf->size == other->size &&
                         memcmp(buf->buffer, other->buffer, buf->size) == 0);
  return 1;
}

int l_buffer__len(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  lua_pushinteger(L, (lua_Integer)buf->size);
  return 1;
}

int l_buffer__index(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);

  if (lua_type(L, 2) == LUA_TNUMBER && lua_isinteger(L, 2)) {
    lua_Integer idx = luaL_checkinteger(L, 2);
    if (idx >= 1 && idx <= (lua_Integer)buf->size) {
      lua_pushinteger(L, (lua_Integer)buf->buffer[idx - 1]);
      return 1;
    }
  }

  luaL_getmetatable(L, BUFFER_MT);
  lua_getfield(L, -1, BUFFER_METHODSINDEX);
  lua_pushvalue(L, 2);
  lua_rawget(L, -2);
  return 1;
}

int l_buffer__newindex(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  size_t index = (size_t)luaL_checkinteger(L, 2);
  lua_Integer value = luaL_checkinteger(L, 3);

  if (index < 1 || index > buf->size)
    return luaL_error(L, "Index out of range");

  buf->buffer[index - 1] = (uint8_t)(value & 0xFF);
  return 0;
}

int l_buffer__tostring(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, BUFFER_MT);
  size_t display_len = (size_t)MIN(buf->size, BUFFER_INSPECT_MAX_BYTES);

  luaL_Buffer b;
  luaL_buffinit(L, &b);

  luaL_addstring(&b, "<Buffer ");

  for (size_t i = 0; i < display_len; ++i) {
    char tmp[4];  // 2 hex digits + space + null
    snprintf(tmp, sizeof(tmp), "%02x", buf->buffer[i]);
    luaL_addstring(&b, tmp);
    if (i != display_len - 1) luaL_addchar(&b, ' ');
  }

  if (buf->size > BUFFER_INSPECT_MAX_BYTES) {
    char tmp[64];
    snprintf(tmp, sizeof(tmp), " ... %zu more bytes",
             buf->size - BUFFER_INSPECT_MAX_BYTES);
    luaL_addstring(&b, tmp);
  }

  luaL_addstring(&b, ">");
  luaL_pushresult(&b);
  return 1;
}
