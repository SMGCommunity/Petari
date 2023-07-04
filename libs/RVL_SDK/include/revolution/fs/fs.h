#ifndef FS_H
#define FS_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef s32 ISFSError;
typedef void (*ISFSCallback) (ISFSError, void *ctxt);

typedef struct {
    u32 blockSize;
    u32 freeBlocks;
    u32 occupiedBlcocks;
    u32 badBlocks;
    u32 reservedBlocks;
    u32 freeInodes;
    u32 occupedInodes;
} ISFSStats;

typedef struct {
    u32 size;
    u32 offset;
} ISFSFileStats;

s32 ISFS_Read(s32, u8 *, u32);

#ifdef __cplusplus
}
#endif

#endif // FS_H