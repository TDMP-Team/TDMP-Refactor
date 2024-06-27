#include "pch.h"
#include "teardown/teardown.h"
#include "teardown/types.h"
#include "util/util.h"
#include "memory/hooks.h"
#include "offsets_generated.h"

using namespace tdmp;

// Hooked Functions
//------------------------------------------------------------------------
lua_State* h_lua_newstate(lua_Alloc f, void* ud) {
    lua_State* L = funcs::lua::lua_newstate(f, ud);
    console::writeln("Lua State: 0x{:X}", (uintptr_t)L);

    return L;
}

structures::teardown* h_teardown_initialize(structures::teardown* magicShit, DWORD** a2, int64_t a3) {
    structures::teardown* result = funcs::teardown::initialize(magicShit, a2, a3);
    console::writeln("Teardown Initialize?");

    tdmp::structures::small_string ss;
    funcs::small_string::fromCString(&ss, "options.audio.menumusic");

    int num = funcs::registry::getInt(result->RegistryThingyMbob, (uint8_t**)&ss);

    return result;
}

// Public Functions
//------------------------------------------------------------------------
void teardown::initialize() {
    console::setStatus("Setting up hooks");

    mem::hooks::addHook("Teardown::Initialize", tdmp::offsets::teardown::initialize, &h_teardown_initialize, &funcs::teardown::initialize);
    mem::hooks::addHook("luaL_newstate", tdmp::offsets::lua::lua_newstate, &h_lua_newstate, &funcs::lua::lua_newstate);

    funcs::small_string::fromCString = (funcs::types::small_string::tfromCString)(tdmp::offsets::small_string::fromCString);
    funcs::small_string::free = (funcs::types::small_string::tfree)(tdmp::offsets::small_string::free);

    funcs::registry::getInt = (funcs::types::registry::tgetInt)(tdmp::offsets::registry::getInt);

    // TODO: Figure out how to set a custom tag for the logging function
    funcs::game::log = (funcs::types::game::tlog)(tdmp::offsets::game::log);
    //funcs::game::log(log_level::debug, "Debug");
    //funcs::game::log(log_level::info, "Info");
    //funcs::game::log(log_level::warning, "Warning");
    //funcs::game::log(log_level::error, "Error");
}

void teardown::earlyEntryThread() {
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

    if (!mem::initializeMemory()) {
        util::displayLastError(L"Failed to initialize memory");
        ExitProcess(1);
        return;
    }

    // Wait for data section to load and update all the offsets with the base address
    mem::waitForSection(GetModuleHandle(NULL), ".data");

    if (args.get<bool>("-dump")) {
        mem::dumpOffsets();
        ExitProcess(0);
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

    if (MH_STATUS status = MH_Initialize(); status != MH_OK) {
        std::wstring err = util::s2ws(MH_StatusToString(status));
        util::displayError(L"Failed initializing minhook: {}", err);
        return;
    }

    teardown::initialize();

    console::setStatus("");
}