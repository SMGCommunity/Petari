#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <revolution.h>

namespace MR
{
    void addNumberFontTag(wchar_t *, s32);
    void removeExtensionString(char *, u32, const char *);
    void makeRaceBestTimeString(wchar_t *, s32);
    void makeRaceCurrentTimeString(wchar_t *);
    void copyString(char *, const char *, u32);
    void copyString(wchar_t *, const wchar_t *, u32);

    bool isEqualString(const char *, const char *);
    bool isEqualStringCase(const char *, const char *);
    bool isEqualSubString(const char *, const char *);
};

#endif // STRINGUTIL_H