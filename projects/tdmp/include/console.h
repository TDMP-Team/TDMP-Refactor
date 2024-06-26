#ifndef TDMP_CONSOLE_H
#define TDMP_CONSOLE_H

#include "pch.h"
#include "util/util.h"

namespace tdmp::console {

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

        defaultConsoleTitle = getConsoleTitle();
    }

    template<typename... Args>
    inline void write(const std::string& fmt, Args&&... args) {
        std::string formatted = std::vformat(fmt, std::make_format_args(args...));
        WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), formatted.c_str(), static_cast<DWORD>(formatted.length()), nullptr, nullptr);
    }

    template<typename... Args>
    inline void writeln(const std::string& fmt, Args&&... args) {
        std::string concatenatedString(fmt);
        concatenatedString += "\n";

        console::write(concatenatedString, std::forward<Args>(args)...);
    }

}

#endif // TDMP_CONSOLE_H