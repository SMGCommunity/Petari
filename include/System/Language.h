#pragma once

#include <revolution.h>

const int LANGUAGE_NUM = 0xC;

struct LanguageEntry
{
    u32 mLanguageID;
    const char* mLanguageString;
};

namespace
{
    const LanguageEntry cLanguages[LANGUAGE_NUM] =
    {
        { 0x10, "JpJapanese" },
        { 0x21, "UsEnglish" },
        { 0x24, "UsSpanish" },
        { 0x23, "UsFrench" },
        { 0x1, "EuEnglish" },
        { 0x4, "EuSpanish" },
        { 0x3, "EuFrench" },
        { 0x2, "EuGerman" },
        { 0x5, "EuItalian" },
        { 0x6, "EuDutch" },
        { 0x37, "CnSimpChinese" },
        { 0x49, "KrKorean" }
    };
};

namespace MR
{
    u8 getDecidedLanguageFromIPL();
    u32 getLanguage();
    u32 getLanguageFromIPL();
    const char* getCurrentLanguagePrefix();
    const char* getCurrentRegionPrefix();
    u32 getLanguageNum();
    const char* getLanguagePrefixByIndex(u32);
};