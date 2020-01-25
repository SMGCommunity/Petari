#ifndef DVD_H
#define DVD_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DVDFileInfo DVDFileInfo;
typedef struct FSTEntryNode FSTEntryNode;
typedef void (*DVDCallback)(s32 result, DVDFileInfo* fileInfo);

struct DVDFileInfo
{
    u32 mStartAdress;
    u32 _4;
    u32 _8;
    u32 _C;
    u32 _10;
    u32 _14;
    u32 _18;
    u32 _1C;
    u32 _20;
    u32 _24;
    u32 _28;
    u32 _2C;
    u32 _30;
    u32 _34;
    u32 _38;
};

struct FSTEntryNode
{
    // First 8 bits: Node Type
    // Remaining 24: String Table Offset
    u32 mData;

    union
    {
        u32 mMember_0;
        u32 mMember_1;
    };
    
};

s32 DVDFastOpen(s32, DVDFileInfo *);

s32 DVDConvertPathToEntrynum(const char *);

static char* currentDirectory;
static s32 MaxEntryNum;
static s32 FstStringStart;
static FSTEntryNode* FstStart;
static s32  __DVDLongFileNameFlag = 1;
static s32 __DVDLayoutFormat;

#ifdef __cplusplus
}
#endif

#endif // DVD_H