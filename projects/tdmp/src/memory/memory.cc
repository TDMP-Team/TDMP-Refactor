#include "pch.h"
#include "memory/memory.h"
#include "util/util.h"

using namespace tdmp;

static uint64_t base = 0;
static uint32_t moduleSize = 0;

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
	if (base != 0) return true;

	MODULEINFO mi = { 0 };
	if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandleW(nullptr), &mi, sizeof(MODULEINFO))) {
		return false;
	}

	base = reinterpret_cast<uint64_t>(mi.lpBaseOfDll);
	moduleSize = mi.SizeOfImage;

	return true;
}

uint64_t mem::findIDAPattern(const char* pattern_u8, bool relative) {
	bool isRelative = relative || (pattern_u8[0] == 'E' && pattern_u8[1] == '8');

	for (size_t i = 0; i < moduleSize - 1; ++i) {
		if (patternCompare(reinterpret_cast<const uint8_t*>(base + i), pattern_u8)) {
			if (isRelative) {
				return readPtr(base + i, 1);
			}
			return base + i;
		}
	}

	return 0;
}

uint64_t mem::baseAddress() {
	return base;
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