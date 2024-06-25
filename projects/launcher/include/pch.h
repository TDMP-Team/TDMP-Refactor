#ifndef TDMP_PCH_H
#define TDMP_PCH_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <shellapi.h>
#include <cstdio>
#include <stdexcept>
#include <optional>
#include <string_view>
#include <string>
#include <format>
#include <filesystem>
#include <fstream>
#include <numeric>
#include <utility>

#include "argparse/argparse.hpp"

namespace fs = std::filesystem;

#endif // TDMP_PCH_H