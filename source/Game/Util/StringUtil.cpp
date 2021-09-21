#include "Game/Util/StringUtil.h"

#include "string.h"

namespace MR {
    // missing branch, and wrong condition (likely because of the branch)
    const char* getBasename(const char *pString) {
        const char* str = strrchr(pString, 0x2F); // '/'
        return str ? str + 1 : pString;
    }

    void extractString(char *pDest, const char *pSource, u32 num, /* unused */ u32) {
        strncpy(pDest, pSource, num);
        pDest[num] = 0;
    }
};
