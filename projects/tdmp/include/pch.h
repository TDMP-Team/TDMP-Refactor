#ifndef TDMP_PCH_H
#define TDMP_PCH_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <psapi.h>

#include <iostream>
#include <cstdio>
#include <string>
#include <string_view>
#include <format>
#include <unordered_map>
#include <type_traits>
#include <iomanip>
#include <span>
#include <map>


extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#include "argparse/argparse.hpp"
#include "MinHook.h"

#include "console.h"

#endif // TDMP_PCH_H