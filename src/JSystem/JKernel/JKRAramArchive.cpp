#include "JSystem/JKernel/JKRAramArchive.hpp"
#include "JSystem/JKernel/JKRAram.hpp"
#include "JSystem/JKernel/JKRCompression.hpp"
#include "JSystem/JKernel/JKRDvdAramRipper.hpp"
#include "JSystem/JKernel/JKRDvdFile.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include "JSystem/JUtility/JUTException.hpp"
#include <cstdlib>
#include <mem.h>

JKRAramArchive::JKRAramArchive(long entryNum, EMountDirection mountDir) : JKRArchive(entryNum, MOUNT_MODE_ARAM) {
    mMountDir = mountDir;

    if (!open(entryNum)) {
        return;
    }

    mLoaderType = RARC_MAGIC;
    mLoaderName = mStringTable + mDirs->mNameOffset;

    prependVolumeList(&mLoaderLink);

    mIsMounted = true;
}

int JKRConvertAttrToCompressionType(int arg) {
    if ((arg & 0b100) == 0) {
        return 0;
    }

    return ((arg & 0b10000000) != 0) + 1;
}

JKRAramArchive::~JKRAramArchive() {
    if (mIsMounted == true) {
        if (mInfoBlock != NULL) {
            SDIFileEntry* entry = mFiles;
            for (int i = 0; i < mInfoBlock->mNrFiles; i++) {
                if (entry->mFileData != NULL) {
                    JKRFreeToHeap(mHeap, entry->mFileData);
                }
                entry++;
            }

            JKRFreeToHeap(mHeap, mInfoBlock);
            mInfoBlock = NULL;
        }

        if (mExpandSizes != NULL) {
            JKRFree(mExpandSizes);
            mExpandSizes = NULL;
        }

        if (mDvdFile != NULL) {
            delete mDvdFile;
        }

        if (mBlock != NULL) {
            delete mBlock;
        }

        JKRFileLoader::sVolumeList.remove(&mLoaderLink);
        mIsMounted = false;
    }
}

bool JKRAramArchive::open(s32 entryNum) {
    mInfoBlock = NULL;
    mDirs = NULL;
    mFiles = NULL;
    mStringTable = NULL;
    mBlock = NULL;

    mDvdFile = new (JKRGetSystemHeap(), mMountDir == MOUNT_DIRECTION_1 ? 4 : -4) JKRDvdFile(entryNum);
    if (mDvdFile == NULL) {
        mMountMode = 0;
        return 0;
    }

    RarcHeader* mem = NULL;
    mem = (RarcHeader*)JKRAllocFromSysHeap(32, -32);
    if (mem == NULL) {
        mMountMode = 0;
    } else {
        JKRDvdToMainRam(entryNum, (u8*)mem, EXPAND_SWITCH_UNKNOWN1, 32, NULL, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0, &_5C, NULL);
        DCInvalidateRange(mem, 32);
        int alignment = mMountDir == MOUNT_DIRECTION_1 ? 32 : -32;
        u32 alignedSize = ALIGN_NEXT(mem->mFileDataOffset, 32);
        mInfoBlock = (RarcInfoBlock*)JKRAllocFromHeap(mHeap, alignedSize, alignment);
        if (mInfoBlock == NULL) {
            mMountMode = 0;
        } else {
            JKRDvdToMainRam(entryNum, (u8*)mInfoBlock, EXPAND_SWITCH_UNKNOWN1, alignedSize, NULL, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 32, NULL,
                            NULL);
            DCInvalidateRange(mInfoBlock, alignedSize);

            mDirs = (SDIDirEntry*)((u8*)mInfoBlock + mInfoBlock->mDirOffset);
            mFiles = (SDIFileEntry*)((u8*)mInfoBlock + mInfoBlock->mFileOffset);
            mStringTable = (char*)((u8*)mInfoBlock + mInfoBlock->mStringTableOffset);
            mExpandSizes = NULL;

            u8 compressedFiles = 0;

            SDIFileEntry* fileEntry = mFiles;
            for (int i = 0; i < mInfoBlock->mNrFiles; i++) {
                u8 flag = fileEntry->mFlag;
                ;
                if ((flag & 1)) {
                    compressedFiles |= u8(flag & 4);
                }
                fileEntry++;
            }

            if (compressedFiles != 0) {
                mExpandSizes = (s32*)JKRAllocFromHeap(mHeap, mInfoBlock->mNrFiles << 2, abs(alignment));
                if (mExpandSizes == NULL) {
                    JKRFree(mInfoBlock);
                    mMountMode = 0;
                    goto cleanup;
                }
                memset(mExpandSizes, 0, mInfoBlock->mNrFiles << 2);
            }

            u32 aramSize = ALIGN_NEXT(mem->mTotalDataSize, 32);
            mBlock = (JKRAramBlock*)JKRAllocFromAram(aramSize, mMountDir == MOUNT_DIRECTION_1 ? JKRAramHeap::HEAD : JKRAramHeap::TAIL);
            if (mBlock == NULL) {
                if (mInfoBlock) {
                    JKRFree(mInfoBlock);
                }
                if (mExpandSizes) {
                    JKRFree(mExpandSizes);
                }
                mMountMode = 0;
            } else {
                JKRDvdToAram(entryNum, mBlock->getAddress(), EXPAND_SWITCH_UNKNOWN1, mem->mHeaderSize + mem->mFileDataOffset, 0, NULL);
            }
        }
    }
cleanup:
    if (mem != NULL) {
        JKRFreeToSysHeap(mem);
    }
    if (mMountMode == 0) {
        if (mDvdFile != NULL) {
            delete mDvdFile;
        }
        return false;
    }
    return true;
}

