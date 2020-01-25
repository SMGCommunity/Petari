#ifndef ARC_H
#define ARC_H

#include "types.h"

typedef struct
{
    void* mArchiveStartAddr; // _0
    void* mFilesystemStart; // _4
    void* mFileStart; // _8
    u32 mEntryNum; // _C
    char* mFSTStringStart; // _10
    u32 mLength; // _14
    u32 mCurrentDir; // _18
} ARCHandle;

typedef struct
{
    ARCHandle* mHandle; // _0
    u32 mStartOffset; // _4
    u32 mLength; // _8
} ARCFileInfo;

typedef struct
{
    ARCHandle* mHandle; // _0
    u32 mEntryNum; // _4
    u32 mLocation; // _8
    u32 mNextDir; // _C
} ARCDir;

typedef struct 
{
    ARCHandle* mHandle; // _0
    u32 mEntryNum; // _4
    u32 mIsDir; // _8
    char* mName; // _C
} ARCDirEntry;

s32 ARCConvertPathToEntrynum(ARCHandle *, const char *);
void* ARCGetStartAddrInMem(ARCFileInfo *);
u32 ARCGetLength(ARCFileInfo *);
u32 ARCClose(ARCFileInfo *);
u32 ARCChangeDir(ARCHandle *, const char *);

#endif // ARC_H