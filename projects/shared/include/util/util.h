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
static void displayLastError(std::wstring format, Args&&... args) {
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

static void replaceAllOccurrences(std::string& input, const std::string& to_replace, const std::string& replacement) {
    size_t pos = 0;
    while ((pos = input.find(to_replace, pos)) != std::string::npos) {
        input.replace(pos, to_replace.length(), replacement);
        pos += replacement.length();
    }
}

// https://stackoverflow.com/a/10738141
static std::wstring s2ws(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo( size_needed, 0 );
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

// http://alter.org.ua/en/docs/win/args/
static PCHAR*
commandLineToArgvA(
    PCHAR CmdLine,
    int* _argc
    )
{
    PCHAR* argv;
    PCHAR  _argv;
    ULONG   len;
    ULONG   argc;
    CHAR   a;
    ULONG   i, j;

    BOOLEAN  in_QM;
    BOOLEAN  in_TEXT;
    BOOLEAN  in_SPACE;

    len = (ULONG)strlen(CmdLine);
    i = ((len+2)/2)*sizeof(PVOID) + sizeof(PVOID);

    argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
        i + (len+2)*sizeof(CHAR));

    _argv = (PCHAR)(((PUCHAR)argv)+i);

    argc = 0;
    argv[argc] = _argv;
    in_QM = FALSE;
    in_TEXT = FALSE;
    in_SPACE = TRUE;
    i = 0;
    j = 0;

    while( a = CmdLine[i] ) {
        if(in_QM) {
            if(a == '\"') {
                in_QM = FALSE;
            } else {
                _argv[j] = a;
                j++;
            }
        } else {
            switch(a) {
            case '\"':
                in_QM = TRUE;
                in_TEXT = TRUE;
                if(in_SPACE) {
                    argv[argc] = _argv+j;
                    argc++;
                }
                in_SPACE = FALSE;
                break;
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                if(in_TEXT) {
                    _argv[j] = '\0';
                    j++;
                }
                in_TEXT = FALSE;
                in_SPACE = TRUE;
                break;
            default:
                in_TEXT = TRUE;
                if(in_SPACE) {
                    argv[argc] = _argv+j;
                    argc++;
                }
                _argv[j] = a;
                j++;
                in_SPACE = FALSE;
                break;
            }
        }
        i++;
    }
    _argv[j] = '\0';
    argv[argc] = NULL;

    (*_argc) = argc;
    return argv;
}

}

#endif // TDMP_UTIL_H