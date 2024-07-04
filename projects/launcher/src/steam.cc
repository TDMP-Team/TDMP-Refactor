#include "pch.h"
#include "steam.h"
#include "shared/util/util.h"

using namespace mp;

bool steam::getPath(std::wstring& outPath) {
    wchar_t path[MAX_PATH];
    HKEY steamKey;

    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\Valve\\Steam", 0, KEY_QUERY_VALUE, &steamKey) != ERROR_SUCCESS) {
        return false;
    }

    DWORD len = MAX_PATH;
    if (RegQueryValueExW(steamKey, L"InstallPath", nullptr, nullptr, reinterpret_cast<LPBYTE>(&path), &len) == ERROR_SUCCESS) {
        path[len - 1] = '\0';
        RegCloseKey(steamKey);

        outPath = path;
        return true;
    }

    return false;
}

bool steam::getGamePath(fs::path& outPath, std::wstring_view folderName, std::wstring_view executableName) {
    // Get steam path
    std::wstring steamPath;
    if (!mp::steam::getPath(steamPath)) {
        if (GetLastError() != 0) {
            util::displayLastError(L"Failed getting steam path");
        } else {
            util::displayError(L"Failed getting steam path");
        }

        return false;
    }

    // Read libraryfolders file
    std::wifstream configFile(steamPath + L"\\steamapps\\libraryfolders.vdf");
    if (!configFile.is_open()) {
        util::displayError(L"Could not open libraryfolders.vdf");
        return false;
    }

    std::wstring line;

    while (std::getline(configFile, line)) {
        if (line.find(L"\"path\"") != std::wstring::npos) {
            size_t first_quote = line.find('"', 7);
            if (first_quote == std::wstring::npos)
                continue;

            size_t second_quote = line.find('"', first_quote + 1);
            if (second_quote == std::string::npos)
                continue;

            std::wstring path = line.substr(first_quote + 1);
            path.erase(0, 3);
            path.erase(path.size() - 1);

            fs::path executablePath = fs::path(path) / "steamapps" / "common" / folderName / executableName;
            
            if (fs::exists(executablePath)) {
                outPath = executablePath;
                return true;
            }
        }
    }

    return false;
}