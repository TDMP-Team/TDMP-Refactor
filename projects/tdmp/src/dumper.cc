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
    if (!mem::getModuleInfo()) {
        return false;
    }

    MODULEINFO mi = { 0 };
    if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandleW(nullptr), &mi, sizeof(MODULEINFO))) {
        util::displayLastError(L"GetModuleInformation Failed");
        return false;
    }

    std::vector<std::string> invalidSignatures;

    std::map<std::string, std::vector<signature_t>> signatureNamespaces = {
        { "lua", {
            signature_t("lua_newstate", "E8 ? ? ? ? 48 8B 4B 30 48 89 01")
        }}
    };

    std::stringstream ss;
    ss << "#ifndef TDMP_GENERATED_OFFSETS_H\n"
       << "#define TDMP_GENERATED_OFFSETS_H\n\n"
       << "namespace tdmp::offsets {\n\n";

    for (const auto& ns : signatureNamespaces) {
        std::string nsName = ns.first;
        std::vector<signature_t> nsSignatures = ns.second;

        ss << "    namespace " << nsName << " {\n";

        for (const signature_t& sig : nsSignatures) {
            const uint64_t offset = mem::findIDAPattern(sig.signature, sig.relative);
            if (!offset) {
                invalidSignatures.emplace_back((nsName + std::string("::") + sig.name));
            }

            std::cout << std::format("{}::{} = {:#x}\n", nsName, sig.name, offset);

            std::string offsetStr = std::format("{:#x}", offset);
            ss << "        constexpr uint64_t " << sig.name << " = " << offsetStr << ";\n";
        }

        ss << "    }\n\n";
    }

    ss << "}\n\n"
       << "#endif // TDMP_GENERATED_OFFSETS_H";

    std::string finalString = ss.str();
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
        printf("Failed getting offsets, invalid signatures are:\n");
        for (const std::string& name : invalidSignatures) {
            printf("    %s\n", name.data());
        }
    }

    printf("\n%s\n", finalString.data());

    return true;
}