#ifndef TDMP_TEARDOWN_SCRIPT_CORE_H
#define TDMP_TEARDOWN_SCRIPT_CORE_H

#include "pch.h"

namespace td {

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    struct lua_state_info_t {
        lua_State* state;
    };

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    struct script_core_inner_t {
        uint8_t padding[0x30];
        lua_state_info_t* state_info;
    };

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    struct internal_check_t {
        uint8_t padding[0x38C];
        uint32_t privilege;
    };

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    struct script_core_t {
        char padding1[8];
        float time;
        float dt;
        td::td_string path;
        td::td_string location;
        uint8_t padding2[24];
        script_core_inner_t innerCore;
        uint8_t padding4[504];
        td::td_vector<uint32_t> entities;
        uint8_t padding5[24];
        internal_check_t* check_internal;
    };

}

#endif // TDMP_TEARDOWN_SCRIPT_CORE_H