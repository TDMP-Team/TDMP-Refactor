#ifndef TDMP_CONSOLE_H
#define TDMP_CONSOLE_H

#include "pch.h"

namespace tdmp::console {

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