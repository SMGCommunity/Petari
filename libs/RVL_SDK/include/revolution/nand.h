#ifndef NAND_H
#define NAND_H

#include <revolution.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NANDFileInfo
{
    s32  fileDescriptor;
    s32  origFd;
    char origPath[64];
    char tmpPath[64];
    u8   accType;
    u8   stage;
    u8   mark;
} NANDFileInfo;

s32 NANDCreate(const char *, u8, u8);

s32 NANDOpen(const char *, NANDFileInfo *, u8);

s32 NANDClose(NANDFileInfo *);

s32 NANDRead(NANDFileInfo *, void *, u32);

s32 NANDGetLength(NANDFileInfo *, u32 *);

s32 NANDDelete(const char *);

s32 NANDMove(const char *, const char *);

s32 NANDCheck(u32, u32, u32 *);

s32 NANDWrite(NANDFileInfo *, const void *, u32);

#ifdef __cplusplus
}
#endif

#endif // NAND_H