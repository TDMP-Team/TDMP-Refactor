#include "pch.h"
#include "memory/dumper.h"
#include "memory/memory.h"
#include "memory/function_hints.h"
#include "teardown/types.h"
#include "util.h"

using namespace mp;

constexpr const char* headerTemplate = R"(#ifndef TDMP_GENERATED_OFFSETS_H
#define TDMP_GENERATED_OFFSETS_H

// Generated by dumper.cc
// Date: {0}
// Time: {1}

#include "offsets_generated_prerequisite.h"

namespace mp {{
    namespace offsets {{
{2}        inline bool generate(std::vector<std::string>& failedOffsets) {{
            console::writeln("Function Addresses:");

{3}
            return failedOffsets.size() == 0;
        }}
    }}

    namespace funcs {{
        namespace types {{
{4}        }}

{5}
        inline bool assign() {{
            console::writeln("Assigning functions");

{6}
            return true;
        }}
    }}

    namespace lua_funcs {{
{7}

        inline bool assign() {{
{8}
            return true;
        }}
    }}
}}

#endif // TDMP_GENERATED_OFFSETS_H)";

struct lua_func_category {
    std::string category;
    std::vector<mem::game_function> funcs;
};

struct game_structure {
    std::vector<structure_order_element> members;
    std::vector<int32_t>     offsets;
};

struct structure_member {
    std::string name;
    std::string type;
    int32_t offset;
    std::vector<structure_member> nestedMembers;
};

static void processSignature(const std::string& nsName, const dumper::dumper_signature& sig,
                             std::stringstream& offsets, std::stringstream& checks,
                             std::stringstream& types, std::stringstream& functions,
                             std::stringstream& assignFuncs) {
    const uint64_t address = mem::findIDAPattern(sig.sig, false);
    const uint64_t offsetFromBase = address - mem::baseAddress;

    console::setStatus("Generating offset {} {:#x}", sig.name, offsetFromBase);

    std::string offsetStr = std::format("{:#x}", offsetFromBase);
    offsets << std::format("            inline uint64_t {0} = {1};\n", sig.name, offsetStr);

    checks << std::format("            SET_AND_CHECK_OFFSET({0}::{1}, \"{0}::{1}\");\n", nsName, sig.name);
    assignFuncs << std::format("            ASSIGN_FUNCTION({0}, {1});\n", nsName, sig.name);

    console::writeln("{}::{} = {:#x}", nsName, sig.name, offsetFromBase);

    if (sig.function.has_value()) {
        dumper::function_type func = sig.function.value();
        types << std::format("                using t{0} = std::add_pointer_t<{1}({2})>;\n",
                             sig.name, func.returnType, func.arguments);
        functions << std::format("            inline types::{0}::t{1} {1} = nullptr;\n", nsName, sig.name);
    }
}

static void generateTemplate(const dumper::signature_namespace* signatureNamespaces, size_t namespaceCount,
                             std::stringstream& offsets, std::stringstream& checks,
                             std::stringstream& types, std::stringstream& functions,
                             std::stringstream& assignFuncs) {
    for (size_t i = 0; i < namespaceCount; ++i) {
        const auto& ns = signatureNamespaces[i];
        offsets << std::format("        namespace {0} {{\n", ns.name);
        types << std::format("            namespace {0} {{\n", ns.name);
        functions << std::format("        namespace {0} {{\n", ns.name);

        for (const auto& sig : ns.signatures) {
            processSignature(ns.name, sig, offsets, checks, types, functions, assignFuncs);
        }

        offsets << "        }\n\n";
        types << "            }\n\n";
        functions << "        }\n\n";
    }
}

