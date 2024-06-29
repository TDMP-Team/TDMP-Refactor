#include "pch.h"
#include "memory/memory.h"
#include "shared/util/util.h"

using namespace tdmp;

uint64_t mem::baseAddress = 0;
uint32_t mem::moduleSize = 0;

// Internal Functions
//------------------------------------------------------------------------
static bool patternCompare(const uint8_t* data, const char* pattern_u8) {
    const char* pattern = pattern_u8;
    while (*pattern) {
        if (*pattern == ' ') {
            ++pattern;
            continue;
        }

        if (*pattern == '?') {
            ++data;

            if (*(pattern + 1) == '?') {
                ++pattern;
            }
        } else {
            char hexStr[3] = { pattern[0], pattern[1], '\0' };
            uint8_t byte = static_cast<uint8_t>(strtol(hexStr, nullptr, 16));
            if (*data != byte) {
                return false;
            }

            ++data;
            ++pattern;
        }

        ++pattern;
    }

    return true;
}

static int64_t readPtr(uint64_t addr, uint32_t offset) {
    int32_t rel_offset = *reinterpret_cast<int32_t*>(addr + offset);
    return static_cast<int64_t>(addr) + rel_offset + sizeof(int32_t) + static_cast<int64_t>(offset);
}

// Public Functions
//------------------------------------------------------------------------
bool mem::initializeMemory() {
    if (baseAddress != 0) return true;

    MODULEINFO mi = { 0 };
    if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandleW(nullptr), &mi, sizeof(MODULEINFO))) {
        return false;
    }

    baseAddress = reinterpret_cast<uint64_t>(mi.lpBaseOfDll);
    moduleSize = mi.SizeOfImage;

    return true;
}

uint64_t mem::findIDAPattern(const char* pattern_u8, bool relative) {
    bool isRelative = relative || (pattern_u8[0] == 'E' && pattern_u8[1] == '8');

    for (size_t i = 0; i < moduleSize - 1; ++i) {
        if (patternCompare(reinterpret_cast<const uint8_t*>(baseAddress + i), pattern_u8)) {
            if (isRelative) {
                return readPtr(baseAddress + i, 1);
            }
            return baseAddress + i;
        }
    }

    return 0;
}

bool mem::isSectionLoaded(HMODULE module, const char* section) {
    MODULEINFO moduleInfo;
    if (!GetModuleInformation(GetCurrentProcess(), module, &moduleInfo, sizeof(moduleInfo))) {
        util::displayLastError(L"Failed to get module information");
        return false;
    }

    IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)moduleInfo.lpBaseOfDll;
    IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS*)((BYTE*)dosHeader + dosHeader->e_lfanew);

    IMAGE_SECTION_HEADER* sectionHeader = IMAGE_FIRST_SECTION(ntHeader);
    for (WORD i = 0; i < ntHeader->FileHeader.NumberOfSections; i++) {
        if (strcmp((char*)sectionHeader[i].Name, section) == 0) {
            DWORD virtualAddress = sectionHeader[i].VirtualAddress;
            DWORD sizeOfRawData = sectionHeader[i].SizeOfRawData;

            // Check if the entire section is readable
            MEMORY_BASIC_INFORMATION mbi;
            SIZE_T result = VirtualQuery((LPCVOID)((BYTE*)moduleInfo.lpBaseOfDll + virtualAddress), &mbi, sizeof(mbi));

            if (result == sizeof(mbi) &&
                mbi.State == MEM_COMMIT &&
                (mbi.Protect == PAGE_READONLY ||
                mbi.Protect == PAGE_READWRITE ||
                mbi.Protect == PAGE_EXECUTE_READ ||
                mbi.Protect == PAGE_EXECUTE_READWRITE)) {
                return true;
            }
            break;
        }
    }

    return false;
}

