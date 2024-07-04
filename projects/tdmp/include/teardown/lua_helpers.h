#ifndef TDMP_LUA_HELPERS_H
#define TDMP_LUA_HELPERS_H

#include "pch.h"
#include "teardown/types.h"

namespace mp::lua_helpers {

    void registerLuaFunction(mp::teardown::types::script_core_t* scriptCore, mp::teardown::types::td_string name, void* func);

}

#endif // TDMP_LUA_HELPERS_H