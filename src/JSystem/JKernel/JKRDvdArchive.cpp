#include "JSystem/JKernel/JKRDvdArchive.hpp"
#include "JSystem/JKernel/JKRAramArchive.hpp"
#include "JSystem/JKernel/JKRArchive.hpp"
#include "JSystem/JKernel/JKRCompression.hpp"
#include "JSystem/JKernel/JKRDecomp.hpp"
#include "JSystem/JKernel/JKRDvdFile.hpp"
#include "JSystem/JKernel/JKRDvdRipper.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include "JSystem/JUtility/JUTException.hpp"
#include "revolution/types.h"
#include <cstdlib>
#include <cstring>
#include <mem.h>

JKRDvdArchive::JKRDvdArchive(s32 entryNum, EMountDirection mountDir) : JKRArchive(entryNum, MOUNT_MODE_DVD) {
    mMountDir = mountDir;

    if (!open(entryNum)) {
        return;
    }

    mLoaderType = RARC_MAGIC;
    mLoaderName = mStringTable + mDirs->mNameOffset;

    prependVolumeList(&mLoaderLink);

    mIsMounted = true;
}

JKRDvdArchive::~JKRDvdArchive() {
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

        if (mFile != NULL) {
            delete mFile;
        }

        removeVolumeList(&mLoaderLink);
        mIsMounted = false;
    }
}

bool JKRDvdArchive::open(s32 entryNum) {
    mInfoBlock = nullptr;
    _64 = 0;
    mDirs = nullptr;
    mFiles = nullptr;
    mStringTable = nullptr;

    mFile = new (JKRHeap::getSystemHeap(), 0) JKRDvdFile(entryNum);
    if (mFile == nullptr) {
        mMountMode = MOUNT_MODE_0;
        return false;
    }

    RarcHeader* header = reinterpret_cast< RarcHeader* >(JKRHeap::getSystemHeap()->alloc(32, 32));
    if (header == nullptr) {
        mMountMode = MOUNT_MODE_0;
    } else {
        JKRDvdRipper::loadToMainRAM(entryNum, reinterpret_cast< u8* >(header), EXPAND_SWITCH_UNKNOWN1, 32, nullptr,
                                    JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0, &_5C, nullptr);
        DCInvalidateRange(header, 32);

        int alignment = mMountDir == MOUNT_DIRECTION_1 ? 32 : -32;
        mInfoBlock = reinterpret_cast< RarcInfoBlock* >(JKRHeap::alloc(header->mFileDataOffset, alignment, mHeap));
        if (mInfoBlock == nullptr) {
            mMountMode = MOUNT_MODE_0;
        } else {
            JKRDvdRipper::loadToMainRAM(entryNum, reinterpret_cast< u8* >(mInfoBlock), EXPAND_SWITCH_UNKNOWN1, header->mFileDataOffset, nullptr,
                                        JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 32, nullptr, nullptr);
            DCInvalidateRange(mInfoBlock, header->mFileDataOffset);

            mDirs = reinterpret_cast< SDIDirEntry* >(reinterpret_cast< u8* >(mInfoBlock) + mInfoBlock->mDirOffset);
            mFiles = reinterpret_cast< SDIFileEntry* >(reinterpret_cast< u8* >(mInfoBlock) + mInfoBlock->mFileOffset);
            mStringTable = reinterpret_cast< char* >(mInfoBlock) + mInfoBlock->mStringTableOffset;
            mExpandSizes = nullptr;

            u8 compressedFiles = 0;
            SDIFileEntry* file = mFiles;
            for (u32 i = 0; i < mInfoBlock->mNrFiles; i++) {
                u8 flags = file->mFlag;
                if (flags & FILE_FLAG_FILE) {
                    compressedFiles |= flags & FILE_FLAG_COMPRESSED;
                }
                file++;
            }

            if (compressedFiles != 0) {
                mExpandSizes = reinterpret_cast< s32* >(JKRHeap::alloc(mInfoBlock->mNrFiles * 4, abs(alignment), mHeap));
                if (mExpandSizes == nullptr) {
                    JKRHeap::getSystemHeap()->free(mInfoBlock);
                    mMountMode = MOUNT_MODE_0;
                    goto cleanup;
                } else {
                    memset(mExpandSizes, 0, mInfoBlock->mNrFiles * 4);
                }
            }
            _64 = header->mHeaderSize + header->mFileDataOffset;
        }
    }

cleanup:
    if (header != nullptr) {
        JKRHeap::getSystemHeap()->free(header);
    }
    if (mMountMode == MOUNT_MODE_0) {
        if (mFile != nullptr) {
            delete mFile;
        }
        return false;
    }
    return true;
}

