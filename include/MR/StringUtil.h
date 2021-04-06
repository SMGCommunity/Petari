#pragma once

#include <revolution.h>

namespace MR
{
    void removeExtensionString(char *, u32, const char *);

    void addFilePrefix(char *, u32, const char *, const char *);
    void makeDateString(wchar_t *, s32, s32, s32, s32);
    void makeTimeString(wchar_t *, s32, s32, s32);

    void addNumberFontTag(wchar_t *, s32);
    void removeExtensionString(char *, u32, const char *);
    void makeRaceBestTimeString(wchar_t *, s32);
    void makeRaceCurrentTimeString(wchar_t *);
    void copyString(char *, const char *, u32);
    void copyString(wchar_t *, const wchar_t *, u32);

    void addPictureFontCode(wchar_t *, s32);
    void addPictureFontTag(wchar_t *, s32);
    void addPictureFontTagPlayerIcon(wchar_t *);

    void addNewLine(wchar_t *);

    const char* getBasename(const char *);

    bool strcasecmp(const char *, const char *);
    bool isEqualString(const char *, const char *);
    bool isEqualStringCase(const char *, const char *);
    bool isEqualSubString(const char *, const char *);
    bool hasStringSpace(const char *);
    
    const char* extractString(char *, const char *, u32, u32);
};