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

    extern uint64_t baseAddress;
    extern uint32_t moduleSize;

    uint64_t findIDAPattern(const char* pattern_u8, bool relative = false);

    bool initializeMemory();

    bool isSectionLoaded(HMODULE module, const char* section);
    void waitForSection(HMODULE hModule, const char* sectionName);

    bool isAddressExecutable(uintptr_t address);

}

#endif // TDMP_MEMORY_H