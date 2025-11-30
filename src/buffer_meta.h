#pragma once

#include <lua.h>

int l_buffer_meta_gc(lua_State *L);
int l_buffer_meta_eq(lua_State *L);
int l_buffer_meta_len(lua_State *L);
int l_buffer_meta_index(lua_State *L);
int l_buffer_meta_concat(lua_State *L);
int l_buffer_meta_newindex(lua_State *L);
int l_buffer_meta_tostring(lua_State *L);
