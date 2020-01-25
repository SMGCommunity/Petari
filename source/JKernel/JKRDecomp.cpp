#include "JKernel/JKRDecomp.h"

void JKRDecomp::decode(u8 *pIn, u8 *pOut, u32 unk1, u32 unk2)
{
    s32 decompType = checkCompressed(pIn);

    if (decompType == 1)
    {
        decodeSZP(pIn, pOut, unk1, unk2);
    }
    else if (decompType == 2)
    {
        decodeSZS(pIn, pOut, unk1, unk2);
    }
}

void JKRDecomp::decodeSZS(u8 *pIn, u8 *pOut, u32 unk1, u32 unk2)
{
    s32 loopVal = 0;
    u32 curBlock;
    s32 totalSize = unk2 - ((s32)pOut + *(u32*)(pIn + 4));

    if (unk1 == 0)
    {
        return;
    }

    if (unk2 > *(u32*)pIn)
    {
        return;
    }

    pIn += 0x10;

    do
    {
        if (loopVal == 0)
        {
            curBlock = *pIn;
            loopVal = 8;
            pIn++;
        }

        if ((curBlock & 0x80) != 0)
        {
            if (unk2 == 0)
            {
                unk1--;
                *pOut++ = *pIn;

                if (unk1 == 0)
                {
                    return;
                }
            }
            else
            {
                unk2--;
            }

            pIn++;
        }
        else
        {
            u8 val1 = *pIn++;
            u8 val2 = *pIn++;

            s32 numBytes = val1 >> 4;
            u8* copyLoc = (u8*)pOut;
            copyLoc -= ((val1 & 0xF) << 8) | val2;

            if (numBytes == 0)
            {
                numBytes = *pIn++ + 0x12;
            }
            else
            {
                numBytes += 0x2;
            }
            
            do
            {
               if (unk2 == 0)
                {
                    unk1--;
                    *pOut = *(copyLoc - 1);
                    pOut++;

                    if (unk1 == 0)
                    {
                        return;
                    }
                }
                else
                {
                    unk2--;
                }

                copyLoc--;

            } while (--numBytes != 0);
        }

        curBlock <<= 1;
        loopVal--;

    } while ((s32)pOut != totalSize);
}

s32 JKRDecomp::checkCompressed(u8 *pSrc)
{
    // Return values:
    // 0 -- Not Compressed
    // 1 -- Yay0
    // 2 -- Yaz0
    // 3 -- ASR
    if (pSrc[0] == 0x59 && pSrc[1] == 0x61 && pSrc[3] == 0x30)
    {
        if (pSrc[2] == 0x79)
        {
            return 1;
        }

        if (pSrc[2] == 0x7A)
        {
            return 2;
        }
    }

    if (pSrc[0] == 0x41 && pSrc[1] == 0x53 && pSrc[2] == 0x52)
    {
        return 3;
    }
    
    return 0;
}