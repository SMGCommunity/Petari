#include "Game/Player/GhostPacket.h"
#include "string.h"

GhostPacket::GhostPacket(void *pSrc, u32 unk)
    : mSource(pSrc), mCurPos(0), _C(unk)
{ }

void GhostPacket::read(u8 *out, u32 numBytes)
{
    for (u32 i = 0; i < numBytes; i++)
    {
        *out++ = static_cast<s8*>(mSource)[mCurPos];
        mCurPos++;
    }
}

void GhostPacket::read(u32 *out)
{
    read((u8*)out, 4);
}

void GhostPacket::read(s16 *out)
{
    read((u8*)out, 4);
}

void GhostPacket::read(char **out)
{
    *out = static_cast<char*>(mSource) + mCurPos;
    s32 length = strlen(*out) + 1; // add one for the null terminator
    mCurPos = length + mCurPos;
}

void GhostPacket::read(s8 *out)
{
    read(out);
}

void GhostPacket::read(JGeometry::TVec3<s8> *out)
{
    read((u8*)&out->x, 1);
    read((u8*)&out->y, 1);
    read((u8*)&out->z, 1);
}

void GhostPacket::read(JGeometry::TVec3<s16> *out)
{
    read((u8*)&out->x, 2);
    read((u8*)&out->y, 2);
    read((u8*)&out->z, 2);
}