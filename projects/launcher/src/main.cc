#include "pch.h"
#include "shared/util/util.h"
#include "steam.h"

using namespace tdmp;

static constexpr wchar_t DLL_NAME[] = L"tdmp.dll";
static PROCESS_INFORMATION pi{};

static BOOL WINAPI consoleHandler(DWORD dwCtrlType) {
    switch (dwCtrlType) {
    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        TerminateProcess(pi.hProcess, 0);
        return TRUE;
    default:
        util::displayMessage(MB_OK, L"consoleHandler -> {}", dwCtrlType);
        return FALSE;
    }
}

static std::wstring getCommandLineSkipFirst() {
    LPWSTR commandLine = GetCommandLineW();
    std::wstring cmdline = commandLine;

    size_t spacePos = cmdline.find(' ');
    if (spacePos != std::wstring::npos)
        return cmdline.substr(spacePos + 1);

    return L"";
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
#if defined(TDMP_DEBUG)
    const std::wstring teardownExeName = L"teardown.exe.unpacked.exe";
#else
    const std::wstring teardownExeName = L"teardown.exe";
#endif

    // Get Game Path
    //------------------------------------------------------------------------
    fs::path teardownPath;
    if (!steam::getGamePath(teardownPath, L"Teardown", teardownExeName)) {
    #if defined(TDMP_DEBUG)
        util::displayError(L"Could not find {}, please download \"Steamless\" and unpack Teardown in order to debug properly", teardownExeName);
    #else
        util::displayError(L"Could not locate teardown\nIf you have the game installed, please contact us so we can help fix the problem");
    #endif
        return 1;
    }

    const std::wstring executablePath = std::wstring(teardownPath);
    const std::wstring executableRoot = std::wstring(teardownPath.parent_path());

    // Get DLL Path
    //------------------------------------------------------------------------
    std::wstring dllPathString = L"";

    {
        /* Priority Order:
         * 1. Current path
         * 2. (If debug) Debug folder
         * 3. Teardown root directory
        */

        // Places we want to search for the dll
        const std::wstring searchPaths[] = {
            std::wstring(fs::current_path()),

            #if TDMP_DEBUG
                fs::current_path().parent_path() / L"tdmp\\build\\bin\\debug",
                fs::current_path().parent_path().parent_path().parent_path().parent_path() / L"tdmp\\build\\bin\\debug", // This is stupid but it works if you run the launcher from its directory
            #endif

            executableRoot
        };



        for (const auto& searchPath : searchPaths) {
            const fs::path dllPath = searchPath / fs::path(DLL_NAME);

            if (fs::exists(dllPath)) {
                dllPathString = std::wstring(dllPath);
                break;
            }
        }

        if (dllPathString.empty()) {
            std::wstring joinedPaths = std::accumulate(
                std::begin(searchPaths),
                std::end(searchPaths),
                std::wstring(L""),
                [](const std::wstring& a, const std::wstring& b) {
                    return a.empty() ? b : a + L"\n" + b;
                }
            );

            util::displayError(L"Could not locate tdmp.dll, searched in locations:\n\n{}", joinedPaths);
            return 1;
        }
    }

    // Setup Console
    //------------------------------------------------------------------------
    FILE* f = nullptr;

    if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
        // We couldn't attach to an existing console, so let's create a new one
        if (!AllocConsole()) {
            util::displayLastError(L"AttachConsole & AllocConsole Failed");
            return 1;
        }

        SetConsoleTitleA("Teardown Multiplayer");
        freopen_s(&f, "CONOUT$", "w", stdout);
        freopen_s(&f, "CONOUT$", "w", stderr);
    }

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hStdErr = GetStdHandle(STD_ERROR_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE || hStdErr == INVALID_HANDLE_VALUE) {
        util::displayLastError(L"GetStdHandle Failed");
        return 1;
    }

    // Spawn Process
    //------------------------------------------------------------------------
    std::wstring currentPath = util::s2ws(fs::current_path().string());
    std::wstring launchOptions = std::format(L"\"{}\" -launch-dir=\"{}\" {}", executablePath, currentPath, getCommandLineSkipFirst());

    STARTUPINFOW si = { 0 };
    si.cb           = sizeof(STARTUPINFOW);
    si.dwFlags      = STARTF_USESTDHANDLES;
    si.hStdOutput   = hStdOut;
    si.hStdError    = hStdErr;

    DWORD exitCode = 0;

    // Make Teardown think we launched it from Steam
    SetEnvironmentVariableA("SteamAppId", "1167630");

    if (!CreateProcessW(nullptr,
        (LPWSTR)launchOptions.data(),
        nullptr,
        nullptr,
        TRUE,
        CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED | INHERIT_PARENT_AFFINITY,
        nullptr,
        (LPWSTR)executableRoot.data(),
        &si,
        &pi)) {
        util::displayError(L"CreateProcessW Failed");
        return 1;
    }

    LPVOID loadlibAddress;
    HANDLE thread;

    // Allocate Memory
    //------------------------------------------------------------------------
    LPVOID remoteDll = VirtualAllocEx(pi.hProcess, nullptr, dllPathString.length() + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!remoteDll) {
        util::displayLastError(L"VirtualAllocEx Failed");
        exitCode = 1;
        goto cleanup;
    }

    // Write Memory
    //------------------------------------------------------------------------
    if (!WriteProcessMemory(pi.hProcess, remoteDll, dllPathString.data(), (dllPathString.length() + 1) * sizeof(wchar_t), nullptr)) {
        util::displayLastError(L"WriteProcessMemory Failed");
        exitCode = 1;
        goto cleanup;
    }

    // Get LoadLibraryW Address
    //------------------------------------------------------------------------
    loadlibAddress = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryW");
    if (!loadlibAddress) {
        util::displayLastError(L"GetProcAddress Failed");
        exitCode = 1;
        goto cleanup;
    }

    // Create Thread
    //------------------------------------------------------------------------
    thread = CreateRemoteThread(pi.hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)loadlibAddress, remoteDll, 0, nullptr);
    if (!thread) {
        util::displayLastError(L"CreateRemoteThread Failed");
        exitCode = 1;
        goto cleanup;
    }

    VirtualFreeEx(pi.hProcess, thread, 0, MEM_RELEASE);
    ResumeThread(pi.hThread);

    SetConsoleCtrlHandler(consoleHandler, TRUE); // ! Does not work with AttachConsole (CTRL+C event)
    //     GenerateConsoleCtrlEvent(CTRL_C_EVENT, pi.dwProcessId);

    WaitForSingleObject(pi.hThread, INFINITE);

    if (!GetExitCodeProcess(pi.hProcess, &exitCode)) {
        util::displayLastError(L"CreateRemoteThread Failed");
        exitCode = 1;
        goto cleanup;
    }

cleanup:
    if (pi.hProcess != INVALID_HANDLE_VALUE)
        CloseHandle(pi.hProcess);

    if (pi.hThread != INVALID_HANDLE_VALUE)
        CloseHandle(pi.hThread);

    fclose(f);
    TerminateProcess(pi.hProcess, 0);

    CloseHandle(hStdOut);
    CloseHandle(hStdErr);

    if (GetConsoleWindow()) {
        if (!CloseWindow(GetConsoleWindow())) {
            util::displayLastError(L"CloseWindow failed");
            exitCode = 1;
        }
    }

    ExitProcess(exitCode);
}