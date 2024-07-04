#include "pch.h"
#include "teardown/types.h"
#include "teardown/lua_helpers.h"

using namespace mp;
using namespace teardown;

// Internal Functions
//------------------------------------------------------------------------
int invokeLuaFunction(lua_State* L) {
    types::tLuaFunction func = reinterpret_cast<types::tLuaFunction>(lua_topointer(L, lua_upvalueindex(1)));
    const void* scriptCore = lua_topointer(L, lua_upvalueindex(2));
    const char* funcName = reinterpret_cast<const char*>(lua_topointer(L, lua_upvalueindex(3)));

    return func((types::script_core_t*)scriptCore, L, funcName);
}

// Public Functions
//------------------------------------------------------------------------
void lua_helpers::registerLuaFunction(types::script_core_t* scriptCore, types::td_string name, void* func) {
    lua_State* L = scriptCore->innerCore.state_info->state;

    lua_pushlightuserdata(L, func);
    lua_pushlightuserdata(L, scriptCore);
    lua_pushlightuserdata(L, (void*)name.c_str());
    lua_pushcclosure(L, invokeLuaFunction, 3);

     lua_setfield(L, LUA_GLOBALSINDEX, name.c_str());
}