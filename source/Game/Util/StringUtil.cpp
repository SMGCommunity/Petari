#include "Game/Util/StringUtil.h"

// the linker will fix me when we are linking to the codewarrior libs
#ifdef __cplusplus
extern "C" {
extern int strcasecmp(const char*, const char*);
};
#endif

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

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

    void scan32(const char *pOut, const char *a2, s32 *pVal) {
        if (!strstr(pOut, a2)) {
            return;
        }

        sscanf(pOut, "\t%d", pVal);
    }

    void scan16(const char *pOut, const char *a2, u16 *pVal) {
        if (!strstr(pOut, a2)) {
            return;
        }

        u32 out;
        sscanf(pOut, "\t%d", &out);
        *pVal = out;
    }

    void scan8(const char *pOut, const char *a2, u8 *pVal) {
        if (!strstr(pOut, a2)) {
            return;
        }

        u32 out;
        sscanf(pOut, "\t%d", &out);
        *pVal = out;
    }

    void scanf32(const char *pOut, const char *a2, f32 *pVal) {
        if (!strstr(pOut, a2)) {
            return;
        }

        sscanf(pOut, "\t%ff", pVal);
    }

    void scanu8x4(const char *pStr, const char *a2, u8 *pVal) {
        if (!strstr(pStr, a2)) {
            return;
        }

        u32 vars[4];
        sscanf(pStr, "\t{%d,%d,%d,%d}", &vars[0], &vars[1], &vars[2], &vars[3]);

        for (int i = 0; i < 4; i++) {
            pVal[i] = vars[i];
        }
    }

    void scans16x4(const char *pStr, const char *a2, s16 *pVal) {
        if (!strstr(pStr, a2)) {
            return;
        }

        u32 vars[4];
        sscanf(pStr, "\t{%d,%d,%d,%d}", &vars[0], &vars[1], &vars[2], &vars[3]);

        for (int i = 0; i < 4; i++) {
            pVal[i] = vars[i];
        }
    }

    void scanf32x4(const char *pStr, const char *a2, f32 *pVal) {
        if (!strstr(pStr, a2)) {
            return;
        }

        sscanf(pStr, "\t{%ff,%ff,%ff,%ff}", &pVal[0], &pVal[1], &pVal[2], &pVal[3]);
    }
};
