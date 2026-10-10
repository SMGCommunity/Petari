#ifndef ARC_H
#define ARC_H

#include "revolution/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    /* 0x00 */ unsigned int magic;
    /* 0x04 */ int fstStart;
    /* 0x08 */ int fstSize;
    /* 0x0C */ int fileStart;
    /* 0x10 */ int pad[4];
} ARCHeader;

typedef struct {
    /* 0x00 */ void* archiveStartAddr;
    /* 0x04 */ void* FSTStart;
    /* 0x08 */ void* fileStart;
    /* 0x0C */ u32 entryNum;
    /* 0x10 */ char* FSTStringStart;
    /* 0x14 */ u32 FSTLength;
    /* 0x18 */ u32 currDir;
} ARCHandle;

typedef struct {
    /* 0x0 */ ARCHandle* handle;
    /* 0x4 */ u32 startOffset;
    /* 0x8 */ u32 length;
} ARCFileInfo;

typedef struct {
    /* 0x0 */ ARCHandle* handle;
    /* 0x4 */ u32 entryNum;
    /* 0x8 */ u32 location;
    /* 0xC */ u32 next;
} ARCDir;

typedef struct {
    /* 0x0 */ ARCHandle* handle;
    /* 0x4 */ u32 entryNum;
    /* 0x8 */ BOOL isDir;
    /* 0xC */ char* name;
} ARCDirEntry;

BOOL ARCInitHandle(void*, ARCHandle*);
BOOL ARCFastOpen(ARCHandle*, s32, ARCFileInfo*);
s32 ARCConvertPathToEntrynum(ARCHandle*, const char*);
void* ARCGetStartAddrInMem(ARCFileInfo*);
u32 ARCGetLength(ARCFileInfo*);
BOOL ARCClose(ARCFileInfo*);
BOOL ARCChangeDir(ARCHandle*, const char*);
BOOL ARCGetCurrentDir(ARCHandle*, char*, u32);

BOOL ARCOpenDir(ARCHandle*, const char*, ARCDir*);
BOOL ARCReadDir(ARCDir*, ARCDirEntry*);
BOOL ARCCloseDir(ARCDir*);

#ifdef __cplusplus
}
#endif

#endif  // ARC_H
