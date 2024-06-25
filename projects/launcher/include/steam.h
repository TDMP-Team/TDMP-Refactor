#ifndef TDMP_STEAM_H
#define TDMP_STEAM_H

#include "pch.h"

namespace tdmp::steam {

    bool getPath(std::wstring& outPath);
    bool getGamePath(fs::path& outPath, std::wstring_view folderName, std::wstring_view executableName);

}

#endif // TDMP_STEAM_H