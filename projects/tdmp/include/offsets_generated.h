#ifndef TDMP_GENERATED_OFFSETS_H
#define TDMP_GENERATED_OFFSETS_H

#include "memory/memory.h"

namespace tdmp::offsets {

    namespace lua {
        inline uint64_t lua_newstate = 0x39b360;
    }

    namespace game {
        inline uint64_t log = 0x408b10;
    }

    inline void generate() {
        const uint64_t base = mem::baseAddress();

        lua::lua_newstate = base + lua::lua_newstate;
        game::log = base + game::log;
    }

}

#endif // TDMP_GENERATED_OFFSETS_H