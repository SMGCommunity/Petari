#include "JSystem/JKernel/JKRCompArchive.hpp"
#include "JSystem/JKernel/JKRAram.hpp"
#include "JSystem/JKernel/JKRAramArchive.hpp"
#include "JSystem/JKernel/JKRAramBlock.hpp"
#include "JSystem/JKernel/JKRDecomp.hpp"
#include "JSystem/JKernel/JKRDvdAramRipper.hpp"
#include "JSystem/JKernel/JKRDvdArchive.hpp"
#include "JSystem/JKernel/JKRDvdFile.hpp"
#include "JSystem/JKernel/JKRDvdRipper.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include "JSystem/JKernel/JKRMemArchive.hpp"
#include "JSystem/JUtility/JUTException.hpp"
#include <cstdlib>
#include <mem.h>
#include <stdint.h>

JKRCompArchive::JKRCompArchive(s32 entryNum, JKRArchive::EMountDirection eMountDirection) : JKRArchive(entryNum, MOUNT_MODE_COMP) {
    mMountDir = eMountDirection;

    if (!open(entryNum)) {
        return;
    }

    mLoaderType = 'RARC';
    mLoaderName = &mStringTable[mDirs->mNameOffset];
    /* SMG1 change */
    prependVolumeList(&mLoaderLink);
    // sVolumeList.prepend(&mLoaderLink);
    mIsMounted = true;
}

JKRCompArchive::~JKRCompArchive() {
    if (mInfoBlock != NULL) {
        SDIFileEntry* file = mFiles;
        for (int i = 0; i < mInfoBlock->mNrFiles; i++) {
            if (!((file->mFlag)) && file->mFileData != NULL) {
                JKRFreeToHeap(mHeap, file->mFileData);
            }

            file++;
        }

        JKRFreeToHeap(mHeap, mInfoBlock);
        mInfoBlock = NULL;
    }

    if (mAramPart != NULL) {
        delete mAramPart;
    }

    if (mExpandSizes != NULL) {
        JKRFree(mExpandSizes);
        mExpandSizes = NULL;
    }

    if (mDvdFile != NULL) {
        delete mDvdFile;
    }

    removeVolumeList(&mLoaderLink);
    mIsMounted = false;
}

