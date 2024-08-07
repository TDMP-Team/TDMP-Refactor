#ifndef TDMP_GENERATED_OFFSETS_H
#define TDMP_GENERATED_OFFSETS_H

// Generated by dumper.cc
// Date: Jul 24 2024
// Time: 20:06:50

#include "offsets_generated_prerequisite.h"

namespace mp {
    namespace offsets {
        namespace lua {
            inline uint64_t lua_newstate = 0x580280;
        }

        namespace script_core {
            inline uint64_t ctor = 0x3dcb60;
            inline uint64_t registerLuaFunctions = 0x409290;
            inline uint64_t loadScript = 0x407bb0;
            inline uint64_t destroy = 0x35cd0;
        }

        namespace script {
            inline uint64_t ctor = 0x3db900;
        }

        namespace game {
            inline uint64_t log = 0x5e7cc0;
        }

        namespace teardown {
            inline uint64_t initialize = 0x5f950;
            inline uint64_t update = 0x657b0;
        }

        namespace registry {
            inline uint64_t getInt = 0x3d2750;
            inline uint64_t getFloat = 0x3d2700;
            inline uint64_t getString = 0x3d2a60;
            inline uint64_t setBool = 0x3d3d00;
            inline uint64_t setString = 0x3d3e60;
        }

        namespace renderer {
            inline uint64_t createDXGIFactory1 = 0x60d005;
            inline uint64_t initializeForAPI = 0x5337a0;
        }

        inline bool generate(std::vector<std::string>& failedOffsets) {
            console::writeln("Function Addresses:");

            SET_AND_CHECK_OFFSET(lua::lua_newstate, "lua::lua_newstate");
            SET_AND_CHECK_OFFSET(script_core::ctor, "script_core::ctor");
            SET_AND_CHECK_OFFSET(script_core::registerLuaFunctions, "script_core::registerLuaFunctions");
            SET_AND_CHECK_OFFSET(script_core::loadScript, "script_core::loadScript");
            SET_AND_CHECK_OFFSET(script_core::destroy, "script_core::destroy");
            SET_AND_CHECK_OFFSET(script::ctor, "script::ctor");
            SET_AND_CHECK_OFFSET(game::log, "game::log");
            SET_AND_CHECK_OFFSET(teardown::initialize, "teardown::initialize");
            SET_AND_CHECK_OFFSET(teardown::update, "teardown::update");
            SET_AND_CHECK_OFFSET(registry::getInt, "registry::getInt");
            SET_AND_CHECK_OFFSET(registry::getFloat, "registry::getFloat");
            SET_AND_CHECK_OFFSET(registry::getString, "registry::getString");
            SET_AND_CHECK_OFFSET(registry::setBool, "registry::setBool");
            SET_AND_CHECK_OFFSET(registry::setString, "registry::setString");
            SET_AND_CHECK_OFFSET(renderer::createDXGIFactory1, "renderer::createDXGIFactory1");
            SET_AND_CHECK_OFFSET(renderer::initializeForAPI, "renderer::initializeForAPI");

            return failedOffsets.size() == 0;
        }
    }

    namespace funcs {
        namespace types {
            namespace lua {
                using tlua_newstate = std::add_pointer_t<lua_State*(lua_Alloc f, void* ud)>;
            }

            namespace script_core {
                using tctor = std::add_pointer_t<td::script_core_t*(td::script_core_t* scriptCore)>;
                using tregisterLuaFunctions = std::add_pointer_t<void(td::script_core_t* scriptCore)>;
                using tloadScript = std::add_pointer_t<void(td::script_core_t* scriptCore, td::td_string name)>;
                using tdestroy = std::add_pointer_t<void(td::script_core_t* scriptCore, char a2)>;
            }

            namespace script {
                using tctor = std::add_pointer_t<mp::teardown::types::script_t*(mp::teardown::types::script_t* script, bool entityThing)>;
            }

            namespace game {
                using tlog = std::add_pointer_t<void(mp::teardown::types::log_level level, const char* fmt, ...)>;
            }

            namespace teardown {
                using tinitialize = std::add_pointer_t<mp::teardown::types::game_t*(mp::teardown::types::game_t* game, DWORD** a2, int64_t a3)>;
                using tupdate = std::add_pointer_t<mp::teardown::types::game_t*(mp::teardown::types::game_t* game, int64_t input)>;
            }

