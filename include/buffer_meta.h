#pragma once

#include <lua.h>

int l_buffer__gc(lua_State* L);
int l_buffer__eq(lua_State* L);
int l_buffer__len(lua_State* L);
int l_buffer__index(lua_State* L);
int l_buffer__concat(lua_State* L);
int l_buffer__newindex(lua_State* L);
int l_buffer__tostring(lua_State* L);