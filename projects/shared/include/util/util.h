#ifndef TDMP_UTIL_H
#define TDMP_UTIL_H

#include "pch.h"

namespace tdmp::util {

static std::pair<int32_t, std::wstring> getLastError() {
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0) {
        return {};
    }
    
    LPWSTR messageBuffer = nullptr;

    size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

    std::wstring message(messageBuffer, size);
    LocalFree(messageBuffer);
            
    return std::make_pair(errorMessageID, message);
}

template<typename... Args>
static void outputDebugString(const std::wstring& format, Args&&... args) {
    std::wstring formatted = std::vformat(format, std::make_wformat_args(std::forward<Args>(args)...));
    if (!formatted.empty() && formatted.back() != L'\n') {
        formatted += L"\n";
    }

    OutputDebugStringW(formatted.data());
}

static void displayMessage(uint32_t type, std::wstring formatted) {
    if (!formatted.empty() && formatted.back() != L'\n') {
        formatted += L"\n";
    }

    MessageBoxW(NULL, formatted.c_str(), L"TDMP Launcher", type);

#if TDMP_DEBUG
    OutputDebugStringW(formatted.c_str());
#endif
}

template<typename... Args>
static void displayMessage(uint32_t type, const std::wstring& format, Args&&... args) {
    std::wstring formatted = std::vformat(format, std::make_wformat_args(std::forward<Args>(args)...));
    if (!formatted.empty() && formatted.back() != L'\n') {
        formatted += L"\n";
    }

    MessageBoxW(NULL, formatted.data(), L"TDMP Launcher", type);

#if TDMP_DEBUG
    OutputDebugStringW(formatted.data());
#endif
}

template<typename... Args>
static void displayError(const std::wstring& format, Args&&... args) {
    std::wstring formatted = std::vformat(format, std::make_wformat_args(std::forward<Args>(args)...));
    displayMessage(MB_OK | MB_ICONERROR, format, std::forward<Args>(args)...);
}

template<typename... Args>
void displayLastError(std::wstring format, Args&&... args) {
    auto [code, message] = getLastError();
    std::wstring formatted = L"";

    if (code == 0) {
        formatted = std::vformat(format, std::make_wformat_args(std::forward<Args>(args)...));
    } else {
        message = message.substr(0, message.length() - 2);
        format = format + L"\n\nCode: {}\nMessage: {}";

        auto originalArgs   = std::make_tuple(std::forward<Args>(args)...);
        auto additionalArgs = std::make_tuple(code, message);
        auto combinedArgs   = std::tuple_cat(originalArgs, additionalArgs);

        formatted = std::apply(
            [&](auto&&... combinedArgs) {
                return std::vformat(format, std::make_wformat_args(std::forward<decltype(combinedArgs)>(combinedArgs)...));
            },
            combinedArgs
        );
    }

    displayMessage(MB_OK | MB_ICONERROR, formatted);
}

}

#endif // TDMP_UTIL_H