void* JKRAramArchive::fetchResource(SDIFileEntry* pEntry, u32* pOutSize) {
    u32 outSize;
    u8* outBuf;
    if (pOutSize == NULL) {
        pOutSize = &outSize;
    }

    JKRCompression compression = JKRConvertAttrToCompressionType(u8(pEntry->mFlag));
    if (pEntry->mFileData == NULL) {
        u32 size =
            JKRAramArchive::fetchResource_subroutine(pEntry->mDataOffset + mBlock->getAddress(), pEntry->mDataSize, mHeap, compression, &outBuf);

        *pOutSize = size;
        if (size == 0) {
            return NULL;
        }

        pEntry->mFileData = outBuf;
        if (compression == COMPRESSION_YAZ0) {
            this->setExpandSize(pEntry, *pOutSize);
        }
    } else {
        if (compression == COMPRESSION_YAZ0) {
            *pOutSize = this->getExpandSize(pEntry);
        } else {
            *pOutSize = pEntry->mDataSize;
        }
    }

    return pEntry->mFileData;
}

void* JKRAramArchive::fetchResource(void* buffer, u32 bufferSize, SDIFileEntry* pEntry, u32* resourceSize) {
    u32 size = pEntry->mDataSize;
    if (size > bufferSize) {
        size = bufferSize;
    }

    JKRCompression compression = JKRConvertAttrToCompressionType(u8(pEntry->mFlag));
    if (pEntry->mFileData == NULL) {
        bufferSize = (s32)ALIGN_PREV(bufferSize, 0x20);
        size = JKRAramArchive::fetchResource_subroutine(pEntry->mDataOffset + mBlock->getAddress(), size, (u8*)buffer, bufferSize, compression);
    } else {
        if (compression == COMPRESSION_YAZ0) {
            u32 expandSize = this->getExpandSize(pEntry);
            if (expandSize != 0) {
                size = expandSize;
            }
        }
        if (size > bufferSize) {
            size = bufferSize;
        }
        JKRHeap::copyMemory(buffer, pEntry->mFileData, size);
    }

    if (resourceSize != NULL) {
        *resourceSize = size;
    }
    return buffer;
}

