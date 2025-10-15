#include "errors.h"

#include <errno.h>
#include <lauxlib.h>
#include <lua.h>
#include <string.h>

int push_lua_errno(lua_State* L) {
  lua_pushnil(L);
  lua_pushstring(L, strerror(errno));
  lua_pushinteger(L, errno);
  return 3;
}

int throw_lua_oom(lua_State* L, size_t size) {
  int err = errno;
  return luaL_error(L, "memory allocation failed (%I bytes): %s (errno=%d)",
                    (lua_Integer)size, strerror(err), err);
}
