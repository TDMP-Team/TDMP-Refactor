#ifndef TDMP_GENERATED_OFFSETS_H
#define TDMP_GENERATED_OFFSETS_H

#include "memory.h"

namespace tdmp::offsets {

    namespace game {
        static uint64_t log = 0x408b10;
    }

    namespace lua {
        static uint64_t lua_newstate = 0x39b360;
    }

    inline void generate() {
        const uint64_t base = mem::baseAddress();

        game::log         = base + game::log;
        lua::lua_newstate = base + lua::lua_newstate;
    }

}

#endif // TDMP_GENERATED_OFFSETS_H