#pragma once

#include <revolution.h>

#ifdef VER_KOR
#define LANG_NUM 0xC
#else
#define LANG_NUM 0xA
#endif

struct GameLanguage {
    int mIdentifier;    // 0x0
    const char* mName;  // 0x4
};

namespace MR {
    u8 getDecidedLanguageFromIPL();
    const char* getCurrentLanguagePrefix();

    int getLangauge();

    extern u8* sUnknownVals;
    extern GameLanguage sLanguages[];
};
