#ifndef TDMP_MEMORY_H
#define TDMP_MEMORY_H

#include "pch.h"

namespace tdmp::mem {
    struct signature {
        char* pattern;
        char* mask;
        bool  relative;
    };

    bool initializeMemory();
    uint64_t findIDAPattern(const char* pattern_u8, bool relative = false);
    uint64_t baseAddress();
}

#endif // TDMP_MEMORY_H