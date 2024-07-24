#include "pch.h"
#include "teardown/teardown.h"
#include "teardown/lua_helpers.h"
#include "teardown/types.h"
#include "shared/util/util.h"
#include "memory/hooks.h"
#include "offsets_generated.h"

using namespace mp;
using namespace teardown;

types::script_t* mpScript = nullptr;

// Forward Functions
//------------------------------------------------------------------------
lua_State* h_lua_newstate(lua_Alloc f, void* ud);
void h_script_core_registerLuaFunctions(td::script_core_t* scriptCore);
types::game_t* h_teardown_initialize(types::game_t* magicShit, DWORD** a2, int64_t a3);
void h_teardown_update(types::game_t* game, int64_t input);

// Public Functions
//------------------------------------------------------------------------
void teardown::initialize() {
    console::setStatus("Setting up hooks");

    mem::hooks::addHook("teardown::ctor", mp::offsets::teardown::initialize, &h_teardown_initialize, &funcs::teardown::initialize);
    mem::hooks::addHook("teardown::update", mp::offsets::teardown::update, &h_teardown_update, &funcs::teardown::update);
    mem::hooks::addHook("luaL_newstate", mp::offsets::lua::lua_newstate, &h_lua_newstate, &funcs::lua::lua_newstate);
    mem::hooks::addHook("script_core::registerLuaFunctions", mp::offsets::script_core::registerLuaFunctions, &h_script_core_registerLuaFunctions, &funcs::script_core::registerLuaFunctions);

    // TODO: Figure out how to set a custom tag for the logging function
    funcs::game::log(types::log_level::debug, "Debug");
    funcs::game::log(types::log_level::info, "Info");
    funcs::game::log(types::log_level::warning, "Warning");
    funcs::game::log(types::log_level::error, "Error");
}

// Hooked Functions
//------------------------------------------------------------------------
lua_State* h_lua_newstate(lua_Alloc f, void* ud) {
    lua_State* L = funcs::lua::lua_newstate(f, ud);
    //console::writeln("Lua State: 0x{:X}", (uintptr_t)L);

    return L;
}

void h_script_core_registerLuaFunctions(td::script_core_t* scriptCore) {
    console::writeln("Registering functions for {}", scriptCore->path.c_str());

    lua_helpers::registerLuaFunction(scriptCore, td::td_string("MP_GetVersion"), [](td::script_core_t* scriptCore, lua_State* L) -> int {
        lua_pushstring(L, "not-set");
        return 1;
    });

    funcs::script_core::registerLuaFunctions(scriptCore);
}

teardown::types::game_t* h_teardown_initialize(teardown::types::game_t* magicShit, DWORD** a2, int64_t a3) {
    teardown::game = funcs::teardown::initialize(magicShit, a2, a3);
    console::writeln("game initialized");

    if (SteamAPI_Init()) {
        ISteamFriends* friends = SteamFriends();
        int32_t friendCount = friends->GetFriendCount(k_EFriendFlagImmediate);

        for (int32_t i = 0; i < friendCount; ++i) {
            CSteamID id = friends->GetFriendByIndex(i, k_EFriendFlagImmediate);
            std::string_view name = friends->GetFriendPersonaName(id);

            console::writeln("Friend {} is {}", i, name);
        }
    } else {
        console::writeln("Failed to initialize Steam!");
    }

    return teardown::game;
}

void h_teardown_update(types::game_t* game, int64_t input) {
    static types::game_state state = types::game_state::none;

    funcs::teardown::update(teardown::game, input);

    types::player_t* player = (types::player_t*)(teardown::game->player);
    types::scene_t*  scene = (types::scene_t*)(teardown::game->scene);

    if (state != game->state) {
        state = game->state;

        td::td_string stateSwitch = "Switched state to: ";
        switch (state) {
            case types::game_state::splash:       stateSwitch += "splash"; break;
            case types::game_state::menu:         stateSwitch += "menu"; break;
            case types::game_state::ui:           stateSwitch += "ui"; break;
            case types::game_state::loading:      stateSwitch += "loading"; break;
            case types::game_state::menu_loading: stateSwitch += "menu_loading"; break;
            case types::game_state::play:         stateSwitch += "play"; break;
            case types::game_state::edit:         stateSwitch += "edit"; break;
            case types::game_state::quit:         stateSwitch += "quit"; break;
        }

        console::writeln(stateSwitch);
    }

    if (scene) {
        scene->firesystem->fires.reset();
        scene->projectiles.reset();
    }

    player->health = 1;

    if (GetAsyncKeyState(VK_F1) & 0x8000) {
        //console::writeln("TEST: {}", *((float*)((char*)game->qwordB8 + 0x15c)));

        console::writeln("Health: {}\nSpeed: {}", player->health, player->walkingSpeed);
        console::writeln("Grabbed body & shape: {} {}",
                         (player->grabbedBody != nullptr ? player->grabbedBody->handle : 0),
                         (player->grabbedShape != nullptr ? player->grabbedShape->handle : 0)
        );

        console::writeln("Transform: ({} {} {}) ({} {} {} {})", player->pos.x, player->pos.y, player->pos.z,
                         player->rot.x,
                         player->rot.y,
                         player->rot.z,
                         player->rot.w
        );

        console::writeln("Velocity: {} {} {}", player->velocity.x, player->velocity.y, player->velocity.z);
        console::writeln("Grounded: {}", player->isGrounded != 0);
    }
}