void* JKRDvdArchive::fetchResource(JKRArchive::SDIFileEntry* pArg1, u32* pSize) {
    u32 emergency_pointer;
    if (pSize == nullptr) {
        pSize = &emergency_pointer;
    }

    JKRCompression compression = JKRConvertAttrToCompressionType(pArg1->mFlag);

    if (pArg1->mFileData == nullptr) {
        u8* out;
        u32 size = fetchResource_subroutine(mEntryNum, _64 + pArg1->mDataOffset, pArg1->mDataSize, mHeap, compression, _5C, &out);
        *pSize = size;
        if (size == 0) {
            return nullptr;
        }

        pArg1->mFileData = out;
        if (compression == 2) {
            setExpandSize(pArg1, *pSize);
        }
    } else if (compression == 2) {
        u32 size = getExpandSize(pArg1);
        *pSize = size;
    } else {
        *pSize = pArg1->mDataSize;
    }

    return pArg1->mFileData;
}

void* JKRDvdArchive::fetchResource(void* pArg1, u32 arg2, JKRArchive::SDIFileEntry* pEntry, u32* pSize) {
    u32 fetchedSize = pEntry->mDataSize;
    JKRCompression compression = JKRConvertAttrToCompressionType(pEntry->mFlag);

    if (pEntry->mFileData == nullptr) {
        fetchedSize = fetchResource_subroutine(mEntryNum, _64 + pEntry->mDataOffset, pEntry->mDataSize, static_cast< u8* >(pArg1),
                                               ALIGN_PREV(arg2, 32), compression, _5C);

    } else {
        if (compression == 2) {
            u32 size = getExpandSize(pEntry);

            if (size != 0) {
                fetchedSize = size;
            }
        }

        if (fetchedSize > arg2) {
            fetchedSize = arg2;
        }

        JKRHeap::copyMemory(pArg1, pEntry->mFileData, fetchedSize);
    }

    if (pSize != nullptr) {
        *pSize = fetchedSize;
    }

    return pArg1;
}

u32 JKRDvdArchive::getExpandedResSize(const void* pArg) const {
    if (mExpandSizes == nullptr) {
        return getResSize(pArg);
    }

    SDIFileEntry* fileEntry = findPtrResource(pArg);
    if (fileEntry == nullptr) {
        return -1;
    }

    if ((fileEntry->mFlag & FILE_FLAG_COMPRESSED) == 0) {
        return getResSize(pArg);
    }

    u32 size = getExpandSize(fileEntry);
    if (size != 0) {
        return size;
    }

    u8 buff[0x40];
    u8* alignedPointer = reinterpret_cast< u8* >((ALIGN_NEXT(reinterpret_cast< u32 >(buff), 32)));

    JKRDvdRipper::loadToMainRAM(mEntryNum, alignedPointer, EXPAND_SWITCH_UNKNOWN2, 0x20, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD,
                                _64 + fileEntry->mDataOffset, 0, 0);

    DCInvalidateRange(alignedPointer, 0x20);

    u32 size2 = JKRDecompExpandSize(alignedPointer);
    const_cast< JKRDvdArchive* >(this)->setExpandSize(fileEntry, size2);

    return size2;
}

