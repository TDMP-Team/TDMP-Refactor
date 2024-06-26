#ifndef TDMP_PCH_H
#define TDMP_PCH_H

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


extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#include "argparse/argparse.hpp"
#include "MinHook.h"

#include "console.h"

#endif // TDMP_PCH_H