#include "pch.h"
#include "teardown/teardown.h"
#include "teardown/lua_helpers.h"
#include "teardown/types.h"
#include "shared/util/util.h"
#include "memory/hooks.h"
#include "memory/dumper.h"
#include "offsets_generated.h"

using namespace mp;
using namespace teardown;

static teardown::types::game* game;

// Hooked Functions
//------------------------------------------------------------------------
lua_State* h_lua_newstate(lua_Alloc f, void* ud) {
    lua_State* L = funcs::lua::lua_newstate(f, ud);
    //console::writeln("Lua State: 0x{:X}", (uintptr_t)L);

    return L;
}

teardown::types::game* h_teardown_initialize(teardown::types::game* magicShit, DWORD** a2, int64_t a3) {
    game = funcs::teardown::initialize(magicShit, a2, a3);

    console::writeln("Teardown Initialize?");

    teardown::types::td_string str("options.audio.menumusic");
    //funcs::small_string::fromCString(&ss, "options.audio.menumusic");

    int num = funcs::registry::getInt(game->registry, &str);

    return game;
}

void h_teardown_update(types::game* game, int64_t input) {
    funcs::teardown::update(game, input);
    //game->splashProgress = 1.0f;

    types::player_t* ply = (types::player_t*)(game->player);
    types::scene_t* scene = (types::scene_t*)(game->scene);
    types::something* something = (types::something*)(game);

    if (scene) {
        scene->firesystem->fires.reset();
        scene->projectiles.reset();
    }

    ply->health = 1;

    if (GetAsyncKeyState(VK_F1) & 0x8000) {
        //console::writeln("TEST: {}", *((float*)((char*)game->qwordB8 + 0x15c)));

        console::writeln("Health: {}\nSpeed: {}", ply->health, ply->walkingSpeed);
        console::writeln("Grabbed body & shape: {} {}",
                         (ply->grabbedBody != nullptr ? ply->grabbedBody->handle : 0),
                         (ply->grabbedShape != nullptr ? ply->grabbedShape->handle : 0)
        );

        console::writeln("Transform: ({} {} {}) ({} {} {} {})", ply->pos.x, ply->pos.y, ply->pos.z,
                         0,
                         0,
                         0,
                         0
        );
        console::writeln("Velocity: {} {} {}", ply->velocity.x, ply->velocity.y, ply->velocity.z);
        console::writeln("Grounded: {}", ply->isGrounded != 0);

        //if (scene != nullptr) {
        //    console::writeln("Vehicle: {}", scene->activeVehicle != nullptr ? scene->activeVehicle->handle : 0);
        //}
        //ply->walkingSpeed = 15;
    } else if (GetAsyncKeyState(VK_F2) & 0x8000) {
        something->respawnFlag = true;
        //ply->walkingSpeed = 7;
    } else if (GetAsyncKeyState(VK_F3) & 0x8000) {
        for (uint32_t i = 0; i < scene->bodies.size(); ++i) {
            types::body* body = scene->bodies[i];
            console::writeln("{}", body->handle);
        }
    }
}

typedef int (*tLuaFunction)(const types::script_core_t* pSC, lua_State* L, const char* cFunctionName);

void h_script_core_registerLuaFunctions(mp::teardown::types::script_core_t* scriptCore) {
    console::writeln("Registering functions for {}", scriptCore->path.c_str());

    lua_helpers::registerLuaFunction(scriptCore, types::td_string("MP_GetVersion"), [](types::script_core_t* scriptCore, lua_State* L) -> int {
        lua_pushstring(L, "not-set");
        return 1;
    });

    funcs::script_core::registerLuaFunctions(scriptCore);
}

// Public Functions
//------------------------------------------------------------------------
void teardown::initialize() {
    console::setStatus("Setting up hooks");

    mem::hooks::addHook("game::ctor", mp::offsets::teardown::initialize, &h_teardown_initialize, &funcs::teardown::initialize);
    mem::hooks::addHook("game::update", mp::offsets::teardown::update, &h_teardown_update, &funcs::teardown::update);
    mem::hooks::addHook("luaL_newstate", mp::offsets::lua::lua_newstate, &h_lua_newstate, &funcs::lua::lua_newstate);
    mem::hooks::addHook("script_core::registerLuaFunctions", mp::offsets::script_core::registerLuaFunctions, &h_script_core_registerLuaFunctions, &funcs::script_core::registerLuaFunctions);

    // TODO: Figure out how to set a custom tag for the logging function
    funcs::game::log(types::log_level::debug, "Debug");
    funcs::game::log(types::log_level::info, "Info");
    funcs::game::log(types::log_level::warning, "Warning");
    funcs::game::log(types::log_level::error, "Error");
}

void teardown::earlyEntryThread() {
    // Parse Arguments
    //------------------------------------------------------------------------
    argparse::ArgumentParser args("TDMP");
    args.add_argument("-dump").default_value(false).implicit_value(true);
    args.add_argument("-generate-structs").default_value(false).implicit_value(true);

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

    {
        bool wasDumped = false;

        if (args.get<bool>("-dump")) {
            dumper::dump(false);
            wasDumped = true;
        }

        if (args.get<bool>("-generate-structs")) {
            dumper::dump(true);
            wasDumped = true;
        }

        if (wasDumped) {
            ExitProcess(0);
        }
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
    lua_funcs::assign();

    if (MH_STATUS status = MH_Initialize(); status != MH_OK) {
        std::wstring err = util::s2ws(MH_StatusToString(status));
        util::displayError(L"Failed initializing minhook: {}", err);
        return;
    }

    teardown::initialize();
    console::setStatus("");
}