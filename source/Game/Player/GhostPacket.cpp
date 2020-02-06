#include "Game/Player/GhostPacket.h"
#include "string.h"

GhostPacket::GhostPacket(void *pSrc, u32 unk)
    : mSource(pSrc), mCurPos(0), _C(unk)
{ }

void GhostPacket::read(u8 *pOut, u32 numBytes)
{
    for (u32 i = 0; i < numBytes; i++)
    {
        *pOut++ = static_cast<s8*>(mSource)[mCurPos];
        mCurPos++;
    }
}

void GhostPacket::read(u32 *pOut)
{
    read((u8*)pOut, 4);
}

void GhostPacket::read(s16 *pOut)
{
    read((u8*)pOut, 4);
}

void GhostPacket::read(char **pOut)
{
    *pOut = static_cast<char*>(mSource) + mCurPos;
    s32 length = strlen(*pOut) + 1; // add one for the null terminator
    mCurPos = length + mCurPos;
}

void GhostPacket::read(s8 *pOut)
{
    read(pOut);
}

void GhostPacket::read(JGeometry::TVec3<s8> *pOut)
{
    read((u8*)&pOut->x, 1);
    read((u8*)&pOut->y, 1);
    read((u8*)&pOut->z, 1);
}

void GhostPacket::read(JGeometry::TVec3<s16> *pOut)
{
    read((u8*)&pOut->x, 2);
    read((u8*)&pOut->y, 2);
    read((u8*)&pOut->z, 2);
}