#ifndef ARC_H
#define ARC_H

#include "revolution/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned int magic;
    int fstStart;
    int fstSize;
    int fileStart;
    int pad[4];
} ARCHeader;

typedef struct {
    void* archiveStartAddr;     // _0
    void* FSTStart;             // _4
    void* fileStart;            // _8
    u32 entryNum;               // _C
    char* FSTStringStart;       // _10
    u32 FSTLength;              // _14
    u32 currDir;                // _18
} ARCHandle;

typedef struct {
    ARCHandle* handle;
    u32 startOffset;
    u32 length;
} ARCFileInfo;

typedef struct {
    ARCHandle* handle;
    u32 entryNum;
    u32 location;
    u32 next;
} ARCDir;

typedef struct  {
    ARCHandle* handle;
    u32 entryNum;
    BOOL isDir;
    char* name;
} ARCDirEntry;

s32 ARCConvertPathToEntrynum(ARCHandle *, const char *);
BOOL ARCGetCurrentDir(ARCHandle *, char *, u32);

#ifdef __cplusplus
}
#endif

#endif // ARC_H