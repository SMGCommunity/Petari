#include "Util/HashUtil.h"

namespace MR
{
    s32 getHashCode(const char *pStr)
    {
        s32 hash = 0;

        while (*pStr)
        {
            hash = (hash * 0x1F) + *pStr;
            pStr++;
        }

        return hash;
    }
};