#ifndef TDMP_DUMPER_H
#define TDMP_DUMPER_H

#include "pch.h"

namespace tdmp::dumper {

    struct function_type {
        const char* returnType = nullptr;
        const char* arguments = nullptr;

        constexpr function_type() = default;
        constexpr function_type(const char* r, const char* a) : returnType(r), arguments(a) {}
    };

    struct dumper_signature {
        const char* name;
        const char* sig;
        std::optional<function_type> function;

        constexpr dumper_signature(const char* n, const char* s, std::optional<function_type> optFunction = std::nullopt) : name(n), sig(s), function(optFunction) {}
    };

    struct signature_namespace {
        const char* name;
        std::span<const dumper_signature> signatures;
    };

    // Lua Signatures
    //------------------------------------------------------------------------
    static constexpr dumper_signature luaSignatures[] = {
        {
            "lua_newstate",
            "E8 ? ? ? ? 48 8B 4B 30 48 89 01",
            function_type("lua_State*", "lua_Alloc f, void* ud")
        }
    };

    // Game Signatures
    //------------------------------------------------------------------------
    static constexpr dumper_signature gameSignatures[] = {
        {
            "log",
            "E8 ? ? ? ? 3B 37",
            function_type("void", "log_level level, const char* fmt, ...")
        }
    };

    static constexpr dumper_signature tdSignatures[] = {
        {
            "initialize",
            "4C 89 44 24 18 48 89 4C 24 08 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C",
            function_type("structures::teardown*", "structures::teardown* magicShit, DWORD** a2, int64_t a3")
        }
    };

    static constexpr dumper_signature registrySignatures[] = {
        {
            "getInt",
            "40 53 48 83 EC 40 45 33 C0 E8 ?? ?? ?? ?? 48 8D 4C 24 20 48 85 C0 74 0B 48 8D 50 28 E8 ?? ?? ?? ?? EB 06 E8 ?? ?? ?? ?? 90 48 8D 4C",
            function_type("int", "uint8_t** a1, uint8_t** a2")
        }
    };

    static constexpr dumper_signature smallStringSignatures[] = {
        {
            "fromCString",
            "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 48 85 D2 C6",
            function_type("structures::small_string*", "structures::small_string* ss, const char* src")
        },

        {
            "free",
            "48 83 EC 28 80 79 1F 00 74 09",
            function_type("void", "structures::small_string* ss")
        }
    };

    // All Signatures
    //------------------------------------------------------------------------
    #define ADD_NAMESAPCE(name, arr) {##name, {arr, sizeof(arr) / sizeof(arr[0])}}

    static constexpr signature_namespace signatureNamespaces[] = {
        ADD_NAMESAPCE("lua", luaSignatures),
        ADD_NAMESAPCE("game", gameSignatures),
        ADD_NAMESAPCE("teardown", tdSignatures),
        ADD_NAMESAPCE("registry", registrySignatures),
        ADD_NAMESAPCE("small_string", smallStringSignatures)
    };

    bool dump();

}

#endif // TDMP_DUMPER_H