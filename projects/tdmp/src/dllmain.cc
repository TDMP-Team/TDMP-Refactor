#include "pch.h"
#include "teardown/teardown.h"

using namespace mp;

HANDLE g_hThread = NULL;
HANDLE hEvent = NULL;

DWORD WINAPI threadFunction(LPVOID lpParam) {
    SetEvent((HANDLE)lpParam);
    teardown::earlyEntryThread();

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