void generateStructures(std::vector<lua_func_category>& categories, std::stringstream& ss, const std::vector<game_structure>& structures) {
    std::unordered_map<std::string, structure_member> structureMap;

    std::unordered_map<std::string, size_t> typeSizes = {
        {"float", sizeof(float)},
        {"int32_t", sizeof(int32_t)},
        {"bool", sizeof(bool)},

        {"teardown::types::vector3_t", sizeof(teardown::types::vector3_t)},
        {"teardown::types::transform_t", sizeof(teardown::types::transform_t)},
        {"void*", sizeof(void*)},
    };

    // Helper function to insert nested member recursively
    std::function<void(structure_member&, const structure_order_element&, int32_t)> insertNestedMember =
        [&insertNestedMember](structure_member& parent, const structure_order_element& element, int32_t offset) {
        if (parent.nestedMembers.empty()) {
            parent.nestedMembers.push_back({ element.name, element.type, offset });
        } else {
            insertNestedMember(parent.nestedMembers.back(), element, offset);
        }
    };

    // First Pass: Generate Structures
    for (const auto& structure : structures) {
        assert(structure.members.size() == structure.offsets.size());

        size_t size = structure.members.size();
        structure_member nestedMember{};

        // Iterate over members
        for (size_t j = 0; j < size; j++) {
            const auto& orderElement = structure.members[j];
            int32_t offset = structure.offsets[j];

            if (j == 0) { // First main struct
                if (size == 2) {
                    nestedMember = { orderElement.name, orderElement.type, offset };
                }
            } else if (j < (size - 1)) { // Everything until last element
                if (nestedMember.name.empty()) {
                    nestedMember = { orderElement.name, orderElement.type, offset };
                }

                structureMap[structure.members[j - 1].name].nestedMembers.push_back({ orderElement.name, orderElement.type, offset });
            } else { // Last element
                insertNestedMember(nestedMember, orderElement, offset);

                structureMap[nestedMember.name].nestedMembers.push_back({ orderElement.name, orderElement.type, offset });
                nestedMember = {};
            }
        }
    }

    // Sort members by offset for each structure
    for (auto& [name, structure] : structureMap) {
        std::sort(structure.nestedMembers.begin(), structure.nestedMembers.end(),
                  [](const structure_member& a, const structure_member& b) {
                      return a.offset < b.offset;
                  });
    }

    // Helper function to get member size
    std::function<size_t(const structure_member&)> getMemberSize = [&](const structure_member& member) -> size_t {
        if (!member.type.empty() && member.type.back() == '*') {
            return sizeof(void*);
        } else if (typeSizes.find(member.type) != typeSizes.end()) {
            return typeSizes[member.type];
        } else {
            auto it = structureMap.find(member.type);
            if (it != structureMap.end() && !it->second.nestedMembers.empty()) {
                auto& lastMember = it->second.nestedMembers.back();
                return lastMember.offset + getMemberSize(lastMember);
            }

            return sizeof(void*);  // Default size for unknown types
        }
    };

    // Second Pass: Generate padding
    std::function<void(structure_member&)> generatePadding = [&](structure_member& structure) {
        int32_t currentOffset = 0;
        std::vector<structure_member> paddedMembers;

        for (const auto& member : structure.nestedMembers) {
            if (member.offset > currentOffset) {
                // Add padding
                int32_t paddingSize = member.offset - currentOffset;
                paddedMembers.push_back({ "padding_" + std::to_string(currentOffset) + "[" + std::to_string(paddingSize) + "]",
                                         "uint8_t",
                                         currentOffset });
            }

            paddedMembers.push_back(member);
            currentOffset = int32_t(member.offset + getMemberSize(member));
        }

        structure.nestedMembers = paddedMembers;
    };

    // Apply padding to all structures
    for (auto& [name, structure] : structureMap) {
        generatePadding(structure);
    }

    // Generate string output
    for (const auto& [name, structure] : structureMap) {
        ss << "struct " << name << " {\n";
        for (const auto& member : structure.nestedMembers) {
            ss << "    " << member.type << " " << member.name << "; // 0x" << std::hex << member.offset << "\n";
        }
        ss << "}; // Size: 0x" << std::hex << (structure.nestedMembers.back().offset +
                                               getMemberSize(structure.nestedMembers.back())) << "\n\n";
    }
}

static void generateLuaFunctions(std::vector<lua_func_category>& categories, std::stringstream& funcs, std::stringstream& assignFuncs, std::vector<game_structure>& structures) {
    console::writeln("Generating Lua Functions...");

    for (auto& cat : categories) {
        if (!mem::findGameFunctionOffsets(cat.funcs)) {
            util::displayError(L"Could not find Lua function offsets\n");
            return;
        }

        funcs << "        namespace " << cat.category << "{\n";

        for (const auto& fn : cat.funcs) {
            if (fn.address == 0x0) {
                console::writeln("FAILED FINDING LUA FUNCTION: {}", fn.name);
                continue;
            }

            funcs << std::format("                DECLARE_GAME_LUA_FUNCTION({0}, {1:#x});\n", fn.name, fn.address);
            assignFuncs << std::format("            UPDATE_GAME_LUA_FUNCTION({0}::{1});\n", cat.category, fn.name);

            if (fn.hintCallback.has_value()) {
                HINT_CALLBACK_TYPE hintCallback = fn.hintCallback.value();
                std::vector<structure_order_element> structureOrder;
                std::vector<int32_t> offsetOrder;

                hintCallback(mem::baseAddress + fn.address, structureOrder, offsetOrder);
                structures.push_back({ structureOrder, offsetOrder });
            }
        }

        funcs << "        }\n";
    }
}

