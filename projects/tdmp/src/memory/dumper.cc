#include "pch.h"
#include "memory/memory.h"
#include "util/util.h"

using namespace tdmp;

struct signature_namespace {
    const char* ns;
    std::span<const mem::signature> signatures;
};

// Lua Signatures
//------------------------------------------------------------------------
static constexpr mem::signature luaSignatures[] = {
    {"lua_newstate", "E8 ? ? ? ? 48 8B 4B 30 48 89 01"}
};

// Game Signatures
//------------------------------------------------------------------------
static constexpr mem::signature gameSignatures[] = {
    {"log", "E8 ? ? ? ? 3B 37"}
};

// All Signatures
//------------------------------------------------------------------------
static constexpr signature_namespace signatureNamespaces[] = {
    {"lua", {luaSignatures, sizeof(luaSignatures) / sizeof(luaSignatures[0])}},
    {"game", {gameSignatures, sizeof(gameSignatures) / sizeof(gameSignatures[0])}}
};

static void generateHeaderStart(std::stringstream& header) {
    header << "#ifndef TDMP_GENERATED_OFFSETS_H\n"
        << "#define TDMP_GENERATED_OFFSETS_H\n\n"
        << "#include \"memory/memory.h\"\n\n"
        << "namespace tdmp::offsets {\n\n";
}

static void generateFunctionStart(std::stringstream& genFuncSS) {
    genFuncSS << "    inline void generate() {\n"
        << "        const uint64_t base = mem::baseAddress();\n\n";
}

static void processSignature(const std::string& nsName, const mem::signature& sig, std::stringstream& header, std::stringstream& genFuncSS, std::vector<std::string>& invalidSignatures) {
    const uint64_t address = mem::findIDAPattern(sig.sig, sig.relative);
    const uint64_t offsetFromBase = address - mem::baseAddress();

    if (!address) {
        invalidSignatures.emplace_back(nsName + "::" + sig.name);
    } else {
        std::string formattedLine = std::format("        {:<20} = base + {};\n",
                                                nsName + "::" + sig.name,
                                                nsName + "::" + sig.name);
        genFuncSS << formattedLine;
    }

    console::writeln("{}::{} = {:#x}\n", nsName, sig.name, offsetFromBase);

    std::string offsetStr = std::format("{:#x}", offsetFromBase);
    header << "        inline uint64_t " << sig.name << " = " << offsetStr << ";\n";
}

static void processNamespace(const signature_namespace& ns, std::stringstream& header, std::stringstream& genFuncSS, std::vector<std::string>& invalidSignatures) {
    std::string nsName = ns.ns;
    header << "    namespace " << nsName << " {\n";

    for (const mem::signature& sig : ns.signatures) {
        processSignature(nsName, sig, header, genFuncSS, invalidSignatures);
    }

    header << "    }\n\n";
}

static void generateHeaderEnd(std::stringstream& header, std::stringstream& genFuncSS) {
    genFuncSS << "    }\n";
    header << genFuncSS.str() << '\n';
    header << "}\n\n"
        << "#endif // TDMP_GENERATED_OFFSETS_H";
}

static void copyToClipboard(const std::string& text) {
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.length() + 1);
    if (!hMem) {
        util::displayLastError(L"copyToClipboard failed, could not allocate memory");
        return;
    }

    LPVOID locked = GlobalLock(hMem);
    if (!locked) {
        util::displayLastError(L"Could not lock memory in copyToClipboard");
        GlobalFree(hMem);
        return;
    }

    memcpy(locked, text.data(), text.length() + 1);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();

    GlobalFree(hMem);

    util::displayMessage(MB_OK, L"Copied generated offsets to clipboard!");
}

bool mem::dumpOffsets() {
    std::vector<std::string> invalidSignatures;
    std::stringstream header, genFuncSS;

    generateHeaderStart(header);
    generateFunctionStart(genFuncSS);

    for (const auto& ns : signatureNamespaces) {
        processNamespace(ns, header, genFuncSS, invalidSignatures);
    }

    generateHeaderEnd(header, genFuncSS);

    std::string finalString = header.str();
    console::writeln("\n{}", finalString);

    // Display result
    if (!invalidSignatures.empty()) {
        console::writeln("Failed getting offsets, invalid signatures are:");
        for (const std::string& name : invalidSignatures) {
            console::writeln("    {}", name);
        }
    }

    copyToClipboard(finalString);

    return true;
}

//bool mem::dumpOffsets() {
//    MODULEINFO mi = { 0 };
//    if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandleW(nullptr), &mi, sizeof(MODULEINFO))) {
//        util::displayLastError(L"GetModuleInformation Failed");
//        return false;
//    }
//
//    std::vector<std::string> invalidSignatures;
//
//    std::stringstream header, genFuncSS;
//
//    genFuncSS << "    inline void generate() {\n"
//              << "        const uint64_t base = mem::baseAddress();\n\n";
//
//    { // Header
//        header << "#ifndef TDMP_GENERATED_OFFSETS_H\n"
//            << "#define TDMP_GENERATED_OFFSETS_H\n\n"
//            << "#include \"memory/memory.h\"\n\n"
//            << "namespace tdmp::offsets {\n\n";
//
//        for (const auto& ns : signatureNamespaces) {
//            std::string nsName = ns.ns;
//            auto nsSignatures = ns.signatures;
//
//            header << "    namespace " << nsName << " {\n";
//
//            for (const mem::signature& sig : nsSignatures) {
//                const uint64_t address = mem::findIDAPattern(sig.sig, sig.relative);
//                const uint64_t offsetFrombase = address - mem::baseAddress();
//
//                if (!address) {
//                    invalidSignatures.emplace_back((nsName + std::string("::") + sig.name));
//                } else {
//                    genFuncSS << "        " << nsName << "::" << sig.name << " = base + " << nsName << "::" << sig.name << ";\n";
//                }
//
//                console::writeln("{}::{} = {:#x}\n", nsName, sig.name, offsetFrombase);
//
//                std::string offsetStr = std::format("{:#x}", offsetFrombase);
//                header << "        inline uint64_t " << sig.name << " = " << offsetStr << ";\n";
//            }
//
//            header << "    }\n\n";
//        }
//
//        genFuncSS << "    }\n";
//
//        header << genFuncSS.str() << '\n';
//        header << "}\n\n"
//            << "#endif // TDMP_GENERATED_OFFSETS_H";
//    }
//
//    std::string finalString = header.str();
//    HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, finalString.length() + 1);
//    memcpy(GlobalLock(hMem), finalString.data(), finalString.length() + 1);
//    GlobalUnlock(hMem);
//    OpenClipboard(0);
//    EmptyClipboard();
//    SetClipboardData(CF_TEXT, hMem);
//    CloseClipboard();
//    GlobalFree(hMem);
//
//    util::displayMessage(MB_OK, L"Copied generated offsets to clipboard!");
//
//    if (invalidSignatures.size() > 0) {
//        console::writeln("Failed getting offsets, invalid signatures are:");
//        for (const std::string& name : invalidSignatures) {
//            console::writeln("    {}", name.data());
//        }
//    }
//
//    console::writeln("\n{}", finalString.data());
//
//    return true;
//}