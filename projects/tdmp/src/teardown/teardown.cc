#include "pch.h"
#include "teardown/teardown.h"
#include "teardown/types.h"
#include "shared/util/util.h"
#include "memory/hooks.h"
#include "memory/dumper.h"
#include "offsets_generated.h"

using namespace tdmp;

// Hooked Functions
//------------------------------------------------------------------------
lua_State* h_lua_newstate(lua_Alloc f, void* ud) {
    lua_State* L = funcs::lua::lua_newstate(f, ud);
    console::writeln("Lua State: 0x{:X}", (uintptr_t)L);

    return L;
}

structures::teardown* h_teardown_initialize(structures::teardown* magicShit, DWORD** a2, int64_t a3) {
    structures::teardown* result = funcs::teardown::initialize(magicShit, a2, a3);
    console::writeln("Teardown Initialize?");

    tdmp::structures::small_string ss;
    funcs::small_string::fromCString(&ss, "options.audio.menumusic");

    int num = funcs::registry::getInt(result->RegistryThingyMbob, (uint8_t**)&ss);

    return result;
}

/*
TODO: Rename `func.address` to `func.offset` and use it only in the dumper
The dumper will need to be modified in order to take runtime things, right now it just takes constexpr stuff
But we will make it a vector and add these to it

With these functions, I should be able to automatically find the offsets to specific members in the structures
GetPlayerHealth for example:

```
sub_7FF7FAF03F60          sub_7FF7FAF03F60 proc near              ; DATA XREF: .data:off_7FF7FB609978↓o
sub_7FF7FAF03F60      000                 mov     rax, cs:globalMagicShit <-- Teardown's main big structure
sub_7FF7FAF03F60+7    000                 mov     rcx, [rax+0B8h] <-- Player strucure
sub_7FF7FAF03F60+E    000                 movss   xmm1, dword ptr [rcx+1B0h] <-- Health offset in player structure
sub_7FF7FAF03F60+16   000                 mov     rcx, r8
sub_7FF7FAF03F60+19   000                 jmp     Lua__pushnumber
sub_7FF7FAF03F60+19       sub_7FF7FAF03F60 endp
```

So I can just give it the function address and give it some sort of hint as to where the offset will be
This should pretty much future-proof it for updates (unless the C Lua function itself changes)

With this, I won't need to reverse engineer all the structures either, so I can save a lot of time.
*/