            namespace registry {
                using tgetInt = std::add_pointer_t<int(uint8_t** registry, td::td_string str)>;
                using tgetFloat = std::add_pointer_t<float(uint8_t** registry, td::td_string str)>;
                using tgetString = std::add_pointer_t<td::td_string(uint8_t** registry, td::td_string str)>;
                using tsetBool = std::add_pointer_t<void(uint8_t** a1, td::td_string str, bool value)>;
                using tsetString = std::add_pointer_t<void(uint8_t** a1, td::td_string str, td::td_string value)>;
            }

            namespace renderer {
                using tcreateDXGIFactory1 = std::add_pointer_t<HRESULT __stdcall(const IID *const riid, void **ppFactory)>;
                using tinitializeForAPI = std::add_pointer_t<DWORD* __fastcall(DWORD* a1, int *a2)>;
            }

        }

        namespace lua {
            inline types::lua::tlua_newstate lua_newstate = nullptr;
        }

        namespace script_core {
            inline types::script_core::tctor ctor = nullptr;
            inline types::script_core::tregisterLuaFunctions registerLuaFunctions = nullptr;
            inline types::script_core::tloadScript loadScript = nullptr;
            inline types::script_core::tdestroy destroy = nullptr;
        }

        namespace script {
            inline types::script::tctor ctor = nullptr;
        }

        namespace game {
            inline types::game::tlog log = nullptr;
        }

        namespace teardown {
            inline types::teardown::tinitialize initialize = nullptr;
            inline types::teardown::tupdate update = nullptr;
        }

        namespace registry {
            inline types::registry::tgetInt getInt = nullptr;
            inline types::registry::tgetFloat getFloat = nullptr;
            inline types::registry::tgetString getString = nullptr;
            inline types::registry::tsetBool setBool = nullptr;
            inline types::registry::tsetString setString = nullptr;
        }

        namespace renderer {
            inline types::renderer::tcreateDXGIFactory1 createDXGIFactory1 = nullptr;
            inline types::renderer::tinitializeForAPI initializeForAPI = nullptr;
        }


        inline bool assign() {
            console::writeln("Assigning functions");

            ASSIGN_FUNCTION(lua, lua_newstate);
            ASSIGN_FUNCTION(script_core, ctor);
            ASSIGN_FUNCTION(script_core, registerLuaFunctions);
            ASSIGN_FUNCTION(script_core, loadScript);
            ASSIGN_FUNCTION(script_core, destroy);
            ASSIGN_FUNCTION(script, ctor);
            ASSIGN_FUNCTION(game, log);
            ASSIGN_FUNCTION(teardown, initialize);
            ASSIGN_FUNCTION(teardown, update);
            ASSIGN_FUNCTION(registry, getInt);
            ASSIGN_FUNCTION(registry, getFloat);
            ASSIGN_FUNCTION(registry, getString);
            ASSIGN_FUNCTION(registry, setBool);
            ASSIGN_FUNCTION(registry, setString);
            ASSIGN_FUNCTION(renderer, createDXGIFactory1);
            ASSIGN_FUNCTION(renderer, initializeForAPI);

            return true;
        }
    }

