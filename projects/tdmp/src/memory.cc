#include "pch.h"
#include "memory.h"
#include "util/util.h"

using namespace tdmp;

static uint64_t base        = 0;
static uint32_t moduleSize  = 0;

// Internal Functions
//------------------------------------------------------------------------
static bool patternCompare(const uint8_t* data, const char* pattern_u8) {
    const char* pattern = pattern_u8;
    while (*pattern) {
        if (*pattern == ' ') {
            ++pattern;
            continue;
        }

        if (*pattern == '?') {
            ++data;
            if (*(pattern + 1) == '?') {
                ++pattern;
            }
        } else {
            char hexStr[3] = { pattern[0], pattern[1], '\0' };
            uint8_t byte = static_cast<uint8_t>(strtol(hexStr, nullptr, 16));
            if (*data != byte) {
                return false;
            }
            ++data;
            ++pattern;
        }
        ++pattern;
    }
    return true;
}

static int64_t readPtr(uint64_t addr, uint32_t offset) {
    int32_t rel_offset = *reinterpret_cast<int32_t*>(addr + offset);
    return static_cast<int64_t>(addr) + rel_offset + sizeof(int32_t) + static_cast<int64_t>(offset);
}

// Public Functions
//------------------------------------------------------------------------
bool mem::getModuleInfo() {
    if (base != 0) return true;

    MODULEINFO mi = { 0 };
    if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandleW(nullptr), &mi, sizeof(MODULEINFO))) {
        util::displayLastError(L"GetModuleInformation Failed");
        return false;
    }

    base = reinterpret_cast<uint64_t>(mi.lpBaseOfDll);
    moduleSize = mi.SizeOfImage;

    return true;
}

uint64_t mem::findIDAPattern(const char* pattern_u8, bool relative) {
    bool isRelative = relative || (pattern_u8[0] == 'E' && pattern_u8[1] == '8');

    for (size_t i = 0; i < moduleSize - 1; ++i) {
        if (patternCompare(reinterpret_cast<const uint8_t*>(base + i), pattern_u8)) {
            if (isRelative) {
                return readPtr(base + i, 1);
            }
            return base + i;
        }
    }

    return 0;
}

uint64_t mem::baseAddress() {
    return base;
}