// Public Functions
//------------------------------------------------------------------------
void teardown::initialize() {
    std::vector<mem::game_function*> functions = {
        &game_lua_funcs::player::GetPlayerHealth,
        &game_lua_funcs::player::GetPlayerPos,
        &game_lua_funcs::player::GetPlayerTransform,
        &game_lua_funcs::player::SetPlayerTransform,
        &game_lua_funcs::player::SetPlayerZoom,
        &game_lua_funcs::player::SetPlayerGroundVelocity,
        &game_lua_funcs::player::GetPlayerCameraTransform,
        &game_lua_funcs::player::SetPlayerCameraOffsetTransform,
        &game_lua_funcs::player::SetPlayerSpawnTransform,
        &game_lua_funcs::player::SetPlayerSpawnHealth,
        &game_lua_funcs::player::SetPlayerSpawnTool,
        &game_lua_funcs::player::GetPlayerVelocity,
        &game_lua_funcs::player::SetPlayerVehicle,
        &game_lua_funcs::player::SetPlayerVelocity,
        &game_lua_funcs::player::GetPlayerVehicle,
        &game_lua_funcs::player::IsPlayerGrounded,
        &game_lua_funcs::player::GetPlayerGrabShape,
        &game_lua_funcs::player::GetPlayerGrabBody,
        &game_lua_funcs::player::ReleasePlayerGrab,
        &game_lua_funcs::player::GetPlayerPickShape,
        &game_lua_funcs::player::GetPlayerPickBody,
        &game_lua_funcs::player::GetPlayerInteractShape,
        &game_lua_funcs::player::GetPlayerInteractBody,
        &game_lua_funcs::player::SetPlayerScreen,
        &game_lua_funcs::player::GetPlayerScreen,
        &game_lua_funcs::player::SetPlayerHealth,
        &game_lua_funcs::player::GetPlayerHealth,
        &game_lua_funcs::player::SetPlayerRegenerationState,
        &game_lua_funcs::player::RespawnPlayer,
        &game_lua_funcs::player::GetPlayerWalkingSpeed,
        &game_lua_funcs::player::SetPlayerWalkingSpeed,
        &game_lua_funcs::player::GetPlayerParam,
        &game_lua_funcs::player::SetPlayerParam,
        &game_lua_funcs::player::RegisterTool,
        &game_lua_funcs::player::GetToolBody,
        &game_lua_funcs::player::SetToolTransform
    };

    if (!mem::findGameFunctions(functions)) {
        return;
    }

    for (const auto& func : functions) {
        if (func->address != 0) {
            console::writeln("Function '{}' found at address: {:x}", func->name, func->address);
        } else {
            console::writeln("Function '{}' not found", func->name);
        }
    }

    console::setStatus("Setting up hooks");

    mem::hooks::addHook("Teardown::Initialize", tdmp::offsets::teardown::initialize, &h_teardown_initialize, &funcs::teardown::initialize);
    mem::hooks::addHook("luaL_newstate", tdmp::offsets::lua::lua_newstate, &h_lua_newstate, &funcs::lua::lua_newstate);

    // TODO: Figure out how to set a custom tag for the logging function
    funcs::game::log(types::log_level::debug, "Debug");
    funcs::game::log(types::log_level::info, "Info");
    funcs::game::log(types::log_level::warning, "Warning");
    funcs::game::log(types::log_level::error, "Error");
}

bool mem::isAddressExecutable(uintptr_t address) {
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery((void*)address, &mbi, sizeof(mbi))) {
        return (mbi.Protect & PAGE_EXECUTE_READ) ||
            (mbi.Protect & PAGE_EXECUTE_READWRITE) ||
            (mbi.Protect & PAGE_EXECUTE_WRITECOPY);
    }
    return false;
}

void teardown::earlyEntryThread() {
    // Parse Arguments
    //------------------------------------------------------------------------
    argparse::ArgumentParser args("TDMP");
    args.add_argument("-dump").default_value(false).implicit_value(true);

    {
        int argc;
        char** argv = util::commandLineToArgvA(GetCommandLineA(), &argc);

        try {
            args.parse_known_args(argc, argv);
        } catch (const std::exception& e) {
            std::wstring err = util::s2ws(e.what());
            util::displayError(L"Failed parsing arguments: {}", err);
            return;
        }
    }

    if (!mem::initializeMemory()) {
        util::displayLastError(L"Failed to initialize memory");
        ExitProcess(1);
        return;
    }

    // Wait for data section to load and update all the offsets with the base address
    mem::waitForSection(GetModuleHandle(NULL), ".data");

    if (args.get<bool>("-dump")) {
        dumper::dump();
        ExitProcess(0);
    }

    console::setStatus("Generating addresses");

    std::vector<std::string> badOffsets;
    if (!offsets::generate(badOffsets)) {
        std::stringstream ss;
        ss << "Invalid offsets: " << badOffsets.size() << "\n";

        for (const std::string& offset : badOffsets) {
            ss << "  " << offset << '\n';
        }

        std::wstring ssStr = util::s2ws(ss.str());

        console::writeln(L"\n{}", ssStr);
        util::displayError(ssStr);

        ExitProcess(1);
    }

    // Assign all the functions to their addresses
    funcs::assign();

    if (MH_STATUS status = MH_Initialize(); status != MH_OK) {
        std::wstring err = util::s2ws(MH_StatusToString(status));
        util::displayError(L"Failed initializing minhook: {}", err);
        return;
    }

    teardown::initialize();

    console::setStatus("");
}