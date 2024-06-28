#ifndef TDMP_UTIL_H
#define TDMP_UTIL_H

#include "pch.h"
#include "shared/util/util.h"

namespace tdmp::util {

    static void copyToClipboard(const std::string& text) {
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.length() + 1);
        if (!hMem) {
            tdmp::util::displayLastError(L"copyToClipboard failed, could not allocate memory");
            return;
        }

        LPVOID locked = GlobalLock(hMem);
        if (!locked) {
            tdmp::util::displayLastError(L"Could not lock memory in copyToClipboard");
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

        tdmp::util::displayMessage(MB_OK, L"Copied generated offsets to clipboard!");
    }

}

#endif // TDMP_UTIL_H