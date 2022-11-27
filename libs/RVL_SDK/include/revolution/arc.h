#ifndef ARC_H
#define ARC_H

#include "revolution/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void* archiveStartAddr;     // _0
    void* FSTStart;             // _4
    void* fileStart;            // _8
    u32 entryNum;               // _C
    char* FSTStringStart;       // _10
    u32 FSTLength;              // _14
    u32 currDir;                // _18
} ARCHandle;

#ifdef __cplusplus
}
#endif

#endif // ARC_H