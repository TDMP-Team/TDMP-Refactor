#include "pch.h"
#include "memory/hooks.h"

using namespace mp::mem;

namespace {
    std::unordered_map<std::string, hooks::hook> hookMap;
}

bool hooks::addHook(const std::string& name, void* target, void* detour, void** orig, bool enabled) {
    MH_STATUS status = MH_CreateHook(target, detour, orig);
    if (status != MH_OK) {
        return false;
    }

    if (enabled) {
        status = MH_EnableHook((void*)target);
        if (status != MH_OK) {
            return false;
        }
    }

    hookMap.insert({ name, hooks::hook(name, target, enabled)});
    console::writeln("Inserted hook: {} {{ T: 0x{:X}, D: 0x{:X}, O: 0x{:X}, {} }}", name, (uintptr_t)target, (uintptr_t)detour, (uintptr_t)orig, enabled);

    return true;
}