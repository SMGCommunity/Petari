#include "Game/Util/StringUtil.h"

// the linker will fix me when we are linking to the codewarrior libs
#ifdef __cplusplus
extern "C" {
extern int strcasecmp(const char*, const char*);
};
#endif

#include "string.h"

namespace MR { 
    #ifdef NON_MATCHING
    // missing branch, and wrong condition (likely because of the branch)
    const char* getBasename(const char *pString) {
        const char* str = strrchr(pString, 0x2F); // '/'
        return str ? str + 1 : pString;
    }
    #endif

    void extractString(char *pDest, const char *pSource, u32 num, /* unused */ u32) {
        strncpy(pDest, pSource, num);
        pDest[num] = 0;
    }

    int strcasecmp(const char *pStr1, const char *pStr2) {
        return strcasecmp(pStr1, pStr2);
    }

    bool isEqualString(const char *pStr1, const char *pStr2) {
        return !strcmp(pStr1, pStr2);
    }

    bool isEqualStringCase(const char *pStr1, const char *pStr2) {
        return !strcasecmp(pStr1, pStr2);
    }

    bool isEqualSubString(const char *pStr1, const char *pStr2) {
        return strstr(pStr1, pStr2) != 0;
    }

    bool hasStringSpace(const char *pString) {
        return strchr(pString, ' ') != 0;
    }
};
