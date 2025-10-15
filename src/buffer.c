#include "buffer.h"

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#include "buffer_alloc.h"
#include "buffer_meta.h"
#include "buffer_rw.h"

static const luaL_Reg buffer_methods[] = {
    //
    {"tostring", l_buffer_tostring},
    {"readUInt32LE", l_buffer_read_u32le},
    {"readUInt32BE", l_buffer_read_u32be},
    {"readFloatLE", l_buffer_read_f32le},
    {"readFloatBE", l_buffer_read_f32be},
    {"readDoubleLE", l_buffer_read_f64le},
    {"readDoubleBE", l_buffer_read_f64be},
    {"writeUInt32LE", l_buffer_write_u32le},
    {"writeUInt32BE", l_buffer_write_u32be},
    {"writeFloatLE", l_buffer_write_f32le},
    {"writeFloatBE", l_buffer_write_f32be},
    {"writeDoubleLE", l_buffer_write_f64le},
    {"writeDoubleBE", l_buffer_write_f64be},
    {NULL, NULL}};

static const luaL_Reg buffer_meta[] = {
    //
    {"__gc", l_buffer__gc},
    {"__eq", l_buffer__eq},
    {"__len", l_buffer__len},
    {"__index", l_buffer__index},
    {"__concat", l_buffer__concat},
    {"__newindex", l_buffer__newindex},
    {"__tostring", l_buffer__tostring},
    {NULL, NULL}};

static const luaL_Reg buffer_module[] = {
    //
    {"from", l_buffer_from},
    {"alloc", l_buffer_alloc},
    {"allocUnsafe", l_buffer_alloc_unsafe},
    {NULL, NULL}};

int luaopen_buffer(lua_State* L) {
  luaL_newmetatable(L, BUFFER_MT);
  luaL_setfuncs(L, buffer_meta, 0);

  lua_newtable(L);
  luaL_setfuncs(L, buffer_methods, 0);
  lua_setfield(L, -2, BUFFER_METHODSINDEX);
  lua_pop(L, 1);

  luaL_newlib(L, buffer_module);
  return 1;
}