u32 JKRAramArchive::fetchResource_subroutine(u32 srcAram, u32 srcLength, u8* dst, u32 dstLength, int compression) {
    u32 outLen;
    u32 srcSize = ALIGN_NEXT(srcLength, 0x20);
    u32 dstSize = ALIGN_PREV(dstLength, 0x20);

    switch (compression) {
    case COMPRESSION_NONE:
        if (srcSize > dstSize) {
            srcSize = dstSize;
        }
        JKRAramToMainRam(srcAram, dst, srcSize, EXPAND_SWITCH_UNKNOWN0, dstSize, NULL, -1, &outLen);
        return outLen;
    case COMPRESSION_YAY0:
    case COMPRESSION_YAZ0:
        JKRAramToMainRam(srcAram, dst, srcSize, EXPAND_SWITCH_UNKNOWN1, dstSize, NULL, -1, &outLen);
        return outLen;
    default:
        JUTException::panic(__FILE__, 655, "??? bad sequence\n");
        return 0;
    }
}

u32 JKRAramArchive::fetchResource_subroutine(u32 entryNum, u32 length, JKRHeap* pHeap, int compression, u8** out) {
    u32 alignedLen = ALIGN_NEXT(length, 0x20);

    u8* buffer;
    switch (compression) {
    case COMPRESSION_NONE: {
        buffer = (u8*)(JKRAllocFromHeap(pHeap, alignedLen, 0x20));
        JKRAramToMainRam(entryNum, buffer, alignedLen, EXPAND_SWITCH_UNKNOWN0, alignedLen, NULL, -1, NULL);
        *out = buffer;
        return length;
    }
    case COMPRESSION_YAY0:
    case COMPRESSION_YAZ0: {
        u8 headerBuf[0x40];
        u8* alignHeader = (u8*)ALIGN_NEXT((s32)&headerBuf[0], sizeof(RarcHeader));
        JKRAramToMainRam(entryNum, alignHeader, sizeof(RarcHeader), EXPAND_SWITCH_UNKNOWN0, 0, NULL, -1, NULL);
        u32 decompressedLen = ALIGN_NEXT(JKRDecompExpandSize(alignHeader), sizeof(RarcHeader));
        buffer = (u8*)(JKRAllocFromHeap(pHeap, decompressedLen, sizeof(RarcHeader)));
        u32 readLen;
        JKRAramToMainRam(entryNum, buffer, alignedLen, EXPAND_SWITCH_UNKNOWN1, decompressedLen, pHeap, -1, &readLen);
        *out = buffer;
        return readLen;
    }
    default:
        JUTException::panic(__FILE__, 713, "??? bad sequence\n");
        return 0;
    }
}

u32 JKRAramArchive::getExpandedResSize(const void* ptr) const {
    if (mExpandSizes == NULL) {
        return this->getResSize(ptr);
    }

    JKRArchive::SDIFileEntry* entry = this->findPtrResource(ptr);
    if (entry == NULL) {
        return 0xFFFFFFFF;
    }

    u8 flags = entry->mFlag;
    if ((flags & 4) == 0) {
        return this->getResSize(ptr);
    }

    u32 expandSize = this->getExpandSize(entry);
    if (expandSize != 0) {
        return expandSize;
    }

    u8 tmpBuf[0x40];
    u8* buf = (u8*)ALIGN_PREV((s32)&tmpBuf[0x1F], 0x20);
    JKRAramToMainRam(entry->mDataOffset + mBlock->getAddress(), buf, 0x20, EXPAND_SWITCH_UNKNOWN0, 0, NULL, -1, NULL);
    u32 expandSize2 = JKRDecompExpandSize(buf);
    // ??? casting away const?
    ((JKRArchive*)this)->setExpandSize(entry, expandSize2);
    return expandSize2;
}
