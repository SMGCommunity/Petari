#ifndef RVL_SDK_NWC24_FILE_API_H
#define RVL_SDK_NWC24_FILE_API_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {

    NWC24_OPEN_WRITE = 1 << 0,
    NWC24_OPEN_READ = 1 << 1,
    NWC24_OPEN_RW = 1 << 2,
    NWC24_OPEN_BUFF = 1 << 3,

    NWC24_OPEN_NAND = 0,
    NWC24_OPEN_VF = 1 << 8,

    NWC24_OPEN_NAND_W = NWC24_OPEN_NAND | NWC24_OPEN_WRITE,
    NWC24_OPEN_NAND_R = NWC24_OPEN_NAND | NWC24_OPEN_READ,
    NWC24_OPEN_NAND_RW = NWC24_OPEN_NAND | NWC24_OPEN_RW,

    NWC24_OPEN_VF_W = NWC24_OPEN_WRITE | NWC24_OPEN_VF,
    NWC24_OPEN_VF_R = NWC24_OPEN_READ | NWC24_OPEN_VF,
    NWC24_OPEN_VF_RW = NWC24_OPEN_RW | NWC24_OPEN_VF,

    NWC24_OPEN_NAND_WBUFF = NWC24_OPEN_NAND_W | NWC24_OPEN_BUFF,
    NWC24_OPEN_NAND_RBUFF = NWC24_OPEN_NAND_R | NWC24_OPEN_BUFF,
    NWC24_OPEN_NAND_RWBUFF = NWC24_OPEN_NAND_RW | NWC24_OPEN_BUFF,

    NWC24_OPEN_VF_WBUFF = NWC24_OPEN_VF_W | NWC24_OPEN_BUFF,
    NWC24_OPEN_VF_RBUFF = NWC24_OPEN_VF_R | NWC24_OPEN_BUFF,
    NWC24_OPEN_VF_RWBUFF = NWC24_OPEN_VF_RW | NWC24_OPEN_BUFF,
} NWC24OpenMode;

typedef enum {
    NWC24_SEEK_BEG,
    NWC24_SEEK_CUR,
    NWC24_SEEK_END,
} NWC24SeekMode;

NWC24Err NWC24FOpen(NWC24File* pFile, const char* pPath, u32 mode);
NWC24Err NWC24FClose(NWC24File* pFile);

NWC24Err NWC24FSeek(NWC24File* pFile, s32 offset, NWC24SeekMode whence);
NWC24Err NWC24FRead(void* pDst, s32 size, NWC24File* pFile);
NWC24Err NWC24FWrite(const void* pSrc, s32 size, NWC24File* pFile);
NWC24Err NWC24FGetLength(NWC24File* pFile, u32* pLength);

NWC24Err NWC24FDeleteVF(const char* pPath);
NWC24Err NWC24MountVF(const char* pDrive, const char* pFileName);
NWC24Err NWC24UnmountVF(const char* pDrive);
NWC24Err NWC24CheckSizeVF(const char* pDrive, u32* pSize);

#ifdef __cplusplus
}
#endif
#endif
