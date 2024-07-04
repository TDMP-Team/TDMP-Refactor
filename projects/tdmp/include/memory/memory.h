#ifndef TDMP_MEMORY_H
#define TDMP_MEMORY_H

#include "pch.h"

struct structure_order_element {
    std::string name;
    std::string type;
    bool isStruct;

    structure_order_element(const std::string& name, const std::string& type = "", bool isStruct = false) : name(name), type(type), isStruct(isStruct) {}
};

#define HINT_CALLBACK_ARGS uintptr_t base, std::vector<structure_order_element>& structureOrder, std::vector<int32_t>& offsetOrder
#define HINT_CALLBACK_TYPE std::function<void(HINT_CALLBACK_ARGS)>

namespace mp::mem {

    struct signature {
        const char* name;
        const char* sig;
        bool        relative;

        constexpr signature(const char* n, const char* s, bool r = false) : name(n), sig(s), relative(r) {}
    };

    struct game_function {
        std::string name = "";
        uintptr_t address = 0x0;
        std::optional<HINT_CALLBACK_TYPE> hintCallback = std::nullopt;

        game_function(const std::string& n) : name(n) {}
        game_function(const std::string& n, std::optional<HINT_CALLBACK_TYPE> cb) : name(n), hintCallback(cb) {}
        game_function(const std::string& n, uintptr_t addr, std::optional<HINT_CALLBACK_TYPE> cb = std::nullopt) : name(n), address(addr), hintCallback(cb) {}
    };

    extern uint64_t baseAddress;
    extern uint32_t moduleSize;

    bool initializeMemory();
    uint64_t findIDAPattern(const char* pattern_u8, bool relative = false);

    bool isSectionLoaded(HMODULE module, const char* section);
    void waitForSection(HMODULE hModule, const char* sectionName);

    bool isAddressExecutable(uintptr_t address);
    bool findGameFunctionOffsets(std::vector<game_function>& functions);

    inline int32_t const readInt8(uintptr_t address)  { return *(int8_t*)(address); }
    inline int32_t const readInt16(uintptr_t address) { return *(int16_t*)(address); }
    inline int32_t const readInt32(uintptr_t address) { return *(int32_t*)(address); }
    inline int64_t const readInt64(uintptr_t address) { return *(int64_t*)(address); }
    inline uintptr_t const readUintptr (uintptr_t address) { return *(uintptr_t*)(address); }
}

#endif // TDMP_MEMORY_H