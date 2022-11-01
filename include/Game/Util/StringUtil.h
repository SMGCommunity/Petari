#pragma once

#include "revolution.h"

namespace MR {
    const char* getBasename(const char *);
    void extractString(char *, const char *, u32, u32);

    int strcasecmp(const char *, const char *);
    bool isEqualString(const char *, const char *);
    bool isEqualStringCase(const char *, const char *);
    bool isEqualSubString(const char *, const char *);
    bool hasStringSpace(const char *);

    void addFilePrefix(char *, u32, const char *, const char *);

    void copyString(char *, const char *, u32);
    void copyString(wchar_t *, const wchar_t *, u32);
    void addPictureFontCode(wchar_t *, int);
    void addPictureFontTag(wchar_t *, int);
    void addPictureFontTagPlayerIcon(wchar_t *);
    void addNewLine(wchar_t *);

    bool isNullOrEmptyString(const char *);

    void scan32(const char *, const char *, s32 *);
    void scan16(const char *, const char *, u16 *);
    void scan8(const char *, const char *, u8 *);
    void scanf32(const char *, const char *, f32 *);
    void scanu8x4(const char *, const char *, u8 *);
    void scans16x4(const char *, const char *, s16 *);
    void scanf32x4(const char *, const char *, f32 *);
};