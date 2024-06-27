#ifndef TDMP_TEARDOWN_TYPES
#define TDMP_TEARDOWN_TYPES

#include "pch.h"
#include "teardown/structures.h"

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
    //namespace types {

    //    // Old type structure
    //    using lua_newstate = std::add_pointer_t<lua_State* (lua_Alloc f, void* ud)>;
    //    using tlog = std::add_pointer_t<int(const log_level level, const char* fmt, ...)>;
    //    using tinitialize = std::add_pointer_t<structures::teardown*(structures::teardown* magicShit, DWORD** a2, int64_t a3)>;

    //    // New type structure
    //    namespace registry {
    //        using tgetInt = std::add_pointer_t<int(uint8_t** a1, uint8_t** a2)>;
    //    }

    //    namespace small_string {
    //        using tfromCString = std::add_pointer_t<structures::small_string*(structures::small_string* ss, const char* src)>;
    //        using tFree = std::add_pointer_t<void(structures::small_string* ss)>;
    //    }
    //
    //}

    //// Functions
    ////------------------------------------------------------------------------
    //// Old func structure
    //namespace funcs {
    //    static types::lua_newstate tlua_newstate = nullptr;
    //    static types::tlog tlog = nullptr;
    //    static types::tinitialize tinitialize = nullptr;
    //}

    //// New func structure
    //namespace registry {
    //    static types::registry::tgetInt getInt = nullptr;
    //}

    //namespace small_string {
    //    static types::small_string::tfromCString fromCString = nullptr;
    //    static types::small_string::tFree free = nullptr;
    //}

}

#endif // TDMP_TEARDOWN_TYPES