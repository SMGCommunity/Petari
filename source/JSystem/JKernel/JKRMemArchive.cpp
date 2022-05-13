#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRDecomp.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTException.h"
#include "revolution.h"
#include <cstring>

JKRMemArchive::JKRMemArchive() {

}

JKRMemArchive::JKRMemArchive(long entryNum, EMountDirection mountDir) : JKRArchive(entryNum, MOUNT_MODE_MEM) {
    mIsMounted = false;
    mMountDir = mountDir;

    if (!open(entryNum, mountDir)) {
        return;
    }

    mLoaderType = RARC_MAGIC;
    mLoaderName = mStringTable + mDirs->mNameOffset;

    prependVolumeList(&mLoaderLink);

    mIsMounted = true;
}

JKRMemArchive::~JKRMemArchive() {
    if (mIsMounted == true) {
        if (_6C && mHeader != NULL) {
            JKRHeap::free(mHeader, mHeap);
        }

        removeVolumeList(&mLoaderLink);
        mIsMounted = false;
    }
}

void JKRMemArchive::removeResourceAll() {
    if (mInfoBlock == NULL) {
        return;
    }

    if (mMountMode == MOUNT_MODE_MEM) {
        return;
    }

    SDIFileEntry *current = mFiles;

    for (s32 i = 0; i < mInfoBlock->mNrFiles; i++) {
        if (current->mFileData != NULL) {
            current->mFileData = NULL;
        }
    }
}

bool JKRMemArchive::removeResource(void *pResource) {
    SDIFileEntry *file = findPtrResource(pResource);

    if (file == NULL) {
        return false;
    }

    file->mFileData = NULL;
    return true;
}

s32 JKRMemArchive::getExpandedResSize(const void *pResource) const {
    SDIFileEntry *file = findPtrResource(pResource);

    if (file == NULL) {
        return -1;
    }

    if ((file->mFlag & FILE_FLAG_COMPRESSED) == 0) {
        return getResSize(pResource);
    }

    return JKRDecompExpandSize(reinterpret_cast<u8 *>(const_cast<void *>(pResource)));
}

void *JKRMemArchive::fetchResource(SDIFileEntry *pFile, unsigned long *pSize) {
    if (pFile->mFileData == NULL) {
        pFile->mFileData = mFileDataStart + pFile->mDataOffset;
    }

    if (pSize != NULL) {
        *pSize = pFile->mDataSize;
    }

    return pFile->mFileData;
}

void *JKRMemArchive::fetchResource(void *pData, unsigned long dataSize, SDIFileEntry *pFile, unsigned long *pSize) {
    u32 size = pFile->mDataSize;

    if (size > dataSize) {
        size = dataSize;
    }

    if (pFile->mFileData != NULL) {
        memcpy(pData, pFile->mFileData, size);
    }
    else {
        s32 compression;

        if ((pFile->mFlag & FILE_FLAG_COMPRESSED) == 0) {
            compression = JKR_COMPRESSION_NONE;
        }
        else if ((pFile->mFlag & FILE_FLAG_IS_YAZ0) != 0) {
            compression = JKR_COMPRESSION_SZS;
        }
        else {
            compression = JKR_COMPRESSION_SZP;
        }

        size = fetchResource_subroutine(
            mFileDataStart + pFile->mDataOffset,
            size,
            reinterpret_cast<u8 *>(pData),
            dataSize,
            compression
            );
    }

    if (pSize != NULL) {
        *pSize = size;
    }

    return pData;
}

void JKRMemArchive::fixedInit(long entryNum) {
    mIsMounted = false;
    mMountMode = MOUNT_MODE_MEM;
    _34 = 1;
    _58 = 2;
    mHeap = JKRHeap::sCurrentHeap;
    mEntryNum = entryNum;

    if (gCurrentFileLoader != NULL) {
        return;
    }

    gCurrentFileLoader = this;
    sCurrentDirIndex = 0;
}

bool JKRMemArchive::mountFixed(void *a1, JKRMemBreakFlag breakFlag) {
    if (check_mount_already(reinterpret_cast<s32>(a1)) != NULL) {
        return false;
    }

    fixedInit(reinterpret_cast<s32>(a1));

    if (!open(a1, 0xFFFF, breakFlag)) {
        return false;
    }

    SDIDirEntry *firstDir = mDirs;
    char *stringTable = mStringTable;

    mLoaderType = RARC_MAGIC;
    mLoaderName = stringTable + firstDir->mNameOffset;

    prependVolumeList(&mLoaderLink);

    mIsMounted = true;
    _6C = breakFlag == JKR_MEM_BREAK_FLAG_1;

    return true;
}

