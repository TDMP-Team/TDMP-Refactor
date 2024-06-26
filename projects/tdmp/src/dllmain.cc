#include "pch.h"
#include "teardown/teardown.h"

using namespace tdmp;

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH) {
    #if defined(TDMP_DEBUG)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif

        DisableThreadLibraryCalls(hinstDLL);
        console::init();

        CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(teardown::earlyEntryThread), nullptr, 0, nullptr);
    }

    return TRUE;
}