#ifndef TDMP_CONSOLE_H
#define TDMP_CONSOLE_H

#include "pch.h"
#include "shared/util/util.h"
#include "teardown/containers/td_containers.h"

namespace mp::console {

    inline std::string defaultConsoleTitle;

    inline std::string getConsoleTitle() {
        char buffer[MAX_PATH];
        DWORD size = GetConsoleTitleA(buffer, MAX_PATH);

        if (size > 0) {
            return std::string(buffer, size);
        } else {
            util::displayLastError(L"getConsoleTitle failed");
            return "";
        }
    }

    template<typename... Args>
    inline void setStatus(const std::string& fmt, Args&&... args) {
        if (fmt.empty()) {
            SetConsoleTitleA(defaultConsoleTitle.c_str());
            return;
        }

        std::string formatted = std::vformat(fmt, std::make_format_args(args...));
        std::string newTitle = std::format("{} [{}]", defaultConsoleTitle, formatted);
        SetConsoleTitleA(newTitle.c_str());
    }

    inline void init() {
        AttachConsole(ATTACH_PARENT_PROCESS);

        FILE* f;
        freopen_s(&f, "CONOUT$", "w", stdout);
        freopen_s(&f, "CONOUT$", "w", stderr);

        DWORD mode = 0;
        GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), reinterpret_cast<LPDWORD>(&mode));
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), mode);

        defaultConsoleTitle = getConsoleTitle();
    }

    template<typename... Args>
    inline void write(const std::string& fmt, Args&&... args) {
        std::string formatted = std::vformat(fmt, std::make_format_args(args...));
        WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), formatted.c_str(), static_cast<DWORD>(formatted.length()), nullptr, nullptr);
        OutputDebugStringA(formatted.c_str());
    }

    template<typename... Args>
    inline void writeln(const std::string& fmt, Args&&... args) {
        std::string concatenatedString(fmt);
        concatenatedString += "\n";

        console::write(concatenatedString, std::forward<Args>(args)...);
    }

    inline void writeln_td(const td::td_string& tdStr) {
        WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), tdStr.c_str(), static_cast<DWORD>(tdStr.length()), nullptr, nullptr);
        WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), "\n", 1, nullptr, nullptr);
        OutputDebugStringA(tdStr.c_str());
    }

    // Wide alternatives
    template<typename... Args>
    inline void write(const std::wstring& fmt, Args&&... args) {
        std::wstring formatted = std::vformat(fmt, std::make_wformat_args(args...));
        WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), formatted.c_str(), static_cast<DWORD>(formatted.length()), nullptr, nullptr);
        OutputDebugStringW(formatted.c_str());
    }

    template<typename... Args>
    inline void writeln(const std::wstring& fmt, Args&&... args) {
        std::wstring concatenatedString(fmt);
        concatenatedString += L"\n";

        console::write(concatenatedString, std::forward<Args>(args)...);
    }

}

#endif // TDMP_CONSOLE_H