    namespace lua_funcs {
        namespace parameters{
                DECLARE_GAME_LUA_FUNCTION(GetIntParam, 0x40de70);
                DECLARE_GAME_LUA_FUNCTION(GetFloatParam, 0x40dd10);
                DECLARE_GAME_LUA_FUNCTION(GetBoolParam, 0x40d6d0);
                DECLARE_GAME_LUA_FUNCTION(GetStringParam, 0x40dfe0);
                DECLARE_GAME_LUA_FUNCTION(GetColorParam, 0x40d8d0);
        }
        namespace scriptControl{
                DECLARE_GAME_LUA_FUNCTION(GetVersion, 0x40e280);
                DECLARE_GAME_LUA_FUNCTION(HasVersion, 0x40e4f0);
                DECLARE_GAME_LUA_FUNCTION(GetTime, 0x40e090);
                DECLARE_GAME_LUA_FUNCTION(GetTimeStep, 0x40e0a0);
                DECLARE_GAME_LUA_FUNCTION(InputLastPressedKey, 0x40e880);
                DECLARE_GAME_LUA_FUNCTION(InputPressed, 0x40e920);
                DECLARE_GAME_LUA_FUNCTION(InputReleased, 0x40ead0);
                DECLARE_GAME_LUA_FUNCTION(InputDown, 0x40e6d0);
                DECLARE_GAME_LUA_FUNCTION(InputValue, 0x40ecc0);
                DECLARE_GAME_LUA_FUNCTION(InputClear, 0x40e6c0);
                DECLARE_GAME_LUA_FUNCTION(InputResetOnTransition, 0x40ec80);
                DECLARE_GAME_LUA_FUNCTION(LastInputDevice, 0x40f550);
                DECLARE_GAME_LUA_FUNCTION(SetValue, 0x410380);
                DECLARE_GAME_LUA_FUNCTION(SetValueInTable, 0x410510);
                DECLARE_GAME_LUA_FUNCTION(PauseMenuButton, 0x40f720);
                DECLARE_GAME_LUA_FUNCTION(HasFile, 0x40e290);
                DECLARE_GAME_LUA_FUNCTION(StartLevel, 0x4107b0);
                DECLARE_GAME_LUA_FUNCTION(SetPaused, 0x410060);
                DECLARE_GAME_LUA_FUNCTION(Restart, 0x40fa20);
                DECLARE_GAME_LUA_FUNCTION(Menu, 0x7ff794466d30);
        }
        namespace entity{
                DECLARE_GAME_LUA_FUNCTION(FindEntity, 0x3e40a0);
                DECLARE_GAME_LUA_FUNCTION(FindEntities, 0x3e3f50);
                DECLARE_GAME_LUA_FUNCTION(GetEntityChildren, 0x3e6540);
                DECLARE_GAME_LUA_FUNCTION(GetEntityParent, 0x3e67b0);
                DECLARE_GAME_LUA_FUNCTION(SetTag, 0x3fd0c0);
                DECLARE_GAME_LUA_FUNCTION(RemoveTag, 0x7ff79449eb1d);
                DECLARE_GAME_LUA_FUNCTION(HasTag, 0x7ff7944a0b0c);
                DECLARE_GAME_LUA_FUNCTION(GetTagValue, 0x3ebb50);
                DECLARE_GAME_LUA_FUNCTION(ListTags, 0x3ef610);
                DECLARE_GAME_LUA_FUNCTION(GetDescription, 0x3e63b0);
                DECLARE_GAME_LUA_FUNCTION(SetDescription, 0x3f8660);
                DECLARE_GAME_LUA_FUNCTION(Delete, 0x3e0ad0);
                DECLARE_GAME_LUA_FUNCTION(IsHandleValid, 0x3edcc0);
                DECLARE_GAME_LUA_FUNCTION(GetEntityType, 0x3e6880);
                DECLARE_GAME_LUA_FUNCTION(GetProperty, 0x3e9810);
                DECLARE_GAME_LUA_FUNCTION(SetProperty, 0x3fb740);
        }
        namespace body{
                DECLARE_GAME_LUA_FUNCTION(FindBody, 0x3e3f40);
                DECLARE_GAME_LUA_FUNCTION(FindBodies, 0x3e3f30);
                DECLARE_GAME_LUA_FUNCTION(GetBodyTransform, 0x3e59e0);
                DECLARE_GAME_LUA_FUNCTION(SetBodyTransform, 0x3f7af0);
                DECLARE_GAME_LUA_FUNCTION(GetBodyMass, 0x3e5870);
                DECLARE_GAME_LUA_FUNCTION(IsBodyDynamic, 0x3ed780);
                DECLARE_GAME_LUA_FUNCTION(SetBodyDynamic, 0x3f7a50);
                DECLARE_GAME_LUA_FUNCTION(SetBodyVelocity, 0x3f7cf0);
                DECLARE_GAME_LUA_FUNCTION(GetBodyVelocity, 0x3e5ba0);
                DECLARE_GAME_LUA_FUNCTION(GetBodyVelocityAtPos, 0x3e5c40);
                DECLARE_GAME_LUA_FUNCTION(SetBodyAngularVelocity, 0x3f7980);
                DECLARE_GAME_LUA_FUNCTION(GetBodyAngularVelocity, 0x3e52b0);
                DECLARE_GAME_LUA_FUNCTION(IsBodyActive, 0x3ed620);
                DECLARE_GAME_LUA_FUNCTION(SetBodyActive, 0x3f78c0);
                DECLARE_GAME_LUA_FUNCTION(ApplyBodyImpulse, 0x3dedf0);
                DECLARE_GAME_LUA_FUNCTION(GetBodyShapes, 0x3e58f0);
                DECLARE_GAME_LUA_FUNCTION(GetBodyVehicle, 0x3e5b20);
                DECLARE_GAME_LUA_FUNCTION(GetBodyBounds, 0x3e5350);
                DECLARE_GAME_LUA_FUNCTION(GetBodyCenterOfMass, 0x3e5440);
                DECLARE_GAME_LUA_FUNCTION(IsBodyVisible, 0x3eda00);
                DECLARE_GAME_LUA_FUNCTION(IsBodyBroken, 0x3ed6a0);
                DECLARE_GAME_LUA_FUNCTION(IsBodyJointedToStatic, 0x3ed990);
                DECLARE_GAME_LUA_FUNCTION(DrawBodyOutline, 0x3e11e0);
                DECLARE_GAME_LUA_FUNCTION(DrawBodyHighlight, 0x3e10e0);
                DECLARE_GAME_LUA_FUNCTION(GetBodyClosestPoint, 0x3e54e0);
                DECLARE_GAME_LUA_FUNCTION(ConstrainVelocity, 0x3df750);
                DECLARE_GAME_LUA_FUNCTION(ConstrainAngularVelocity, 0x3df1a0);
                DECLARE_GAME_LUA_FUNCTION(ConstrainPosition, 0x3df5d0);
                DECLARE_GAME_LUA_FUNCTION(ConstrainOrientation, 0x3df460);
                DECLARE_GAME_LUA_FUNCTION(GetWorldBody, 0x3ed4d0);
        }
        namespace player{
                DECLARE_GAME_LUA_FUNCTION(GetPlayerPos, 0x3e8d80);
                DECLARE_GAME_LUA_FUNCTION(GetPlayerTransform, 0x3e9200);
                DECLARE_GAME_LUA_FUNCTION(SetPlayerTransform, 0x3fb0b0);
                DECLARE_GAME_LUA_FUNCTION(SetPlayerGroundVelocity, 0x3fa6a0);
                DECLARE_GAME_LUA_FUNCTION(GetPlayerCameraTransform, 0x3e8640);
                DECLARE_GAME_LUA_FUNCTION(SetPlayerCameraOffsetTransform, 0x3fa480);
                DECLARE_GAME_LUA_FUNCTION(SetPlayerSpawnTransform, 0x3faf20);
                DECLARE_GAME_LUA_FUNCTION(SetPlayerSpawnHealth, 0x3fae90);
                DECLARE_GAME_LUA_FUNCTION(SetPlayerSpawnTool, 0x3faed0);
                DECLARE_GAME_LUA_FUNCTION(GetPlayerVelocity, 0x3e94e0);
                DECLARE_GAME_LUA_FUNCTION(SetPlayerVehicle, 0x3fb240);
                DECLARE_GAME_LUA_FUNCTION(SetPlayerVelocity, 0x3fb2c0);
                DECLARE_GAME_LUA_FUNCTION(GetPlayerVehicle, 0x3e94b0);
                DECLARE_GAME_LUA_FUNCTION(IsPlayerGrounded, 0x3ede20);
                DECLARE_GAME_LUA_FUNCTION(GetPlayerGrabShape, 0x3e8990);
                DECLARE_GAME_LUA_FUNCTION(GetPlayerGrabBody, 0x3e8840);
                DECLARE_GAME_LUA_FUNCTION(ReleasePlayerGrab, 0x3f3c30);
                DECLARE_GAME_LUA_FUNCTION(GetPlayerPickShape, 0x3e8d20);
                DECLARE_GAME_LUA_FUNCTION(GetPlayerPickBody, 0x3e8ce0);
                DECLARE_GAME_LUA_FUNCTION(GetPlayerInteractShape, 0x3e8a30);
                DECLARE_GAME_LUA_FUNCTION(GetPlayerInteractBody, 0x3e89e0);
                DECLARE_GAME_LUA_FUNCTION(SetPlayerScreen, 0x3fae00);
                DECLARE_GAME_LUA_FUNCTION(GetPlayerScreen, 0x3e9150);
                DECLARE_GAME_LUA_FUNCTION(SetPlayerHealth, 0x3fa700);
                DECLARE_GAME_LUA_FUNCTION(GetPlayerHealth, 0x3e89c0);
                DECLARE_GAME_LUA_FUNCTION(SetPlayerRegenerationState, 0x3faa20);
                DECLARE_GAME_LUA_FUNCTION(RespawnPlayer, 0x3f4070);
                DECLARE_GAME_LUA_FUNCTION(GetPlayerWalkingSpeed, 0x3e9510);
                DECLARE_GAME_LUA_FUNCTION(SetPlayerWalkingSpeed, 0x3fb320);
                DECLARE_GAME_LUA_FUNCTION(GetPlayerParam, 0x3e8a70);
                DECLARE_GAME_LUA_FUNCTION(SetPlayerParam, 0x3fa760);
                DECLARE_GAME_LUA_FUNCTION(RegisterTool, 0x3f39e0);
                DECLARE_GAME_LUA_FUNCTION(GetToolBody, 0x3ebc10);
                DECLARE_GAME_LUA_FUNCTION(SetToolTransform, 0x3fd940);
        }