bool JKRCompArchive::open(s32 entryNum) {
    mInfoBlock = NULL;
    field_0x64 = 0;
    mAramPart = NULL;
    field_0x6c = 0;
    mSizeOfMemPart = 0;
    mSizeOfAramPart = 0;
    field_0x7c = 0;
    mDirs = NULL;
    mFiles = NULL;
    mStringTable = NULL;

    mDvdFile = new (JKRGetSystemHeap(), 0) JKRDvdFile(entryNum);
    if (mDvdFile == NULL) {
        mMountMode = 0;
        return 0;
    }
    RarcHeader* arcHeader = NULL;
    arcHeader = (RarcHeader*)JKRAllocFromSysHeap(sizeof(RarcHeader), -32);  // NOTE: unconfirmed if this struct is used
    if (arcHeader == NULL) {
        mMountMode = 0;
    } else {
        int alignment;

        JKRDvdToMainRam(entryNum, (u8*)arcHeader, EXPAND_SWITCH_UNKNOWN1, 32, NULL, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0, &_5C, NULL);
        DCInvalidateRange(arcHeader, 32);

        mSizeOfMemPart = arcHeader->mMRamDataSize;
        mSizeOfAramPart = arcHeader->mARamDataSize;

        switch (_5C) {
        case 0:
        case 2:
            alignment = mMountDir == 1 ? 32 : -32;
            mInfoBlock = (RarcInfoBlock*)JKRAllocFromHeap(mHeap, arcHeader->mFileDataOffset + mSizeOfMemPart, alignment);
            if (mInfoBlock == NULL) {
                mMountMode = 0;
            } else {
                JKRDvdToMainRam(entryNum, (u8*)mInfoBlock, EXPAND_SWITCH_UNKNOWN1, (uintptr_t)arcHeader->mFileDataOffset + mSizeOfMemPart, NULL,
                                JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0x20, NULL, NULL);
                DCInvalidateRange(mInfoBlock, (uintptr_t)arcHeader->mFileDataOffset + mSizeOfMemPart);
                field_0x64 = (uintptr_t)mInfoBlock + arcHeader->mFileDataOffset;

                if (mSizeOfAramPart != 0) {
                    mAramPart = (JKRAramBlock*)JKRAllocFromAram(mSizeOfAramPart, JKRAramHeap::HEAD);
                    if (mAramPart == NULL) {
                        mMountMode = 0;
                        break;
                    }

                    JKRDvdToAram(entryNum, mAramPart->getAddress(), EXPAND_SWITCH_UNKNOWN1,
                                 arcHeader->mHeaderSize + arcHeader->mFileDataOffset + mSizeOfMemPart, 0, NULL);
                }

                mDirs = (SDIDirEntry*)((uintptr_t)mInfoBlock + mInfoBlock->mDirOffset);
                mFiles = (SDIFileEntry*)((uintptr_t)mInfoBlock + mInfoBlock->mFileOffset);
                mStringTable = (char*)((uintptr_t)mInfoBlock + mInfoBlock->mStringTableOffset);
                field_0x6c = arcHeader->mHeaderSize + arcHeader->mFileDataOffset;
            }
            break;

        case 1:
            u32 alignedSize = ALIGN_NEXT(mDvdFile->getFileSize(), 32);
            alignment = ((mMountDir == 1) ? 32 : -32);
            u8* buf = (u8*)JKRAllocFromSysHeap(alignedSize, -alignment);

            if (buf == NULL) {
                mMountMode = 0;
            } else {
                JKRDvdToMainRam(entryNum, buf, EXPAND_SWITCH_UNKNOWN2, alignedSize, NULL, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0, NULL, NULL);
                DCInvalidateRange(buf, alignedSize);
                u32 expandSize = ALIGN_NEXT(JKRDecompExpandSize(buf), 32);
                u8* mem = (u8*)JKRAllocFromHeap(mHeap, expandSize, -alignment);

                if (mem == NULL) {
                    mMountMode = 0;
                } else {
                    arcHeader = (RarcHeader*)mem;
                    JKRDecompress((u8*)buf, (u8*)mem, expandSize, 0);
                    JKRFreeToSysHeap(buf);

                    mInfoBlock = (RarcInfoBlock*)JKRAllocFromHeap(mHeap, arcHeader->mFileDataOffset + mSizeOfMemPart, alignment);
                    if (mInfoBlock == NULL) {
                        mMountMode = 0;
                    } else {
                        // arcHeader + 1 should lead to 0x20, which is the data after the header
                        JKRHeap::copyMemory((u8*)mInfoBlock, arcHeader + 1, (arcHeader->mFileDataOffset + mSizeOfMemPart));
                        field_0x64 = (uintptr_t)mInfoBlock + arcHeader->mFileDataOffset;
                        if (mSizeOfAramPart != 0) {
                            mAramPart = (JKRAramBlock*)JKRAllocFromAram(mSizeOfAramPart, JKRAramHeap::HEAD);
                            if (mAramPart == NULL) {
                                mMountMode = 0;
                            } else {
                                JKRMainRamToAram((u8*)mem + arcHeader->mHeaderSize + arcHeader->mFileDataOffset + mSizeOfMemPart,
                                                 mAramPart->getAddress(), mSizeOfAramPart, EXPAND_SWITCH_UNKNOWN0, 0, NULL, -1, NULL);
                            }
                        }
                    }
                }
            }
            mDirs = (SDIDirEntry*)((uintptr_t)mInfoBlock + mInfoBlock->mDirOffset);
            mFiles = (SDIFileEntry*)((uintptr_t)mInfoBlock + mInfoBlock->mFileOffset);
            mStringTable = (char*)((uintptr_t)mInfoBlock + mInfoBlock->mStringTableOffset);
            field_0x6c = arcHeader->mHeaderSize + arcHeader->mFileDataOffset;
            break;
        }

        mExpandSizes = NULL;
        u8 compressedFiles = 0;
        SDIFileEntry* fileEntry = mFiles;
        for (int i = 0; i < mInfoBlock->mNrFiles; i++) {
            u8 flag = fileEntry->mFlag;
            if (((flag & 0x1) != 0) && (((flag) & 0x10) == 0)) {
                compressedFiles |= (flag & 4);
            }
            fileEntry++;
        }

        if (compressedFiles != 0) {
            mExpandSizes = (s32*)JKRAllocFromHeap(mHeap, mInfoBlock->mNrFiles * 4, abs(alignment));
            if (mExpandSizes == NULL) {
                JKRFreeToSysHeap(mInfoBlock);
                mMountMode = 0;
            } else {
                memset(mExpandSizes, 0, mInfoBlock->mNrFiles * 4);
            }
        }
    }

    if (arcHeader != NULL) {
        JKRFreeToSysHeap(arcHeader);
    }
    if (mMountMode == 0) {
        if (mDvdFile != NULL) {
            delete mDvdFile;
        }
        return false;
    }
    return true;
}

void* JKRCompArchive::fetchResource(SDIFileEntry* fileEntry, u32* pSize) {
    u32 ptrSize;
    u32 size = fileEntry->mDataSize;
    int compression = JKRConvertAttrToCompressionType(u8(fileEntry->mFlag));

    if (pSize == NULL) {
        pSize = &ptrSize;  // this makes barely any sense but ok
    }

    if (fileEntry->mFileData == NULL) {
        u32 flag = fileEntry->mFlag;
        if (flag & 0x10) {
            fileEntry->mFileData = (void*)(field_0x64 + fileEntry->mDataOffset);
            *pSize = size;
        } else if (flag & 0x20) {
            u8* data;
            size = JKRAramArchive::fetchResource_subroutine(fileEntry->mDataOffset + mAramPart->getAddress() - mSizeOfMemPart, size, mHeap,
                                                            compression, &data);
            *pSize = size;
            fileEntry->mFileData = data;
            if (compression == COMPRESSION_YAZ0) {
                setExpandSize(fileEntry, *pSize);
            }
        } else if (flag & 0x40) {
            u8* data;
            u32 resSize = JKRDvdArchive::fetchResource_subroutine(mEntryNum, field_0x6c + fileEntry->mDataOffset, fileEntry->mDataSize, mHeap,
                                                                  compression, _5C, &data);
            if (pSize != NULL) {
                *pSize = resSize;
            }
            fileEntry->mFileData = data;
            if (compression == COMPRESSION_YAZ0) {
                setExpandSize(fileEntry, *pSize);
            }
        }
    } else {
        if (pSize != NULL) {
            *pSize = fileEntry->mDataSize;
        }
    }
    return fileEntry->mFileData;
}

