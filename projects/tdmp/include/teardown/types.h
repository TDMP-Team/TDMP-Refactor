#ifndef TDMP_TEARDOWN_TYPES
#define TDMP_TEARDOWN_TYPES

#include "pch.h"
#include "teardown/structures.h"

namespace tdmp::teardown::types {

    // Enums
    //------------------------------------------------------------------------
    enum class log_level : uint8_t {
        debug = 0,
        info = 1,
        warning = 2,
        error = 3
    };

}

#endif // TDMP_TEARDOWN_TYPES