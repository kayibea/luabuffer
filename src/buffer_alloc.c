#include "buffer_alloc.h"

#include <lauxlib.h>
#include <lua.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "buffer.h"
#include "common.h"
#include "errors.h"
#include "hexlib.h"

#define ERR_INVALID_BUFFERLEN                                                  \
  "The value of \"size\" is out of range. It must be >= 0 && <= %I. Received " \
  "%I"

// luaL_Stream

static int buffer_alloc_ftable(lua_State* L);
static int buffer_alloc_fstream(lua_State* L);
static int buffer_alloc_fstring(lua_State* L);
static int buffer_alloc_fbuffer(lua_State* L);

int l_buffer_from(lua_State* L) {
  int type = lua_type(L, 1);

  switch (type) {
    case LUA_TSTRING:
      return buffer_alloc_fstring(L);
    case LUA_TTABLE:
      return buffer_alloc_ftable(L);
    case LUA_TUSERDATA: {
      if (luaL_testudata(L, 1, BUFFER_MT)) return buffer_alloc_fbuffer(L);
      if (luaL_testudata(L, 1, LUA_FILEHANDLE)) return buffer_alloc_fstream(L);
      [[fallthrough]];
    }
    default: {
      const char* tname = luaL_typename(L, 1);
      return luaL_error(L,
                        "Bad argument #1 to 'from' (expected string, table, "
                        "buffer* or file*, got %s)",
                        tname);
    }
  }
}

static int buffer_alloc_fstream(lua_State* L) {
  luaL_Stream* stream = luaL_checkudata(L, 1, LUA_FILEHANDLE);
  if (!stream || !stream->f)
    return luaL_error(L, "Invalid file handle (file closed or invalid)");

  lua_Integer size = luaL_optinteger(L, 2, -1);
  bool seekable = (fseek(stream->f, 0, SEEK_CUR) == 0);

  if (!seekable && size < 0)
    return luaL_error(L, "Size must be provided for non-seekable streams");

  if (seekable && size < 0) {
    long cur = ftell(stream->f);
    fseek(stream->f, 0, SEEK_END);
    long end = ftell(stream->f);
    fseek(stream->f, cur, SEEK_SET);
    size = end - cur;
  }

  if (size <= 0) return luaL_error(L, "Cannot create zero-length buffer");

  uint8_t* data = malloc((size_t)size);
  if (!data) return throw_luaoom(L, size);

  size_t got = fread(data, 1, (size_t)size, stream->f);
  if (got == 0) {
    FREE(data);
    return luaL_error(L, "Failed to read from file (empty or unreadable)");
  }

  Buffer* buf = lua_newuserdata(L, sizeof(Buffer));
  buf->buffer = data;
  buf->size = got;

  luaL_getmetatable(L, BUFFER_MT);
  lua_setmetatable(L, -2);

  return 1;
}

static int buffer_alloc_ftable(lua_State* L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  size_t len = (size_t)lua_rawlen(L, 1);

  Buffer* buf = lua_newuserdata(L, sizeof(Buffer));
  buf->size = len;
  buf->buffer = malloc(len);
  if (!buf->buffer) return throw_luaoom(L, len);

  for (size_t i = 1; i <= len; i++) {
    lua_rawgeti(L, 1, (lua_Integer)i);
    if (!lua_isnumber(L, -1)) {
      const char* tname = luaL_typename(L, -1);
      return luaL_error(L,
                        "Invalid value at index %I (number expected, got %s)",
                        (lua_Integer)i, tname);
    }

    lua_Integer v = lua_tointeger(L, -1) & 0xFF;
    buf->buffer[i - 1] = (uint8_t)v;
    lua_pop(L, 1);
  }

  luaL_getmetatable(L, BUFFER_MT);
  lua_setmetatable(L, -2);
  return 1;
}

static int buffer_alloc_fbuffer(lua_State* L) {
  Buffer* src = luaL_checkudata(L, 1, BUFFER_MT);
  Buffer* buf = lua_newuserdata(L, sizeof(Buffer));

  buf->size = src->size;
  buf->buffer = malloc(src->size);
  if (!buf->buffer) return throw_luaoom(L, src->size);

  memcpy(buf->buffer, src->buffer, src->size);

  luaL_getmetatable(L, BUFFER_MT);
  lua_setmetatable(L, -2);

  return 1;
}

