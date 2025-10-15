#pragma once

#include <lua.h>
#include <stddef.h>

#include "buffer.h"

#define ERR_INVALID_HEX_STRING "Invalid hex string"
#define ERR_OFFSET_OUT_OF_RANGE "Offset out of range"

#define ERR_UNSUPPORTED_ENCODING \
  "Unsupported encoding: \"%s\" (supported: " SUPPORTED_ENCODINGS ")"

int push_lua_errno(lua_State* L);
int throw_lua_oom(lua_State* L, size_t size);
