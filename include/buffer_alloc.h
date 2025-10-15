#pragma once

#include <lua.h>

int l_buffer_from(lua_State* L);
int l_buffer_alloc(lua_State* L);
int l_buffer_alloc_unsafe(lua_State* L);