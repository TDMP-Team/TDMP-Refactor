#ifndef TDMP_PCH_H
#define TDMP_PCH_H

#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <psapi.h>

#include <cstdio>
#include <format>
#include <iomanip>
#include <iostream>
#include <map>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <cassert>
#include <optional>
#include <functional>

#include <dxgi.h>
#include <d3d11.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"

#if defined(TDMP_DEBUG)
    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>

    #define _new_ new
    #define new _new_(_NORMAL_BLOCK, __FILE__, __LINE__)

    #define _malloc_ malloc
    #define malloc(size) _malloc_dbg(size, _NORMAL_BLOCK, __FILE__, __LINE__)

    #define _realloc_ realloc
    #define realloc(ptr, size) _realloc_dbg(ptr, size, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif


extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#include "argparse/argparse.hpp"
#include "MinHook.h"

#include "teardown/containers/td_containers.h"
#include "console.h"

#include "steam/steam_api.h"

#endif // TDMP_PCH_H