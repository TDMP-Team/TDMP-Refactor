#ifndef TDMP_PCH_H
#define TDMP_PCH_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <shellapi.h>

#include <filesystem>
#include <fstream>
#include <format>

#include <cstdio>
#include <stdexcept>

#include <string_view>
#include <string>

#include <numeric>
#include <utility>
#include <vector>
#include <map>

#include "argparse/argparse.hpp"

namespace fs = std::filesystem;

#endif // TDMP_PCH_H