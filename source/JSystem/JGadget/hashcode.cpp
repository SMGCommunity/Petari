#include "JSystem/JGadget/hashcode.h"

namespace JGadget
{
    u32 getHashCode(const register char *pStr)
    {
        __asm
        {
            li r5, 0
            li r0, 0

            b loadChar

            addToHash:
                mulli r4, r5, 0x1F
                extsb r5, r6
                addi pStr, pStr, 1
                add r5, r5, r4

            loadChar:
                lbz r6, 0(pStr)
                extsb r4, r6
                cmpw r4, r0
                bne addToHash
                mr r3, r5
                blr
        }
    }
};