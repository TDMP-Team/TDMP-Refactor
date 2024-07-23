#ifndef TDMP_LUA_HELPERS_H
#define TDMP_LUA_HELPERS_H

#include "pch.h"
#include "teardown/types.h"

namespace mp::lua_helpers {

    void registerLuaFunction(td::script_core_t* scriptCore, td::td_string name, void* func);

}

#endif // TDMP_LUA_HELPERS_H