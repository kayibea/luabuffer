#pragma once

#include <lua.h>

int l_buffer_tostring(lua_State* L);
int l_buffer_write_string(lua_State* L);
int l_buffer_read_u16le(lua_State* L);
int l_buffer_write_u16le(lua_State* L);
int l_buffer_read_i16le(lua_State* L);
int l_buffer_write_i16le(lua_State* L);
int l_buffer_read_u32le(lua_State* L);
int l_buffer_read_u32be(lua_State* L);
int l_buffer_write_u32le(lua_State* L);
int l_buffer_write_u32be(lua_State* L);
int l_buffer_read_f32le(lua_State* L);
int l_buffer_read_f32be(lua_State* L);
int l_buffer_write_f32le(lua_State* L);
int l_buffer_write_f32be(lua_State* L);
int l_buffer_read_f64le(lua_State* L);
int l_buffer_read_f64be(lua_State* L);
int l_buffer_write_f64le(lua_State* L);
int l_buffer_write_f64be(lua_State* L);