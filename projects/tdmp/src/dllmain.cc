#include "pch.h"
#include "memory.h"
#include "util/util.h"

using namespace tdmp;

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

    int mode;
    GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), reinterpret_cast<LPDWORD>(&mode));
    mode |= ENABLE_PROCESSED_INPUT;
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), mode);

    // Parse Arguments
    //------------------------------------------------------------------------
     argparse::ArgumentParser args("TDMP");
     args.add_argument("-dump").default_value(false).implicit_value(true);

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

    // Other
    //------------------------------------------------------------------------
    printf("Injected!\n");

     if (args.get<bool>("-dump")) {
         printf("Dumping!\n");
     }
}