#ifdef NON_MATCHING
// add r3, r5, r3 instead of add r3, r3, r5
bool JKRMemArchive::open(long entryNum, EMountDirection mountDir) {
    mHeader = NULL;
    mInfoBlock = NULL;
    mFileDataStart = NULL;
    mDirs = NULL;
    mFiles = NULL;
    mStringTable = NULL;
    _6C = false;
    mMountDir = mountDir;

    if (mountDir == MOUNT_DIRECTION_1) {
        u32 size;

        u8 *pData = JKRDvdRipper::loadToMainRAM(
            entryNum,
            NULL,
            JKR_EXPAND_SWITCH_1,
            0,
            mHeap,
            JKRDvdRipper::ALLOC_DIRECTION_1,
            0,
            reinterpret_cast<int *>(&_5C),
            &size
            );

        mHeader = reinterpret_cast<RarcHeader *>(pData);

        if (pData != NULL) {
            DCInvalidateRange(pData, size);
        }
    }
    else {
        u32 size;

        u8 *pData = JKRDvdRipper::loadToMainRAM(
            entryNum,
            NULL,
            JKR_EXPAND_SWITCH_1,
            0,
            mHeap,
            JKRDvdRipper::ALLOC_DIRECTION_2,
            0,
            reinterpret_cast<int *>(&_5C),
            &size
            );

        mHeader = reinterpret_cast<RarcHeader * >(pData);

        if (pData != NULL) {
            DCInvalidateRange(pData, size);
        }
    }

    if (mHeader == NULL) {
        mMountMode = MOUNT_MODE_0;
    }
    else {
        mInfoBlock = reinterpret_cast<RarcInfoBlock *>(reinterpret_cast<u8 *>(mHeader) + mHeader->mHeaderSize);;
        mDirs = reinterpret_cast<SDIDirEntry *>(reinterpret_cast<u8 *>(mInfoBlock) + mInfoBlock->mDirOffset);
        mFiles = reinterpret_cast<SDIFileEntry *>(reinterpret_cast<u8 *>(mInfoBlock) + mInfoBlock->mFileOffset);
        mStringTable = reinterpret_cast<char *>(reinterpret_cast<u8 *>(mInfoBlock) + mInfoBlock->mStringTableOffset);
        mFileDataStart = reinterpret_cast<u8 *>(mHeader) + mHeader->mFileDataOffset + mHeader->mHeaderSize;
        _6C = true;
    }

    return mMountMode != MOUNT_MODE_0;
}
#endif

#ifdef NON_MATCHING
// add r5, r4, r5 instead of add r5, r5, r4
bool JKRMemArchive::open(void *pData, unsigned long a2, JKRMemBreakFlag breakFlag) {
    mHeader = reinterpret_cast<RarcHeader *>(pData);
    mInfoBlock = reinterpret_cast<RarcInfoBlock *>(reinterpret_cast<u8 *>(mHeader) + mHeader->mHeaderSize);;
    mDirs = reinterpret_cast<SDIDirEntry *>(reinterpret_cast<u8 *>(mInfoBlock) + mInfoBlock->mDirOffset);
    mFiles = reinterpret_cast<SDIFileEntry *>(reinterpret_cast<u8 *>(mInfoBlock) + mInfoBlock->mFileOffset);
    mStringTable = reinterpret_cast<char *>(reinterpret_cast<u8 *>(mInfoBlock) + mInfoBlock->mStringTableOffset);
    mFileDataStart = reinterpret_cast<u8 *>(mHeader) + mHeader->mFileDataOffset + mHeader->mHeaderSize;
    _6C = breakFlag == JKR_MEM_BREAK_FLAG_1;
    mHeap = JKRHeap::findFromRoot(pData);
    _5C = 0;

    return true;
}
#endif

#ifdef NON_MATCHING
// Register mismatch
s32 JKRMemArchive::fetchResource_subroutine(unsigned char *pSrc, unsigned long srcSize, unsigned char *pDst, unsigned long dstSize, int compression) {
    switch (compression) {
        case JKR_COMPRESSION_NONE:
            if (srcSize > dstSize) {
                srcSize = dstSize;
            }

            memcpy(pDst, pSrc, srcSize);

            return srcSize;
        case JKR_COMPRESSION_SZP:
        case JKR_COMPRESSION_SZS:
            srcSize = JKRDecompExpandSize(pSrc);;

            if (srcSize > dstSize) {
                srcSize = dstSize;
            }

            JKRDecomp::orderSync(pSrc, pDst, srcSize, 0);
            return srcSize;
        default:
            JUTException::panic_f(__FILE__, 723, "%s", "??? bad sequence\n");
            break;
    }

    return 0;
}
#endif
