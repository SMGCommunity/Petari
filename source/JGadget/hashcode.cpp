#include "JGadget/hashcode.h"

namespace JGadget
{
    // todo -- does not match
    u16 getHashCode(const char *pStr)
    {
        u16 i = 0;
        char curChar;

        for (;; i = curChar + 0x1F * i)
        {
            curChar = *pStr;

            if (!*pStr)
            {
                break;
            }

            ++pStr;
        }

        return i;
    }
};