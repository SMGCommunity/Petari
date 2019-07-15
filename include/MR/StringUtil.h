#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include "types.h"

namespace MR
{
    void addNumberFontTag(wchar_t *, s32);
    void removeExtensionString(char *, u64, const char *);
    void makeRaceBestTimeString(wchar_t *, s32);
    void makeRaceCurrentTimeString(wchar_t *);
    void copyString(char *, const char *, u64);
    void copyString(wchar_t *, const wchar_t *, u64);
};

#endif // STRINGUTIL_H