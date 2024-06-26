#include "pch.h"
#include "teardown/teardown.h"

using namespace tdmp;

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved)
{
    switch( fdwReason )  { 
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDLL);
            console::init();

            CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(teardown::earlyEntryThread), nullptr, 0, nullptr);
            break;
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}