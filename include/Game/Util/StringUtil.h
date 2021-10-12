#pragma once

#include <revolution.h>

//#include "string.h"

namespace MR {
    const char* getBasename(const char *);
    void extractString(char *, const char *, u32, u32);

    bool isEqualString(const char *, const char *);
};