#ifndef TDMP_MEMORY_H
#define TDMP_MEMORY_H

#include "pch.h"

namespace tdmp::mem {
    struct signature {
        char* pattern;
        char* mask;
        bool  relative;
    };

    bool getModuleInfo();
    uint64_t findIDAPattern(const char* pattern_u8, bool relative = false);
    uint64_t baseAddress();

    // Hooks
    //------------------------------------------------------------------------
    struct hook {
        hook(std::string_view _name, void* _target, bool _enabled) : name(_name), target(_target), enabled(_enabled) {}

        std::string name;
        void* target;
        bool enabled;
    };

    static std::unordered_map<std::string, hook> hooks;

    template<typename T, typename D, typename O>
    bool make_hook(const std::string_view name, T target, D detour, O* orig, bool enabled = true) {
        auto res = MH_CreateHook((void*)target, (void*)detour, (void**)orig);
        if (res != MH_OK) {
//             console::writeln("Failed creating hook for {}: {}", name, MH_StatusToString(res));
            std::cout << "Failed creating hook for " << name << ": " << MH_StatusToString(res);
            return false;
        }

//         console::writeln("Created hook for {} -> {:#x}", name, target)
        std::cout << "Created hook for " << name << " -> 0x" << std::hex << target << '\n';

//         hooks[name] = hook(name, (void*)target, enabled);

        if (!enabled)
            return true;

        res = MH_EnableHook((void*)target);
        if (res != MH_OK) {
//             console::writeln("Failed enabling hook {}", name);
            return false;
        }

        return true;
    }
}

#endif // TDMP_MEMORY_H