#include "pch.h"
#include "memory.h"
#include "dumper.h"
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
            DisableThreadLibraryCalls(hinstDLL);
            CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(earlyEntry), nullptr, 0, nullptr);
            break;
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

namespace tdmp::types {
    using lua_newstate = std::add_pointer_t<lua_State*(lua_Alloc f, void* ud)>;
    using td_log = std::add_pointer_t<int(int level, const char* fmt, ...)>;
}

tdmp::types::lua_newstate o_lua_newstate = nullptr;
tdmp::types::td_log td_log = nullptr;

lua_State* L;

lua_State* h_lua_newstate(lua_Alloc f, void* ud) {
    L = o_lua_newstate(f, ud);
    std::cout << "Lua State: 0x" << std::hex << L << '\n';

//     luaL_dostring(L, "print('Hello')");
//     luaL_openlibs(L);

    return L;
}

enum class td_log_level : uint8_t {
    debug   = 0,
    info    = 1,
    warning = 2,
    error   = 3
};

void earlyEntry() {
    // Connect to launcher's console
    //------------------------------------------------------------------------
    AttachConsole(ATTACH_PARENT_PROCESS);

    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);

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

    Sleep(1000);

    mem::getModuleInfo();

     if (args.get<bool>("-dump")) {
         util::dumpOffsets();
         ExitProcess(0);
     }

     MH_Initialize();

     mem::make_hook("luaL_newstate", mem::baseAddress() + tdmp::offsets::lua::lua_newstate, &h_lua_newstate, &o_lua_newstate);

     td_log = (tdmp::types::td_log)(mem::baseAddress() + tdmp::offsets::game::log);
     td_log(0, "Zero");
     td_log(1, "One");
     td_log(2, "Three");
}