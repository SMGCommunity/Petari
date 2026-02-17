#ifndef NWC24FILEAPI_H
#define NWC24FILEAPI_H

#include "revolution/nwc24.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    // Access
    NWC24_OPEN_WRITE = (1 << 0),
    NWC24_OPEN_READ = (1 << 1),
    NWC24_OPEN_RW = (1 << 2),

    // Domain
    NWC24_OPEN_BUFF = (1 << 3),
    NWC24_OPEN_VF = (1 << 8),

    // NAND presets
    NWC24_OPEN_NAND_W = NWC24_OPEN_WRITE,
    NWC24_OPEN_NAND_R = NWC24_OPEN_READ,
    NWC24_OPEN_NAND_RW = NWC24_OPEN_RW,

    // VF presets
    NWC24_OPEN_VF_W = NWC24_OPEN_WRITE | NWC24_OPEN_VF,
    NWC24_OPEN_VF_R = NWC24_OPEN_READ | NWC24_OPEN_VF,
    NWC24_OPEN_VF_RW = NWC24_OPEN_RW | NWC24_OPEN_VF,

    // NAND (buffered) presets
    NWC24_OPEN_NAND_WBUFF = NWC24_OPEN_NAND_W | NWC24_OPEN_BUFF,
    NWC24_OPEN_NAND_RBUFF = NWC24_OPEN_NAND_R | NWC24_OPEN_BUFF,
    NWC24_OPEN_NAND_RWBUFF = NWC24_OPEN_NAND_RW | NWC24_OPEN_BUFF,

    // VF (buffered) presets
    NWC24_OPEN_VF_WBUFF = NWC24_OPEN_VF_W | NWC24_OPEN_BUFF,
    NWC24_OPEN_VF_RBUFF = NWC24_OPEN_VF_R | NWC24_OPEN_BUFF,
    NWC24_OPEN_VF_RWBUFF = NWC24_OPEN_VF_RW | NWC24_OPEN_BUFF,
} NWC24OpenMode;

typedef enum {
    NWC24_SEEK_BEG,
    NWC24_SEEK_CUR,
    NWC24_SEEK_END,
} NWC24SeekMode;

typedef struct NWC24File {
    u32 id;              // at 0x0
    u32 mode;            // at 0x4
    u32 align;           // at 0x8
    NANDFileInfo nandf;  // at 0xC
    void* vff;           // at 0x98
} NWC24File;

NWC24Err NWC24FOpen(NWC24File* file, const char* path, u32 mode);
NWC24Err NWC24iFOpenNand(NWC24File* file, const char* path, u32 mode);
NWC24Err NWC24iFOpenVF(NWC24File* file, const char* path, u32 mode);

NWC24Err NWC24FClose(NWC24File* file);
NWC24Err NWC24iFCloseNand(NWC24File* file) NO_INLINE;
NWC24Err NWC24iFCloseVF(NWC24File* file);

NWC24Err NWC24FSeek(NWC24File* file, s32 offset, NWC24SeekMode whence);
NWC24Err NWC24FRead(void* dst, s32 size, NWC24File* file);
NWC24Err NWC24FWrite(const void* src, s32 size, NWC24File* file);
NWC24Err NWC24FGetLength(NWC24File* file, u32* lengthOut);
NWC24Err NWC24FDeleteVF(const char* path);
NWC24Err NWC24MountVF(const char* drive, const char* filename);
NWC24Err NWC24UnmountVF(const char* drive);
NWC24Err NWC24CheckSizeVF(const char* drive, u32* sizeOut);

#ifdef __cplusplus
}
#endif

#endif  // NWC24FILEAPI_H
