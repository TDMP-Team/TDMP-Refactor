#include "pch.h"
#include "dumper.h"
#include "memory.h"
#include "util/util.h"

using namespace tdmp;

struct signature_t {
    signature_t(std::string _name, const char* _sig, bool _relative = false)
    : name(_name), signature((char*)_sig), relative(_relative) {}

    std::string name;
    char* signature;
    bool relative;
};

bool util::dumpOffsets() {
    MODULEINFO mi = { 0 };
    if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandleW(nullptr), &mi, sizeof(MODULEINFO))) {
        util::displayLastError(L"GetModuleInformation Failed");
        return false;
    }

    std::vector<std::string> invalidSignatures;

    std::map<std::string, std::vector<signature_t>> signatureNamespaces = {
        { "lua", {
            signature_t("lua_newstate", "E8 ? ? ? ? 48 8B 4B 30 48 89 01")
        }},
        
        { "game", {
            signature_t("log", "E8 ? ? ? ? 3B 37")
        }}
    };

    std::stringstream header, genFuncSS;

    genFuncSS << "    inline void generate() {\n"
              << "        const uint64_t base = mem::baseAddress();\n\n";

    { // Header
        header << "#ifndef TDMP_GENERATED_OFFSETS_H\n"
            << "#define TDMP_GENERATED_OFFSETS_H\n\n"
            << "#include \"memory.h\"\n\n"
            << "namespace tdmp::offsets {\n\n";

        for (const auto& ns : signatureNamespaces) {
            std::string nsName = ns.first;
            std::vector<signature_t> nsSignatures = ns.second;

            header << "    namespace " << nsName << " {\n";

            for (const signature_t& sig : nsSignatures) {
                const uint64_t address = mem::findIDAPattern(sig.signature, sig.relative);
                const uint64_t offsetFrombase = address - mem::baseAddress();

                if (!address) {
                    invalidSignatures.emplace_back((nsName + std::string("::") + sig.name));
                } else {
                    genFuncSS << "        " << nsName << "::" << sig.name << " = base + " << nsName << "::" << sig.name << ";\n";
                }

                std::cout << std::format("{}::{} = {:#x}\n", nsName, sig.name, offsetFrombase);

                std::string offsetStr = std::format("{:#x}", offsetFrombase);
                header << "        static uint64_t " << sig.name << " = " << offsetStr << ";\n";
            }

            header << "    }\n\n";
        }

        genFuncSS << "    }\n";

        header << genFuncSS.str() << '\n';
        header << "}\n\n"
            << "#endif // TDMP_GENERATED_OFFSETS_H";
    }

    std::string finalString = header.str();
    HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, finalString.length() + 1);
    memcpy(GlobalLock(hMem), finalString.data(), finalString.length() + 1);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
    GlobalFree(hMem);

    util::displayMessage(MB_OK, L"Copied generated offsets to clipboard!");

    if (invalidSignatures.size() > 0) {
        console::writeln("Failed getting offsets, invalid signatures are:");
        for (const std::string& name : invalidSignatures) {
            console::writeln("    {}", name.data());
        }
    }

    console::writeln("\n{}", finalString.data());

    return true;
}