        inline bool assign() {
            UPDATE_GAME_LUA_FUNCTION(parameters::GetIntParam);
            UPDATE_GAME_LUA_FUNCTION(parameters::GetFloatParam);
            UPDATE_GAME_LUA_FUNCTION(parameters::GetBoolParam);
            UPDATE_GAME_LUA_FUNCTION(parameters::GetStringParam);
            UPDATE_GAME_LUA_FUNCTION(parameters::GetColorParam);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::GetVersion);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::HasVersion);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::GetTime);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::GetTimeStep);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::InputLastPressedKey);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::InputPressed);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::InputReleased);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::InputDown);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::InputValue);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::InputClear);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::InputResetOnTransition);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::LastInputDevice);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::SetValue);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::SetValueInTable);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::PauseMenuButton);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::HasFile);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::StartLevel);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::SetPaused);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::Restart);
            UPDATE_GAME_LUA_FUNCTION(scriptControl::Menu);
            UPDATE_GAME_LUA_FUNCTION(entity::FindEntity);
            UPDATE_GAME_LUA_FUNCTION(entity::FindEntities);
            UPDATE_GAME_LUA_FUNCTION(entity::GetEntityChildren);
            UPDATE_GAME_LUA_FUNCTION(entity::GetEntityParent);
            UPDATE_GAME_LUA_FUNCTION(entity::SetTag);
            UPDATE_GAME_LUA_FUNCTION(entity::RemoveTag);
            UPDATE_GAME_LUA_FUNCTION(entity::HasTag);
            UPDATE_GAME_LUA_FUNCTION(entity::GetTagValue);
            UPDATE_GAME_LUA_FUNCTION(entity::ListTags);
            UPDATE_GAME_LUA_FUNCTION(entity::GetDescription);
            UPDATE_GAME_LUA_FUNCTION(entity::SetDescription);
            UPDATE_GAME_LUA_FUNCTION(entity::Delete);
            UPDATE_GAME_LUA_FUNCTION(entity::IsHandleValid);
            UPDATE_GAME_LUA_FUNCTION(entity::GetEntityType);
            UPDATE_GAME_LUA_FUNCTION(entity::GetProperty);
            UPDATE_GAME_LUA_FUNCTION(entity::SetProperty);
            UPDATE_GAME_LUA_FUNCTION(body::FindBody);
            UPDATE_GAME_LUA_FUNCTION(body::FindBodies);
            UPDATE_GAME_LUA_FUNCTION(body::GetBodyTransform);
            UPDATE_GAME_LUA_FUNCTION(body::SetBodyTransform);
            UPDATE_GAME_LUA_FUNCTION(body::GetBodyMass);
            UPDATE_GAME_LUA_FUNCTION(body::IsBodyDynamic);
            UPDATE_GAME_LUA_FUNCTION(body::SetBodyDynamic);
            UPDATE_GAME_LUA_FUNCTION(body::SetBodyVelocity);
            UPDATE_GAME_LUA_FUNCTION(body::GetBodyVelocity);
            UPDATE_GAME_LUA_FUNCTION(body::GetBodyVelocityAtPos);
            UPDATE_GAME_LUA_FUNCTION(body::SetBodyAngularVelocity);
            UPDATE_GAME_LUA_FUNCTION(body::GetBodyAngularVelocity);
            UPDATE_GAME_LUA_FUNCTION(body::IsBodyActive);
            UPDATE_GAME_LUA_FUNCTION(body::SetBodyActive);
            UPDATE_GAME_LUA_FUNCTION(body::ApplyBodyImpulse);
            UPDATE_GAME_LUA_FUNCTION(body::GetBodyShapes);
            UPDATE_GAME_LUA_FUNCTION(body::GetBodyVehicle);
            UPDATE_GAME_LUA_FUNCTION(body::GetBodyBounds);
            UPDATE_GAME_LUA_FUNCTION(body::GetBodyCenterOfMass);
            UPDATE_GAME_LUA_FUNCTION(body::IsBodyVisible);
            UPDATE_GAME_LUA_FUNCTION(body::IsBodyBroken);
            UPDATE_GAME_LUA_FUNCTION(body::IsBodyJointedToStatic);
            UPDATE_GAME_LUA_FUNCTION(body::DrawBodyOutline);
            UPDATE_GAME_LUA_FUNCTION(body::DrawBodyHighlight);
            UPDATE_GAME_LUA_FUNCTION(body::GetBodyClosestPoint);
            UPDATE_GAME_LUA_FUNCTION(body::ConstrainVelocity);
            UPDATE_GAME_LUA_FUNCTION(body::ConstrainAngularVelocity);
            UPDATE_GAME_LUA_FUNCTION(body::ConstrainPosition);
            UPDATE_GAME_LUA_FUNCTION(body::ConstrainOrientation);
            UPDATE_GAME_LUA_FUNCTION(body::GetWorldBody);
            UPDATE_GAME_LUA_FUNCTION(player::GetPlayerPos);
            UPDATE_GAME_LUA_FUNCTION(player::GetPlayerTransform);
            UPDATE_GAME_LUA_FUNCTION(player::SetPlayerTransform);
            UPDATE_GAME_LUA_FUNCTION(player::SetPlayerGroundVelocity);
            UPDATE_GAME_LUA_FUNCTION(player::GetPlayerCameraTransform);
            UPDATE_GAME_LUA_FUNCTION(player::SetPlayerCameraOffsetTransform);
            UPDATE_GAME_LUA_FUNCTION(player::SetPlayerSpawnTransform);
            UPDATE_GAME_LUA_FUNCTION(player::SetPlayerSpawnHealth);
            UPDATE_GAME_LUA_FUNCTION(player::SetPlayerSpawnTool);
            UPDATE_GAME_LUA_FUNCTION(player::GetPlayerVelocity);
            UPDATE_GAME_LUA_FUNCTION(player::SetPlayerVehicle);
            UPDATE_GAME_LUA_FUNCTION(player::SetPlayerVelocity);
            UPDATE_GAME_LUA_FUNCTION(player::GetPlayerVehicle);
            UPDATE_GAME_LUA_FUNCTION(player::IsPlayerGrounded);
            UPDATE_GAME_LUA_FUNCTION(player::GetPlayerGrabShape);
            UPDATE_GAME_LUA_FUNCTION(player::GetPlayerGrabBody);
            UPDATE_GAME_LUA_FUNCTION(player::ReleasePlayerGrab);
            UPDATE_GAME_LUA_FUNCTION(player::GetPlayerPickShape);
            UPDATE_GAME_LUA_FUNCTION(player::GetPlayerPickBody);
            UPDATE_GAME_LUA_FUNCTION(player::GetPlayerInteractShape);
            UPDATE_GAME_LUA_FUNCTION(player::GetPlayerInteractBody);
            UPDATE_GAME_LUA_FUNCTION(player::SetPlayerScreen);
            UPDATE_GAME_LUA_FUNCTION(player::GetPlayerScreen);
            UPDATE_GAME_LUA_FUNCTION(player::SetPlayerHealth);
            UPDATE_GAME_LUA_FUNCTION(player::GetPlayerHealth);
            UPDATE_GAME_LUA_FUNCTION(player::SetPlayerRegenerationState);
            UPDATE_GAME_LUA_FUNCTION(player::RespawnPlayer);
            UPDATE_GAME_LUA_FUNCTION(player::GetPlayerWalkingSpeed);
            UPDATE_GAME_LUA_FUNCTION(player::SetPlayerWalkingSpeed);
            UPDATE_GAME_LUA_FUNCTION(player::GetPlayerParam);
            UPDATE_GAME_LUA_FUNCTION(player::SetPlayerParam);
            UPDATE_GAME_LUA_FUNCTION(player::RegisterTool);
            UPDATE_GAME_LUA_FUNCTION(player::GetToolBody);
            UPDATE_GAME_LUA_FUNCTION(player::SetToolTransform);

            return true;
        }
    }
}

#endif // TDMP_GENERATED_OFFSETS_H