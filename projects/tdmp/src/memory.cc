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

int64_t readPtr(uint64_t addr, uint32_t offset) {
    int32_t rel_offset = *reinterpret_cast<int32_t*>(addr + offset);
    return static_cast<int64_t>(addr) + rel_offset + sizeof(int32_t) + static_cast<int64_t>(offset);
}

static std::string validate_ida_pattern(const std::string& code) {
    std::string result = "\\x" + code;
    util::replaceAllOccurrences(result, "??", "?");
    util::replaceAllOccurrences(result, "?",  "CC");
    util::replaceAllOccurrences(result, " ", "\\x");
    return result;
}

static std::string create_mask_for_ida(std::string_view pattern) {
    const uint16_t mask_len = static_cast<uint16_t>(pattern.length() / 4);
    std::string result;
    result.reserve(mask_len);

    for (size_t i = 1; i < mask_len + 1; i++) {
        auto idx = i * 4 - 1;
        if (pattern[idx] == 'C' && pattern[idx - 1] == 'C') {
            result.push_back('?');
        } else {
            result.push_back('x');
        }
    }

    return result;
}

static std::string escape_ida_pattern(const std::string& pattern) {
    std::string result;
    result.reserve(pattern.length() / 4);

    for (size_t i = 0; i < pattern.length(); i++) {
        if (pattern[i] == '\\' && pattern[i + 1] == 'x') {
            if (i + 3 < pattern.length()) {
                std::string hex = pattern.substr(i + 2, 2);
                char escaped_char = std::stoi(hex, 0, 16);
                result.push_back(escaped_char);
                i += 3;
            } else {
                result.push_back('\\');
                result.push_back('x');
            }
        } else {
            result.push_back(pattern[i]);
        }
    }

    return result;
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

// uint64_t mem::findIDAPattern(const char* pattern_u8, bool relative) {
//     bool isRelative = relative || (pattern_u8[0] == 'E' && pattern_u8[1] == '8');
// 
//     for (size_t i = 0; i < moduleSize - 1; ++i) {
//         if (patternCompare(reinterpret_cast<const uint8_t*>(base + i), pattern_u8)) {
//             if (isRelative) {
//                 return readPtr(base + i, 1);
//             }
//             return base + i;
//         }
//     }
// 
//     return 0;
// }

bool mem::compare_pattern(const char* data, const char* pattern, const char* mask) {
    for (; *mask; ++mask, ++data, ++pattern) {
        if (*mask == 'x' && *data != *pattern)
            return false;
    }

    return true;
}

int64_t mem::read_ptr(int64_t addr, int32_t off) {
    const int32_t rel = *reinterpret_cast<int32_t*>(addr + off);
    return addr + rel + sizeof(int32_t) + static_cast<int64_t>(off);
}

uint64_t mem::find_ida_pattern(const std::string& ida, bool relative) {
    std::string pattern = validate_ida_pattern(ida);
    std::string mask = create_mask_for_ida(pattern);
    std::string escaped = escape_ida_pattern(pattern);

    // console::writeln("pattern: {}", pattern);
    // console::writeln("mask: {}", mask);
    // console::writeln("escaped: {}", escaped);

    return mem::find_code_pattern(mem::signature {
        escaped.data(),
        mask.data(),
        relative
    });
}

uint64_t mem::find_code_pattern(mem::signature sig) {
    for (int i = 0; i < static_cast<int32_t>(moduleSize); i++) {
        if (!mem::compare_pattern(reinterpret_cast<char*>(base + i), sig.pattern, sig.mask))
            continue;

        if (sig.relative)
            return mem::read_ptr((base + i), 1);
        return base + i;
    }

    return 0;
}