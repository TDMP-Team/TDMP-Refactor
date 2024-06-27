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
    lua_State* L = teardown::funcs::tlua_newstate(f, ud);
    console::writeln("Lua State: 0x{:X}", (uintptr_t)L);

    return L;
}

structures::teardown* h_teardown_initialize(structures::teardown* magicShit, DWORD** a2, int64_t a3) {
    structures::teardown* result = teardown::funcs::tinitialize(magicShit, a2, a3);
    console::writeln("Teardown Initialize?");

    tdmp::structures::small_string ss;
    teardown::small_string::fromCString(&ss, "options.audio.menumusic");

    int num = teardown::registry::getInt(result->RegistryThingyMbob, (uint8_t**)&ss);

    return result;
}

// Public Functions
//------------------------------------------------------------------------
void teardown::initialize() {
    console::setStatus("Setting up hooks");

    mem::hooks::addHook("Teardown::Initialize", tdmp::offsets::teardown::initialize, &h_teardown_initialize, &teardown::funcs::tinitialize);
    mem::hooks::addHook("luaL_newstate", tdmp::offsets::lua::lua_newstate, &h_lua_newstate, &teardown::funcs::tlua_newstate);

    teardown::small_string::fromCString = (teardown::types::small_string::tfromCString)(tdmp::offsets::small_string::fromCString);
    teardown::small_string::free = (teardown::types::small_string::tFree)(tdmp::offsets::small_string::free);

    teardown::registry::getInt = (teardown::types::registry::tgetInt)(tdmp::offsets::registry::getInt);

    // TODO: Figure out how to set a custom tag for the logging function
    teardown::funcs::tlog = (teardown::types::tlog)(tdmp::offsets::game::log);
    teardown::funcs::tlog(log_level::debug, "Debug");
    teardown::funcs::tlog(log_level::info, "Info");
    teardown::funcs::tlog(log_level::warning, "Warning");
    teardown::funcs::tlog(log_level::error, "Error");
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