void* JKRCompArchive::fetchResource(void* data, u32 compressedSize, SDIFileEntry* fileEntry, u32* pSize) {
    u32 size = 0;
    u32 fileSize = fileEntry->mDataSize;
    u32 alignedSize = ALIGN_NEXT(fileSize, 32);
    u32 fileFlag = fileEntry->mFlag;
    int compression = JKRConvertAttrToCompressionType(u8(fileFlag));

    if (fileEntry->mFileData != NULL) {
        if (compression == COMPRESSION_YAZ0) {
            u32 expandSize = getExpandSize(fileEntry);
            if (expandSize != 0) {
                fileSize = expandSize;
            }
        }

        if (fileSize > compressedSize) {
            fileSize = compressedSize;
        }

        JKRHeap::copyMemory(data, fileEntry->mFileData, fileSize);
        size = fileSize;
    } else {
        if (fileFlag & 0x10) {
            size = JKRMemArchive::fetchResource_subroutine((u8*)(field_0x64 + fileEntry->mDataOffset), alignedSize, (u8*)data, compressedSize & ~31,
                                                           compression);
        } else if (fileFlag & 0x20) {
            size = JKRAramArchive::fetchResource_subroutine(fileEntry->mDataOffset + mAramPart->getAddress() - mSizeOfMemPart, alignedSize, (u8*)data,
                                                            compressedSize & ~31, compression);
        } else if (fileFlag & 0x40) {
            size = JKRDvdArchive::fetchResource_subroutine(mEntryNum, field_0x6c + fileEntry->mDataOffset, alignedSize, (u8*)data,
                                                           compressedSize & ~31, compression, _5C);
        } else {
            JUTException::panic(__FILE__, 776, "illegal archive.");
        }
    }

    if (pSize != NULL) {
        *pSize = size;
    }
    return data;
}

void JKRCompArchive::removeResourceAll() {
    if (mInfoBlock != NULL && mMountMode != 1) {
        SDIFileEntry* fileEntry = mFiles;
        for (int i = 0; i < mInfoBlock->mNrFiles; i++) {
            int tmp = fileEntry->mFlag;

            if (fileEntry->mFileData != NULL) {
                if (!(tmp & 0x10)) {
                    JKRFreeToHeap(mHeap, fileEntry->mFileData);
                }

                fileEntry->mFileData = NULL;
            }
        }
        fileEntry++;
    }
}

bool JKRCompArchive::removeResource(void* resource) {
    SDIFileEntry* fileEntry = findPtrResource(resource);
    if (!fileEntry)
        return false;

    u32 flags = fileEntry->mFlag;
    if ((flags & 0x10) == 0) {
        JKRFreeToHeap(mHeap, resource);
    }

    fileEntry->mFileData = NULL;
    return true;
}

u32 JKRCompArchive::getExpandedResSize(const void* resource) const {
    if (mExpandSizes == NULL) {
        return getResSize(resource);
    }

    SDIFileEntry* fileEntry = findPtrResource(resource);
    if (!fileEntry) {
        return 0xffffffff;
    }

    u8 flags = fileEntry->mFlag;
    if ((flags & 4) == 0) {  // not compressed
        return getResSize(resource);
    }

    if ((flags & 0x10) != 0) {
        u32 expandSize = JKRDecompExpandSize((u8*)resource);
        return expandSize;
    }

    u8 buf[64];
    u8* bufPtr = (u8*)ALIGN_NEXT((uintptr_t)buf, 32);
    if ((flags & 0x20) != 0) {
        JKRAramToMainRam(fileEntry->mDataOffset + mAramPart->getAddress(), bufPtr, sizeof(buf) / 2, EXPAND_SWITCH_UNKNOWN0, 0, NULL, -1, NULL);
        DCInvalidateRange(bufPtr, sizeof(buf) / 2);
    } else if ((flags & 0x40) != 0) {
        JKRDvdToMainRam(mEntryNum, bufPtr, EXPAND_SWITCH_UNKNOWN2, sizeof(buf) / 2, NULL, JKRDvdRipper::ALLOC_DIRECTION_FORWARD,
                        field_0x6c + fileEntry->mDataOffset, NULL, NULL);
        DCInvalidateRange(bufPtr, sizeof(buf) / 2);
    } else {
        JUTException::panic(__FILE__, 943, "illegal resource.");
    }
    u32 expandSize = JKRDecompExpandSize(bufPtr);
    const_cast< JKRCompArchive* >(this)->setExpandSize(fileEntry, expandSize);
    return expandSize;
}
