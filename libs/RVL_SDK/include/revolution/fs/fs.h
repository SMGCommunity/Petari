#ifndef FS_H
#define FS_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

s32 ISFS_Read(s32, u8 *, u32);

#ifdef __cplusplus
}
#endif

#endif // FS_H