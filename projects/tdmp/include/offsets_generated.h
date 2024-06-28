#ifndef TDMP_GENERATED_OFFSETS_H
#define TDMP_GENERATED_OFFSETS_H

// Generated by dumper.cc
// Date: Jun 28 2024
// Time: 09:13:04

#include "offsets_generated_prerequisite.h"

namespace tdmp {
    namespace offsets {
        namespace lua {
            inline uint64_t lua_newstate = 0x39b360;
        }

        namespace game {
            inline uint64_t log = 0x408b10;
        }

        namespace teardown {
            inline uint64_t initialize = 0x5a5b0;
        }

        namespace registry {
            inline uint64_t getInt = 0x1fffb0;
        }

        namespace small_string {
            inline uint64_t fromCString = 0x289430;
            inline uint64_t free = 0x2895a0;
        }

        inline bool generate(std::vector<std::string>& failedOffsets) {
            console::writeln("Function Addresses:");

            SET_AND_CHECK_OFFSET(lua::lua_newstate, "lua::lua_newstate");
            SET_AND_CHECK_OFFSET(game::log, "game::log");
            SET_AND_CHECK_OFFSET(teardown::initialize, "teardown::initialize");
            SET_AND_CHECK_OFFSET(registry::getInt, "registry::getInt");
            SET_AND_CHECK_OFFSET(small_string::fromCString, "small_string::fromCString");
            SET_AND_CHECK_OFFSET(small_string::free, "small_string::free");

            return failedOffsets.size() == 0;
        }
    }

    namespace funcs {
        namespace types {
            namespace lua {
                using tlua_newstate = std::add_pointer_t<lua_State* (lua_Alloc f, void* ud)>;
            }

            namespace game {
                using tlog = std::add_pointer_t<void(tdmp::teardown::types::log_level level, const char* fmt, ...)>;
            }

            namespace teardown {
                using tinitialize = std::add_pointer_t<structures::teardown* (structures::teardown* magicShit, DWORD** a2, int64_t a3)>;
            }

            namespace registry {
                using tgetInt = std::add_pointer_t<int(uint8_t** a1, uint8_t** a2)>;
            }

            namespace small_string {
                using tfromCString = std::add_pointer_t<structures::small_string* (structures::small_string* ss, const char* src)>;
                using tfree = std::add_pointer_t<void(structures::small_string* ss)>;
            }

        }

        namespace lua {
            static types::lua::tlua_newstate lua_newstate = nullptr;
        }

        namespace game {
            static types::game::tlog log = nullptr;
        }

        namespace teardown {
            static types::teardown::tinitialize initialize = nullptr;
        }

        namespace registry {
            static types::registry::tgetInt getInt = nullptr;
        }

        namespace small_string {
            static types::small_string::tfromCString fromCString = nullptr;
            static types::small_string::tfree free = nullptr;
        }


        inline bool assign() {
            console::writeln("Assigning functions");

            ASSIGN_FUNCTION(lua, lua_newstate);
            ASSIGN_FUNCTION(game, log);
            ASSIGN_FUNCTION(teardown, initialize);
            ASSIGN_FUNCTION(registry, getInt);
            ASSIGN_FUNCTION(small_string, fromCString);
            ASSIGN_FUNCTION(small_string, free);

            return true;
        }
    }
}

#endif // TDMP_GENERATED_OFFSETS_H