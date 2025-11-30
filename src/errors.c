#include "errors.h"

#include <errno.h>
#include <lauxlib.h>
#include <lua.h>
#include <string.h>

int throw_luaoom(lua_State *L, size_t size) {
  return luaL_error(L, "memory allocation failed (%I bytes): %s (errno=%d)",
                    (lua_Integer)size, strerror(errno), errno);
}
