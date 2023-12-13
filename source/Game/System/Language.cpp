#include "Game/System/Language.hpp"

#include <revolution/sc.h>

namespace MR {
    u8 getDecidedLanguageFromIPL() {
        u8 lang = SCGetLanguage();

        u8 maxVal = 0xA;

        if (lang <= 0xA) {
            maxVal = lang;
        }

        return sUnknownVals[maxVal + 0x28];
    }

    const char* getCurrentLanguagePrefix() {
        int lang = getLangauge();

        int curLang = 0;

        while (curLang != LANG_NUM) {
            if (lang == sLanguages[curLang].mIdentifier) {
                return sLanguages[curLang].mName;
            }

            curLang++;
        }

        return "UsEnglish";
    }
}