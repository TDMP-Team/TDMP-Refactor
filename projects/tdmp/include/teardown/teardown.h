#ifndef TDMP_TEARDOWN_H
#define TDMP_TEARDOWN_H

#include "pch.h"
#include "memory/memory.h"

namespace tdmp::teardown {

    void initialize();
    void earlyEntryThread();

    // TODO: Move this somewhere else
    namespace game_lua_funcs {
        namespace player {
            static mem::game_function GetPlayerPos("GetPlayerPos");
            static mem::game_function GetPlayerTransform("GetPlayerTransform");
            static mem::game_function SetPlayerTransform("SetPlayerTransform");
            static mem::game_function SetPlayerZoom("SetPlayerZoom");
            static mem::game_function SetPlayerGroundVelocity("SetPlayerGroundVelocity");
            static mem::game_function GetPlayerCameraTransform("GetPlayerCameraTransform");
            static mem::game_function SetPlayerCameraOffsetTransform("SetPlayerCameraOffsetTransform");
            static mem::game_function SetPlayerSpawnTransform("SetPlayerSpawnTransform");
            static mem::game_function SetPlayerSpawnHealth("SetPlayerSpawnHealth");
            static mem::game_function SetPlayerSpawnTool("SetPlayerSpawnTool");
            static mem::game_function GetPlayerVelocity("GetPlayerVelocity");
            static mem::game_function SetPlayerVehicle("SetPlayerVehicle");
            static mem::game_function SetPlayerVelocity("SetPlayerVelocity");
            static mem::game_function GetPlayerVehicle("GetPlayerVehicle");
            static mem::game_function IsPlayerGrounded("IsPlayerGrounded");
            static mem::game_function GetPlayerGrabShape("GetPlayerGrabShape");
            static mem::game_function GetPlayerGrabBody("GetPlayerGrabBody");
            static mem::game_function ReleasePlayerGrab("ReleasePlayerGrab");
            static mem::game_function GetPlayerPickShape("GetPlayerPickShape");
            static mem::game_function GetPlayerPickBody("GetPlayerPickBody");
            static mem::game_function GetPlayerInteractShape("GetPlayerInteractShape");
            static mem::game_function GetPlayerInteractBody("GetPlayerInteractBody");
            static mem::game_function SetPlayerScreen("SetPlayerScreen");
            static mem::game_function GetPlayerScreen("GetPlayerScreen");
            static mem::game_function SetPlayerHealth("SetPlayerHealth");
            static mem::game_function GetPlayerHealth("GetPlayerHealth");
            static mem::game_function SetPlayerRegenerationState("SetPlayerRegenerationState");
            static mem::game_function RespawnPlayer("RespawnPlayer");
            static mem::game_function GetPlayerWalkingSpeed("GetPlayerWalkingSpeed");
            static mem::game_function SetPlayerWalkingSpeed("SetPlayerWalkingSpeed");
            static mem::game_function GetPlayerParam("GetPlayerParam");
            static mem::game_function SetPlayerParam("SetPlayerParam");
            static mem::game_function RegisterTool("RegisterTool");
            static mem::game_function GetToolBody("GetToolBody");
            static mem::game_function SetToolTransform("SetToolTransform");
        }
    }

}

#endif // TDMP_TEARDOWN_H