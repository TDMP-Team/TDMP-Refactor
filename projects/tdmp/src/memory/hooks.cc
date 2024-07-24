#include "pch.h"
#include "memory/hooks.h"

using namespace mp::mem;

namespace {
    std::unordered_map<std::string, hooks::hook> hookMap;
}

bool hooks::addHook(const std::string& name, void* target, void* detour, void** orig, bool enabled) {
    auto it = hookMap.find(name);
    if (it != hookMap.end()) {
        return true;
    }

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

bool hooks::removeHook(const std::string& name) {
    auto it = hookMap.find(name);
    if (it == hookMap.end()) {
        console::writeln("Hook not found: {}", name);
        return false;
    }

    const auto& hook = it->second;

    if (hook.enabled) {
        MH_STATUS status = MH_DisableHook(hook.target);
        if (status != MH_OK) {
            console::writeln("Failed to disable hook: {} (Error: {})", name, MH_StatusToString(status));
            return false;
        }
    }

    MH_STATUS status = MH_RemoveHook(hook.target);
    if (status != MH_OK) {
        console::writeln("Failed to remove hook: {} (Error: {})", name, MH_StatusToString(status));
        return false;
    }

    hookMap.erase(it);
    
    console::writeln("Removed hook: {}", name);
    return true;
}