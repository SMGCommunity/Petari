#include "Game/Player/GhostPacket.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include <cstdio>

GhostPacket::GhostPacket(void* pData, u32 len) {
    mDataPtr = (u8*)pData;
    mCurOffs = 0;
    _C = len;
}

void GhostPacket::read(u8* pOut, u32 len) {
    for (; len != 0; len--) {
        s32 offs = mCurOffs;
        *pOut++ = mDataPtr[offs++];
        mCurOffs++;
    }
}

void GhostPacket::read(u32* pOut) {
    read((u8*)pOut, 4);
}

void GhostPacket::read(s16* pOut) {
    read((u8*)pOut, 2);
}

void GhostPacket::read(char** pOut) {
    char* v3 = (char*)&mDataPtr[mCurOffs];
    *pOut = (char*)v3;
    s32 offs = strlen(v3) + 1;
    mCurOffs += offs;
}

void GhostPacket::read(s8* pOut) {
    read((u8*)pOut, 1);
}

void GhostPacket::read(TVec3Sc* pOut) {
    read((u8*)&pOut->x, 1);
    read((u8*)&pOut->y, 1);
    read((u8*)&pOut->z, 1);
}

void GhostPacket::read(TVec3s* pOut) {
    read((u8*)&pOut->x, 2);
    read((u8*)&pOut->y, 2);
    read((u8*)&pOut->z, 2);
}