void mem::waitForSection(HMODULE hModule, const char* sectionName) {
    console::setStatus("Waiting for section: {}", sectionName);

    const int MAX_ATTEMPTS = 100;
    const int SLEEP_DURATION_MS = 50;

    for (int attempt = 0; attempt < MAX_ATTEMPTS; ++attempt) {
        if (isSectionLoaded(hModule, sectionName)) {
            return;
        }

        Sleep(SLEEP_DURATION_MS);
    }

    std::wstring errorMsg = L"Timeout waiting for section " + std::wstring(sectionName, sectionName + strlen(sectionName)) + L" to load";
    util::displayError(errorMsg.c_str());
    ExitProcess(1);
}

bool tdmp::mem::findGameFunctions(std::vector<game_function*>& functions) {
    MODULEINFO modInfo;
    HMODULE hModule = GetModuleHandle(NULL);
    if (!GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO))) {
        util::displayLastError(L"GetModuleInformation Failed");
        return false;
    }

    BYTE* baseAddress = (BYTE*)modInfo.lpBaseOfDll;
    IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)baseAddress;
    IMAGE_NT_HEADERS* ntHeaders = (IMAGE_NT_HEADERS*)(baseAddress + dosHeader->e_lfanew);
    IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION(ntHeaders);

    uintptr_t rdataStart = 0, rdataEnd = 0;
    uintptr_t dataStart = 0, dataEnd = 0;

    for (int i = 0; i < ntHeaders->FileHeader.NumberOfSections; ++i, ++section) {
        std::string sectionName((char*)section->Name);
        if (sectionName == ".rdata") {
            rdataStart = (uintptr_t)(baseAddress + section->VirtualAddress);
            rdataEnd = rdataStart + section->Misc.VirtualSize;
        } else if (sectionName == ".data") {
            dataStart = (uintptr_t)(baseAddress + section->VirtualAddress);
            dataEnd = dataStart + section->Misc.VirtualSize;
        }
    }

    MEMORY_BASIC_INFORMATION memInfo;

    // Scan .rdata section for strings
    for (uintptr_t addr = rdataStart; addr < rdataEnd; addr += memInfo.RegionSize) {
        if (!VirtualQuery((LPCVOID)addr, &memInfo, sizeof(memInfo)) || memInfo.State != MEM_COMMIT || memInfo.Protect == PAGE_NOACCESS)
            continue;

        std::vector<char> buffer(memInfo.RegionSize);
        SIZE_T bytesRead;
        if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)addr, buffer.data(), memInfo.RegionSize, &bytesRead))
            continue;

        for (auto& func : functions) {
            auto it = std::search(buffer.begin(), buffer.begin() + bytesRead, func->name.begin(), func->name.end());
            if (it != buffer.begin() + bytesRead) {
                uintptr_t stringAddress = addr + std::distance(buffer.begin(), it);
                func->address = stringAddress; // Temporarily store the string address
            }
        }
    }

    // Scan .data section for references to the strings
    for (uintptr_t addr = dataStart; addr < dataEnd; addr += memInfo.RegionSize) {
        if (!VirtualQuery((LPCVOID)addr, &memInfo, sizeof(memInfo)) || memInfo.State != MEM_COMMIT || memInfo.Protect == PAGE_NOACCESS)
            continue;

        std::vector<uintptr_t> buffer(memInfo.RegionSize / sizeof(uintptr_t));
        SIZE_T bytesRead;
        if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)addr, buffer.data(), memInfo.RegionSize, &bytesRead))
            continue;

        for (auto& func : functions) {
            if (func->address == 0)
                continue;

            for (size_t i = 0; i < buffer.size(); ++i) {
                if (buffer[i] == func->address) {
                    uintptr_t potentialFuncAddr = addr + (i - 1) * sizeof(uintptr_t);
                    uintptr_t funcAddr;
                    if (ReadProcessMemory(GetCurrentProcess(), (LPCVOID)potentialFuncAddr, &funcAddr, sizeof(funcAddr), &bytesRead)) {
                        func->address = funcAddr;
                    }
                }
            }
        }
    }

    return true;
}
