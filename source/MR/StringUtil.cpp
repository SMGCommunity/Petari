#include "MR/StringUtil.h"
#include "smg.h"
#include <cstring>
#include "extras.h"
#include "wstring.h"

namespace MR
{

    void copyString(char *dest, const char *src, u32 len)
    {
        std::strncpy(dest, src, len);
    }

    void copyString(wchar_t *dest, const wchar_t *src, u32 len)
    {
        std::wcsncpy(dest, src, len);
    }

    bool isEqualString(const char *str1, const char *str2)
    {
        return !(std::strcmp(str1, str2));
    }

    bool isEqualSubString(const char *str1, const char *str2)
    {
        return (std::strstr(str1, str2));
    }

    bool hasStringSpace(const char *str)
    {
        return (std::strstr(str, " "));
    }
};