#include "pch.h"
#include "memory.h"

void earlyEntry();

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved)
{
    switch( fdwReason )  { 
        case DLL_PROCESS_ATTACH:
            earlyEntry();
            break;
        case DLL_PROCESS_DETACH:
            if (lpvReserved != nullptr) {
                break;
            }

            break;
    }
    return TRUE;
}

void earlyEntry() {
    // Connect to launcher's console
    //------------------------------------------------------------------------
    AttachConsole(ATTACH_PARENT_PROCESS);

    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);

    // Other
    //------------------------------------------------------------------------
    printf("Injected!\n");
}