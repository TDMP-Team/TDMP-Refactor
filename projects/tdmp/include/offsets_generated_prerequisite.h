#ifndef TDMP_GENERATED_OFFSETS_PREREQUISITE_H
#define TDMP_GENERATED_OFFSETS_PREREQUISITE_H

#include "pch.h"
#include "memory/memory.h"
#include "teardown/structures.h"

#define SET_AND_CHECK_OFFSET(addr, addr_name) \
    do { \
        addr = mem::baseAddress + addr; \
        console::writeln("  {} = 0x{:X}", addr_name, addr); \
        if (!mem::isAddressExecutable(addr)) { \
            failedOffsets.push_back(addr_name); \
        } \
    } while (0)

enum class log_level : uint8_t {
    debug = 0,
    info = 1,
    warning = 2,
    error = 3
};

#endif // TDMP_GENERATED_OFFSETS_PREREQUISITE_H