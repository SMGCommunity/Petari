#include "revolution/arc.h"

void* ARCGetStartAddrInMem(ARCFileInfo *pInfo)
{
    return (u8*)pInfo->mHandle->mArchiveStartAddr + pInfo->mStartOffset;
}

u32 ARCGetLength(ARCFileInfo *pInfo)
{
    return pInfo->mLength;
}

u32 ARCClose(ARCFileInfo *pInfo)
{
    return 1;
}

/*u32 ARCChangeDir(ARCHandle *pHandle, const char *pDirName)
{
    s32 entryNum = ARCConvertPathToEntrynum(pHandle, pDirName);

    if (*(s32*)pHandle->mFilesystemStart >= 0 || *(s32*)((s32*)pHandle->mFilesystemStart + (entryNum * 0xC)) != 0)
    {
        pHandle->mCurrentDir = entryNum;
        return 1;
    }

    return 0;
}*/