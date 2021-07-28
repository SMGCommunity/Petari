#pragma once

#include "RVL/dvd.h"

typedef struct OSBootInfo_
{
    DVDDiskID mDiskID; // _0
    u32 mMagic;        // _20
    u32 mVersion;      // _24
    u32 _28;
    u32 _2C;
    void* mArenaLo;    // _30
    void* mArenaHi;    // _34
    void* _38;
    u32 _3C;
} OSBootInfo;