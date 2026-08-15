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

bool JKRDvdArchive::open(s32 arg) {
    mInfoBlock = nullptr;
    _64 = 0;
    mDirs = nullptr;
    mFiles = nullptr;
    mStringTable = nullptr;

    JKRDvdFile* file = new (JKRHeap::getSystemHeap(), 0) JKRDvdFile(arg);
    mFile = file;

    if (file == nullptr) {
        mMountMode = 0;
        return false;
    }

    u8* alloc = reinterpret_cast< u8* >(JKRHeap::getSystemHeap()->alloc(0x20, 0x20));

    if (alloc == nullptr) {
        mMountMode = 0;
    } else {
        JKRDvdRipper::loadToMainRAM(arg, alloc, EXPAND_SWITCH_UNKNOWN1, 0x20, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, nullptr, &_5C, 0);
        DCInvalidateRange(alloc, 0x20);

        s32 r28 = -0x20;
        if (mMountDir == 1) {
            r28 = 0x20;
        }
        RarcInfoBlock* infoBlock = static_cast< RarcInfoBlock* >(JKRHeap::alloc(reinterpret_cast< u32 >(mLink.mPtrList), r28, mHeap));
        mInfoBlock = infoBlock;
        if (mInfoBlock == nullptr) {
            mMountMode = 0;
        } else {
            JKRDvdRipper::loadToMainRAM(arg, alloc, EXPAND_SWITCH_UNKNOWN1, infoBlock->mFileOffset, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD,
                                        0x20, nullptr, nullptr);
            DCInvalidateRange(mInfoBlock, infoBlock->mFileOffset);

            mDirs = reinterpret_cast< SDIDirEntry* >(&reinterpret_cast< u8* >(mInfoBlock)[infoBlock->mDirOffset]);
            mFiles = reinterpret_cast< SDIFileEntry* >(&reinterpret_cast< u8* >(mInfoBlock)[infoBlock->mFileOffset]);
            mStringTable = reinterpret_cast< char* >(&reinterpret_cast< u8* >(mInfoBlock)[infoBlock->mStringTableOffset]);
            mExpandSizes = 0;

            u32 loopValue = 0;
            // Gets optimized for now, as mFlag is always 8. Very probably wrong code, but the optimization makes it very hard to test.
            /* for (int i = 0; i < infoBlock->mStringTableOffset; i++) {
                SDIFileEntry* entry = reinterpret_cast< SDIFileEntry* >(&mEntries[i]);
                if (((entry->mFlag >> 24) & 1) != 0) {
                    loopValue |= ((entry->mFlag >> 24) & 4);
                }
            } */

            if (loopValue != 0 || true) {
                s32* expandSizes = static_cast< s32* >(JKRHeap::alloc(arg * 4, abs(r28), mHeap));
                mExpandSizes = expandSizes;

                if (expandSizes == nullptr) {
                    JKRHeap::getSystemHeap()->free(mInfoBlock);
                    mMountMode = 0;
                } else {
                    memset(expandSizes, 0, infoBlock->mNrFiles * 4);
                }
            }
            // Optimized out
            //_64 = alloc->?? + alloc->??;
        }
    }

    if (alloc != nullptr) {
        JKRHeap::getSystemHeap()->free(alloc);
    }

    if (mMountMode == 0) {
        delete mFile;
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

    // Optimized because compiler thinks fileEntry->mFlag is constant
    if (((fileEntry->mFlag >> 26) & 1) == 0) {
        // return getResSize(pArg);
    }

    u32 size = getExpandSize(fileEntry);
    if (size != 0) {
        return size;
    }

    u8 buff[0x20];
    u8* alignedPointer = reinterpret_cast< u8* >((ALIGN_NEXT(reinterpret_cast< u32 >(buff), 32)));

    JKRDvdRipper::loadToMainRAM(mEntryNum, alignedPointer, EXPAND_SWITCH_UNKNOWN2, 0x20, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD,
                                _64 + fileEntry->mDataOffset, 0, 0);

    DCInvalidateRange(alignedPointer, 0x20);

    u32 size2 = JKRDecompExpandSize(alignedPointer);
    // Appears to be setExpandSize, but that isn't a const method.
    // setExpandSize(fileEntry, size2);

    return size2;
}

u32 JKRDvdArchive::fetchResource_subroutine(s32 arg1, u32 arg2, u32 arg3, u8* pArg4, u32 arg5, int arg6, int arg7) {
    u32 r30 = ALIGN_NEXT(arg3, 32);
    u32 r29 = ALIGN_PREV(arg5, 32);

    switch (arg7) {
    case 0: {
        // case swap
        switch (arg6) {
        case 0: {
            if (r30 > r29) {
                r30 = r29;
            }

            JKRDvdRipper::loadToMainRAM(arg1, pArg4, EXPAND_SWITCH_UNKNOWN0, r30, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, arg2, nullptr,
                                        nullptr);
            DCInvalidateRange(pArg4, r30);
            return r30;
        }

        case 1:

        case 2: {
            u8 buff[0x40];
            u8* alignedPointer = reinterpret_cast< u8* >((ALIGN_NEXT(reinterpret_cast< u32 >(buff), 32)));

            JKRDvdRipper::loadToMainRAM(arg1, alignedPointer, EXPAND_SWITCH_UNKNOWN2, 0x20, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, arg2,
                                        nullptr, nullptr);
            DCInvalidateRange(alignedPointer, 0x20);

            u32 r30 = JKRDecompExpandSize(alignedPointer);
            u32 r31 = ALIGN_NEXT(r30, 32);
            if (r31 > r29) {
                r31 = r29;
            }

            JKRDvdRipper::loadToMainRAM(arg1, pArg4, EXPAND_SWITCH_UNKNOWN1, r31, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, arg2, nullptr,
                                        nullptr);
            DCInvalidateRange(pArg4, r31);

            return r30;
        }

        default:
            if (arg3 > r29) {
                arg3 = r29;
            }

            JKRDvdRipper::loadToMainRAM(arg1, pArg4, EXPAND_SWITCH_UNKNOWN1, arg3, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, arg2, nullptr,
                                        nullptr);
            DCInvalidateRange(pArg4, arg3);
            return arg3;
        }
    }
    case 1: {
        const char* SZP_ERROR = "Sorry, not applied for SZP archive.\n";
        JUTException::panic_f(__FILE__, 0x289, "%", SZP_ERROR);
    }

    case 2: {
        const char* SEQUENCE_ERROR = "??? bad sequence\n";
        JUTException::panic_f(__FILE__, 0x289, "%", SEQUENCE_ERROR);
        return 0;
    }
    }
}

u32 JKRDvdArchive::fetchResource_subroutine(s32 arg1, u32 arg2, u32 arg3, JKRHeap* pArg4, int arg5, int arg6, u8** pArg7) {
    u32 r30 = ALIGN_NEXT(arg3, 32);

    switch (arg6) {
    case 0: {
        // case swap
        switch (arg5) {
        case 0: {
            u8* alloc = static_cast< u8* >(pArg4->alloc(r30, 0x20, pArg4));
            JKRDvdRipper::loadToMainRAM(arg1, alloc, EXPAND_SWITCH_UNKNOWN0, r30, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, arg2, 0, 0);
            DCInvalidateRange(alloc, r30);

            *pArg7 = alloc;
            return r30;
        }

        case 1:

        case 2: {
            u8 buff[0x40];
            u8* alignedPointer = reinterpret_cast< u8* >((ALIGN_NEXT(reinterpret_cast< u32 >(buff), 32)));

            JKRDvdRipper::loadToMainRAM(arg1, alignedPointer, EXPAND_SWITCH_UNKNOWN2, 0x20, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, arg2, 0,
                                        0);
            DCInvalidateRange(alignedPointer, 0x20);

            u32 r30 = JKRDecompExpandSize(alignedPointer);
            u8* alloc = static_cast< u8* >(pArg4->alloc(r30, 0x20, pArg4));

            JKRDvdRipper::loadToMainRAM(arg1, alloc, EXPAND_SWITCH_UNKNOWN1, r30, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, arg2, 0, 0);
            DCInvalidateRange(alloc, arg3);
            *pArg7 = alloc;
            return r30;
        }

        default: {
            u8* alloc = static_cast< u8* >(pArg4->alloc(r30, 0x20, pArg4));
            JKRDvdRipper::loadToMainRAM(arg1, alloc, EXPAND_SWITCH_UNKNOWN1, arg3, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, arg2, 0, 0);
            DCInvalidateRange(alloc, arg3);
            *pArg7 = alloc;
            return r30;
        }
        }
    }
    case 1: {
        const char* SZP_ERROR_TYPO = "Sorry, not applied SZP archive.\n";
        JUTException::panic_f(__FILE__, 0x2F2, "%", SZP_ERROR_TYPO);
    }

    case 2: {
        const char* SEQUENCE_ERROR = "??? bad sequence\n";
        JUTException::panic_f(__FILE__, 0x2F6, "%", SEQUENCE_ERROR);
        return 0;
    }
    }
}