// Public Functions
//------------------------------------------------------------------------
bool dumper::dump(bool genStructs) {
    std::vector<lua_func_category> categories = {
        { "parameters" , {
            { "GetIntParam" },
            { "GetFloatParam" },
            { "GetBoolParam" },
            { "GetStringParam" },
            { "GetColorParam" }
        } },

        { "scriptControl", {
            { "GetVersion" },
            { "HasVersion" },
            { "GetTime" },
            { "GetTimeStep" },
            { "InputLastPressedKey" },
            { "InputPressed" },
            { "InputReleased" },
            { "InputDown" },
            { "InputValue" },
            { "InputClear" },
            { "InputResetOnTransition" },
            { "LastInputDevice" },
            { "SetValue" },
            { "SetValueInTable" },
            { "PauseMenuButton" },
            { "HasFile" },
            { "StartLevel" },
            { "SetPaused" },
            { "Restart" },
            { "Menu" }
        } },

        { "entity", {
            { "FindEntity" },
            { "FindEntities" },
            { "GetEntityChildren" },
            { "GetEntityParent" },
            { "SetTag" },
            { "RemoveTag" },
            { "HasTag" },
            { "GetTagValue" },
            { "ListTags" },
            { "GetDescription" },
            { "SetDescription" },
            { "Delete" },
            { "IsHandleValid" },
            { "GetEntityType" },
            { "GetProperty" },
            { "SetProperty" }
        } },

        { "body", {
            { "FindBody" },
            { "FindBodies" },
            { "GetBodyTransform" }, // function_hints::body::getTransform
            { "SetBodyTransform" },
            { "GetBodyMass" },
            { "IsBodyDynamic" },
            { "SetBodyDynamic" },
            { "SetBodyVelocity" },
            { "GetBodyVelocity" },
            { "GetBodyVelocityAtPos" },
            { "SetBodyAngularVelocity" },
            { "GetBodyAngularVelocity" },
            { "IsBodyActive" },
            { "SetBodyActive" },
            { "ApplyBodyImpulse" },
            { "GetBodyShapes" },
            { "GetBodyVehicle" },
            { "GetBodyBounds" },
            { "GetBodyCenterOfMass" },
            { "IsBodyVisible" },
            { "IsBodyBroken" },
            { "IsBodyJointedToStatic" },
            { "DrawBodyOutline" },
            { "DrawBodyHighlight" },
            { "GetBodyClosestPoint" },
            { "ConstrainVelocity" },
            { "ConstrainAngularVelocity" },
            { "ConstrainPosition" },
            { "ConstrainOrientation" },
            { "GetWorldBody" }
        } },

        { "player", {
            { "GetPlayerPos", function_hints::player::getPos },
            { "GetPlayerTransform" },
            { "SetPlayerTransform" },
            { "SetPlayerGroundVelocity" },
            { "GetPlayerCameraTransform" },
            { "SetPlayerCameraOffsetTransform" },
            { "SetPlayerSpawnTransform" },
            { "SetPlayerSpawnHealth" },
            { "SetPlayerSpawnTool" },
            { "GetPlayerVelocity", function_hints::player::getVelocity },
            { "SetPlayerVehicle" },
            { "SetPlayerVelocity" },
            { "GetPlayerVehicle", function_hints::player::getVehicle },
            { "IsPlayerGrounded", function_hints::player::isGrounded },
            { "GetPlayerGrabShape", function_hints::player::getGrabShape },
            { "GetPlayerGrabBody", function_hints::player::getGrabBody },
            { "ReleasePlayerGrab" },
            { "GetPlayerPickShape" },
            { "GetPlayerPickBody" },
            { "GetPlayerInteractShape" },
            { "GetPlayerInteractBody" },
            { "SetPlayerScreen" },
            { "GetPlayerScreen" },
            { "SetPlayerHealth" },
            { "GetPlayerHealth", function_hints::player::getHealth },
            { "SetPlayerRegenerationState" },
            { "RespawnPlayer", function_hints::player::respawn },
            { "GetPlayerWalkingSpeed", function_hints::player::getWalkingSpeed },
            { "SetPlayerWalkingSpeed" },
            { "GetPlayerParam" },
            { "SetPlayerParam" },
            { "RegisterTool" },
            { "GetToolBody" },
            { "SetToolTransform" }
        } }
    };

    static bool generated = false;
    static std::vector<game_structure> structures;

    std::string result = "";

    static std::stringstream luaFuncs, assignLuaFuncs;

    if (!generated) {
        generateLuaFunctions(categories, luaFuncs, assignLuaFuncs, structures);
        generated = true;
    }

    if (genStructs) {
        console::writeln("Generating game structures...");

        std::stringstream ss;
        generateStructures(categories, ss, structures);

        result = ss.str();
    } else {
        console::writeln("Generating header...");

        std::stringstream offsets, checks, types, functions, assignFuncs;

        generateTemplate(signatureNamespaces, std::size(signatureNamespaces), offsets, checks, types, functions, assignFuncs);

        result = std::format(headerTemplate,
                             __DATE__,
                             __TIME__,
                             offsets.str(),
                             checks.str(),
                             types.str(),
                             functions.str(),
                             assignFuncs.str(),
                             luaFuncs.str(),
                             assignLuaFuncs.str());
    }

    console::writeln("\n{}", result);
    util::copyToClipboard(result, genStructs ? L"Copied structures to clipboard" : L"Copied offsets to clipboard");

    return true;
}