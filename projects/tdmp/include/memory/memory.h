#ifndef TDMP_MEMORY_H
#define TDMP_MEMORY_H

#include "pch.h"

namespace tdmp::mem {

    struct signature {
        const char* name;
        const char* sig;
        bool        relative;

        constexpr signature(const char* n, const char* s, bool r = false) : name(n), sig(s), relative(r) {}
    };

    uint64_t findIDAPattern(const char* pattern_u8, bool relative = false);
    uint64_t baseAddress();

    bool initializeMemory();
    bool dumpOffsets();

    bool isSectionLoaded(HMODULE module, const char* section);
    void waitForSection(HMODULE hModule, const char* sectionName);

}

#endif // TDMP_MEMORY_H