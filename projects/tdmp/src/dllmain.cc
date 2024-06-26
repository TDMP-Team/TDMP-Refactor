#include "pch.h"
#include "memory/memory.h"
#include "memory/hooks.h"
#include "teardown/types.h"
#include "teardown/teardown.h"
#include "offsets_generated.h"
#include "util/util.h"

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