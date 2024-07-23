#include "pch.h"
#include "teardown/teardown.h"
#include "teardown/types.h"
#include "offsets_generated.h"
#include "memory/dumper.h"

using namespace mp;

void earlyEntryThread();

HANDLE g_hThread = NULL;
HANDLE hEvent = NULL;

DWORD WINAPI threadFunction(LPVOID lpParam) {
    SetEvent((HANDLE)lpParam);
    earlyEntryThread();

    return TRUE;
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        console::init();

    #if defined(TDMP_DEBUG)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif

        DisableThreadLibraryCalls(hinstDLL);

        hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (hEvent == NULL) {
            return FALSE;
        }

        // Create the thread, passing the event handle as a parameter
        g_hThread = CreateThread(nullptr, 0, threadFunction, hEvent, 0, nullptr);
        if (g_hThread == NULL) {
            CloseHandle(hEvent);
            return FALSE;
        }
    }

    return TRUE;
}

void pauseMainThreadAndWaitForChildThread() {
    if (g_hThread != NULL) {
        // Wait for the thread to signal that it has started
        WaitForSingleObject(hEvent, INFINITE);

        CloseHandle(hEvent);
        WaitForSingleObject(g_hThread, INFINITE);
    }
}

void earlyEntryThread() {
    // Parse Arguments
    //------------------------------------------------------------------------
    argparse::ArgumentParser args("TDMP");
    args.add_argument("-dump").default_value(false).implicit_value(true);
    args.add_argument("-generate-structs").default_value(false).implicit_value(true);

    {
        int argc;
        char** argv = util::commandLineToArgvA(GetCommandLineA(), &argc);

        try {
            args.parse_known_args(argc, argv);
        } catch (const std::exception& e) {
            std::wstring err = util::s2ws(e.what());
            util::displayError(L"Failed parsing arguments: {}", err);
            return;
        }
    }

    if (!mem::initializeMemory()) {
        util::displayLastError(L"Failed to initialize memory");
        ExitProcess(1);
        return;
    }

    {
        bool wasDumped = false;

        if (args.get<bool>("-dump")) {
            dumper::dump(false);
            wasDumped = true;
        }

        if (args.get<bool>("-generate-structs")) {
            dumper::dump(true);
            wasDumped = true;
        }

        if (wasDumped) {
            ExitProcess(0);
        }
    }

    console::setStatus("Generating addresses");

    std::vector<std::string> badOffsets;
    if (!offsets::generate(badOffsets)) {
        std::stringstream ss;
        ss << "Invalid offsets: " << badOffsets.size() << "\n";

        for (const std::string& offset : badOffsets) {
            ss << "  " << offset << '\n';
        }

        std::wstring ssStr = util::s2ws(ss.str());

        console::writeln(L"\n{}", ssStr);
        util::displayError(ssStr);

        ExitProcess(1);
    }

    // Assign all the functions to their addresses
    funcs::assign();
    lua_funcs::assign();

    if (MH_STATUS status = MH_Initialize(); status != MH_OK) {
        std::wstring err = util::s2ws(MH_StatusToString(status));
        util::displayError(L"Failed initializing minhook: {}", err);
        return;
    }

    teardown::initialize();
    console::setStatus("");
}