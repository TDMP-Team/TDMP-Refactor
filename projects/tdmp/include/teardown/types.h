#ifndef TDMP_TEARDOWN_TYPES
#define TDMP_TEARDOWN_TYPES

#include "pch.h"

namespace tdmp::teardown {

    // Enums
    //------------------------------------------------------------------------
    enum class log_level : uint8_t {
        debug = 0,
        info = 1,
        warning = 2,
        error = 3
    };

    // Types
    //------------------------------------------------------------------------
    namespace types {

        using lua_newstate = std::add_pointer_t<lua_State* (lua_Alloc f, void* ud)>;
        using tlog = std::add_pointer_t<int(const log_level level, const char* fmt, ...)>;
    
    }

    // Functions
    //------------------------------------------------------------------------
    namespace funcs {

        static types::lua_newstate tlua_newstate = nullptr;
        static types::tlog tlog = nullptr;

    }

}

#endif // TDMP_TEARDOWN_TYPES