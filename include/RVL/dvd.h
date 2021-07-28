#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DVDDiskID DVDDiskID;

struct DVDDiskID
{
    char mName[0x4]; // _0
    char mCompany[0x2]; // _4
    u8 mDiskNum; // _6
    u8 mGameVer; // _7
    u8 _8;
    u8 _9;
    u8 _A[0xE];
    u32 _18;
    u32 _1C;
};

#ifdef __cplusplus
}
#endif