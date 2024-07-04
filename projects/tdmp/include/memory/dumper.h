#ifndef TDMP_DUMPER_H
#define TDMP_DUMPER_H

#include "pch.h"

namespace mp::dumper {

    // Structures
    //------------------------------------------------------------------------
    // This is used in the dumper to generate the functions and types, you can see this below where the signatures are.
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
            function_type("void", "mp::teardown::types::log_level level, const char* fmt, ...")
        }
    };

    static constexpr dumper_signature tdSignatures[] = {
        {
            "initialize",
            "4C 89 44 24 18 48 89 4C 24 08 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C",
            function_type("mp::teardown::types::game*", "mp::teardown::types::game* game, DWORD** a2, int64_t a3")
        },

        {
            "update",
            "48 8B C4 48 89 58 18 48 89 50 10 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C",
            function_type("mp::teardown::types::game*", "mp::teardown::types::game* game, int64_t input")
        }
    };

    static constexpr dumper_signature registrySignatures[] = {
        {
            "getInt",
            "40 53 48 83 EC 40 45 33 C0 E8 ?? ?? ?? ?? 48 8D 4C 24 20 48 85 C0 74 0B 48 8D 50 28 E8 ?? ?? ?? ?? EB 06 E8 ?? ?? ?? ?? 90 48 8D 4C",
            function_type("int", "uint8_t** registry, mp::teardown::types::td_string* str")
        },

        {
            "getFloat",
            "48 83 EC 58 0F 29 74 24 40 45",
            function_type("float", "uint8_t** registry, mp::teardown::types::td_string* str")
        },

        {
            "getString",
            "40 53 48 83 EC 30 49 8B C0 48 8B DA 48",
            function_type("mp::teardown::types::td_string*", "uint8_t** registry, mp::teardown::types::td_string* str")
        },

        {
            "setBool",
            "48 89 5C 24 08 57 48 83 EC 40 48 8B DA 48 8B F9 48 8D 05",
            function_type("void", "uint8_t** a1, mp::teardown::types::td_string* str, bool value")
        },

        {
            "setString",
            "48 89 5C 24 10 57 48 83 EC 20 49 8B D8 41",
            function_type("void", "uint8_t** a1, mp::teardown::types::td_string* str, mp::teardown::types::td_string* value")
        },
    };

    static constexpr dumper_signature scriptCoreSignatures[] = {
        {
            "registerLuaFunctions",
            "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 40 48 8D",
            function_type("void", "mp::teardown::types::script_core_t* scriptCore")
        },

        {
            "loadScript",
            "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 41 56 41 57 48 8B EC 48 83 EC 40",
            function_type("void", "mp::teardown::types::script_core_t* scriptCore, mp::teardown::types::td_string* name")
        },

        {
            "destroy",
            "E8 ? ? ? ? F6 C3 01 74 0D BA ? ? ? ? 48 8B CF E8 ? ? ? ? 48 8B 5C 24 ?",
            function_type("void", "mp::teardown::types::script_core_t* scriptCore, char a2")
        }
    };

    // All Signatures
    //------------------------------------------------------------------------
    #define ADD_NAMESAPCE(name, arr) {##name, {arr, sizeof(arr) / sizeof(arr[0])}}

    static constexpr signature_namespace signatureNamespaces[] = {
        ADD_NAMESAPCE("lua", luaSignatures),
        ADD_NAMESAPCE("script_core", scriptCoreSignatures),
        ADD_NAMESAPCE("game", gameSignatures),
        ADD_NAMESAPCE("teardown", tdSignatures),
        ADD_NAMESAPCE("registry", registrySignatures)
    };

    bool dump(bool genStructs);

}

#endif // TDMP_DUMPER_H