// TODO: Handle empty string?
static int buffer_alloc_fstring(lua_State* L) {
  size_t in_len;
  const char* input = luaL_checklstring(L, 1, &in_len);
  const char* encoding = luaL_optstring(L, 2, ENCODING_UTF8);

  uint8_t* decoded = NULL;
  size_t out_len = 0;

  if (strcasecmp(encoding, ENCODING_UTF8) == 0) {
    decoded = malloc(in_len);
    if (!decoded) return throw_luaoom(L, in_len);
    memcpy(decoded, input, in_len);
    out_len = in_len;
  } else if (strcasecmp(encoding, ENCODING_BASE16) == 0) {
    decoded = hex_decode((const char*)input, &out_len);
    if (!decoded) return luaL_error(L, ERR_INVALID_HEX_STRING);
  } else {
    return luaL_error(L, ERR_UNSUPPORTED_ENCODING, encoding);
  }

  Buffer* buf = lua_newuserdata(L, sizeof(Buffer));
  buf->buffer = decoded;
  buf->size = out_len;

  luaL_getmetatable(L, BUFFER_MT);
  lua_setmetatable(L, -2);

  return 1;
}

int l_buffer_alloc_unsafe(lua_State* L) {
  lua_Integer size = luaL_checkinteger(L, 1);

  if (size < 0)
    return luaL_error(L, ERR_INVALID_BUFFERLEN, LUA_MAXINTEGER, size);

  Buffer* buf = lua_newuserdata(L, sizeof(Buffer));
  buf->size = (size_t)size;
  buf->buffer = malloc(buf->size);

  if (!buf->buffer) return throw_luaoom(L, buf->size);

  luaL_getmetatable(L, BUFFER_MT);
  lua_setmetatable(L, -2);

  return 1;
}

int l_buffer_alloc(lua_State* L) {
  lua_Integer size = luaL_checkinteger(L, 1);
  bool canfill = (size > 0 && !lua_isnoneornil(L, 2));
  const char* encoding = luaL_optstring(L, 3, ENCODING_UTF8);

  if (size < 0)
    return luaL_error(L, ERR_INVALID_BUFFERLEN, LUA_MAXINTEGER, size);

  Buffer* buf = lua_newuserdata(L, sizeof(Buffer));
  buf->size = (size_t)size;
  buf->buffer = calloc(buf->size, 1);

  if (!buf->buffer) return throw_luaoom(L, buf->size);

  if (canfill) {
    int type = lua_type(L, 2);

    switch (type) {
      case LUA_TUSERDATA: {
        Buffer* src = luaL_checkudata(L, 2, BUFFER_MT);
        for (size_t i = 0; i < buf->size; i++)
          buf->buffer[i] = src->buffer[i % src->size];
        break;
      }

      case LUA_TNUMBER: {
        lua_Number d = luaL_checknumber(L, 2);
        uint8_t fill = (uint8_t)(((int)d) & 0xFF);
        memset(buf->buffer, fill, buf->size);
        break;
      }

      case LUA_TSTRING: {
        size_t fill_len;
        const char* fill_str = luaL_checklstring(L, 2, &fill_len);

        uint8_t* data = NULL;
        size_t data_len = 0;

        if (strcasecmp(encoding, ENCODING_UTF8) == 0) {
          data = (uint8_t*)fill_str;
          data_len = fill_len;
        } else if (strcasecmp(encoding, ENCODING_BASE16) == 0) {
          data = hex_decode(fill_str, &data_len);
          if (!data) return luaL_error(L, ERR_INVALID_HEX_STRING);
        } else {
          return luaL_error(L, ERR_UNSUPPORTED_ENCODING, encoding);
        }

        for (size_t i = 0; i < buf->size; i++)
          buf->buffer[i] = data[i % data_len];

        if (data != (uint8_t*)fill_str) FREE(data);
        break;
      }

      default:
        return luaL_error(
            L, "Invalid fill type: must be number, string, or buffer");
    }
  }

  luaL_getmetatable(L, BUFFER_MT);
  lua_setmetatable(L, -2);

  return 1;
}
