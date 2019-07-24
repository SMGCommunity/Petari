#include "MR/util.h"

namespace MR
{
    s32 getHashCode(const char *str)
    {
        s32 hash = 0;

        while (*str)
        {
            hash = (hash * 0x1f) + *str++;
        }

        return hash;
    }
};
