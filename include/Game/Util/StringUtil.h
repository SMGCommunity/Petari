#pragma once

#include <revolution.h>

//#include "string.h"

namespace MR {
    const char* getBasename(const char *);
    void extractString(char *, const char *, u32, u32);

    int strcasecmp(const char *, const char *);
    bool isEqualString(const char *, const char *);
    bool isEqualStringCase(const char *, const char *);
    bool isEqualSubString(const char *, const char *);
    bool hasStringSpace(const char *);

    void addFilePrefix(char *, u32, const char *, const char *);
};