u32 JKRDvdArchive::fetchResource_subroutine(s32 entryNum, u32 offset, u32 sourceSize, u8* destination, u32 destinationSize, int resourceCompression, int archiveCompression) {
    u32 readSize = ALIGN_NEXT(sourceSize, 32);
    u32 alignedDestinationSize = ALIGN_PREV(destinationSize, 32);

    switch (archiveCompression) {
    case 0: {
        switch (resourceCompression) {
        case 0: {
            if (readSize > alignedDestinationSize) {
                readSize = alignedDestinationSize;
            }

            JKRDvdRipper::loadToMainRAM(entryNum, destination, EXPAND_SWITCH_UNKNOWN0, readSize, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset, nullptr,
                                        nullptr);
            DCInvalidateRange(destination, readSize);
            return readSize;
        }

        case 1:

        case 2: {
            u8 headerBuffer[0x40];
            u8* header = reinterpret_cast< u8* >((ALIGN_NEXT(reinterpret_cast< u32 >(headerBuffer), 32)));

            JKRDvdRipper::loadToMainRAM(entryNum, header, EXPAND_SWITCH_UNKNOWN2, 0x20, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset,
                                        nullptr, nullptr);
            DCInvalidateRange(header, 0x20);

            u32 readSize = JKRDecompExpandSize(header);
            u32 alignedExpandedSize = ALIGN_NEXT(readSize, 32);
            if (alignedExpandedSize > alignedDestinationSize) {
                alignedExpandedSize = alignedDestinationSize;
            }

            JKRDvdRipper::loadToMainRAM(entryNum, destination, EXPAND_SWITCH_UNKNOWN1, alignedExpandedSize, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset, nullptr,
                                        nullptr);
            DCInvalidateRange(destination, alignedExpandedSize);

            return readSize;
        }
        }
    }
    case 2: {
        if (sourceSize > alignedDestinationSize) {
            sourceSize = alignedDestinationSize;
        }

        JKRDvdRipper::loadToMainRAM(entryNum, destination, EXPAND_SWITCH_UNKNOWN1, sourceSize, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset, nullptr,
                                    nullptr);
        DCInvalidateRange(destination, sourceSize);
        return sourceSize;
    }
    case 1: {
        const char* SZP_ERROR = "Sorry, not applied for SZP archive.\n";
        JUTException::panic(__FILE__, 0x289, SZP_ERROR);
    }

    default: {
        const char* SEQUENCE_ERROR = "??? bad sequence\n";
        JUTException::panic(__FILE__, 0x28D, SEQUENCE_ERROR);
    }
    }

    return 0;
}

u32 JKRDvdArchive::fetchResource_subroutine(s32 entryNum, u32 offset, u32 sourceSize, JKRHeap* heap, int resourceCompression, int archiveCompression, u8** outBuffer) {
    u32 alignedSourceSize = ALIGN_NEXT(sourceSize, 32);

    switch (archiveCompression) {
    case 0: {
        switch (resourceCompression) {
        case 0: {
            u8* buffer = static_cast< u8* >(heap->alloc(alignedSourceSize, 0x20, heap));
            JKRDvdRipper::loadToMainRAM(entryNum, buffer, EXPAND_SWITCH_UNKNOWN0, alignedSourceSize, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset, 0, 0);
            DCInvalidateRange(buffer, alignedSourceSize);

            *outBuffer = buffer;
            return alignedSourceSize;
        }

        case 1:

        case 2: {
            u8 headerBuffer[0x40];
            u8* header = reinterpret_cast< u8* >((ALIGN_NEXT(reinterpret_cast< u32 >(headerBuffer), 32)));

            JKRDvdRipper::loadToMainRAM(entryNum, header, EXPAND_SWITCH_UNKNOWN2, 0x20, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset, 0,
                                        0);
            DCInvalidateRange(header, 0x20);

            sourceSize = JKRDecompExpandSize(header);
            u8* buffer = static_cast< u8* >(heap->alloc(sourceSize, 0x20, heap));

            JKRDvdRipper::loadToMainRAM(entryNum, buffer, EXPAND_SWITCH_UNKNOWN1, sourceSize, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset, 0, 0);
            DCInvalidateRange(buffer, sourceSize);
            *outBuffer = buffer;
            return sourceSize;
        }
        }
    }
    case 2: {
        u8* buffer = static_cast< u8* >(heap->alloc(alignedSourceSize, 0x20, heap));
        JKRDvdRipper::loadToMainRAM(entryNum, buffer, EXPAND_SWITCH_UNKNOWN1, sourceSize, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset, 0, 0);
        DCInvalidateRange(buffer, sourceSize);
        *outBuffer = buffer;
        return alignedSourceSize;
    }
    case 1: {
        const char* SZP_ERROR_TYPO = "Sorry, not applied SZP archive.\n";
        JUTException::panic(__FILE__, 0x2F2, SZP_ERROR_TYPO);
    }

    default: {
        const char* SEQUENCE_ERROR = "??? bad sequence\n";
        JUTException::panic(__FILE__, 0x2F6, SEQUENCE_ERROR);
    }
    }

    return 0;
}
