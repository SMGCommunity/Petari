#include "MR/StringUtil.h"
#include <cstring>
#include <wchar.h>
#include <stdio.h>

namespace MR
{
    /*void removeExtensionString(char *pOut, u32 len, const char *pSrc)
    {
        snprintf(pOut, len, "%s", pSrc);
        char* fuck = std::strrchr(pOut, '.');
        char* shit = std::strrchr(pOut, '/');

        if (fuck >= shit)
        {
            if (shit++ != fuck)
            {
                *fuck = 0;
            }
        }
    }*/

    void copyString(char *dest, const char *src, u32 len)
    {
        std::strncpy(dest, src, len);
    }

    void copyString(wchar_t *dest, const wchar_t *src, u32 len)
    {
        wcsncpy(dest, src, len);
    }

    /*const char* getBasename(const char *pName)
    {
        const char* cur = pName;
        const char* res = std::strrchr(pName, '/');

        if (res != NULL)
        {
            cur = res++;
        }
        
        return cur;
    }*/

    bool isEqualString(const char *str1, const char *str2)
    {
        return !(std::strcmp(str1, str2));
    }

    /*bool isEqualStringCase(const char *str1, const char *str2)
    {
        return !(strcasecmp(str1, str2));
    }*/

    bool isEqualSubString(const char *str1, const char *str2)
    {
        return (std::strstr(str1, str2));
    }

    bool hasStringSpace(const char *str)
    {
        return (std::strstr(str, " "));
    }
};