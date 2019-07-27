#include "MR/HashUtil.h"

namespace MR
{
    s32 getHashCode(const char *str)
    {
        s32 hash = 0;

        while (*str)
        {
            hash = (hash * 0x1F) + *str;
            str++;
        }

        return hash;
    }
};