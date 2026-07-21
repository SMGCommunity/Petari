#include "JSystem/JKernel/JKRDvdArchive.hpp"
#include "JSystem/JKernel/JKRDvdFile.hpp"
#include "JSystem/JKernel/JKRDvdRipper.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include <cstdlib>
#include <cstring>
#include <mem.h>

JKRDvdArchive::JKRDvdArchive(long entryNum, EMountDirection mountDir) : JKRArchive(entryNum, MOUNT_MODE_DVD) {
    mMountDir = mountDir;

    if (!open(entryNum)) {
        return;
    }

    mLoaderType = RARC_MAGIC;
    mLoaderName = mStringTable + mDirs->mNameOffset;

    prependVolumeList(&mLoaderLink);

    mIsMounted = true;
}

bool JKRDvdArchive::open(long arg) {
    mEntries = nullptr;
    _64 = 0;
    mDirs = nullptr;
    mFiles = nullptr;
    mStringTable = nullptr;

    JKRDvdFile* file = new (JKRHeap::getSystemHeap(), 0) JKRDvdFile(arg);
    mFile = file;

    RarcInfoBlock* infoBlock = static_cast< RarcInfoBlock* >(&mEntries[0]);

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
        RarcInfoBlock* infoBlock = static_cast< RarcInfoBlock* >(JKRHeap::alloc((u32)mLink.mPtrList, r28, mHeap));
        mEntries = infoBlock;
        if (mEntries == nullptr) {
            mMountMode = 0;
        } else {
            u32* out = 0;
            JKRDvdRipper::loadToMainRAM(arg, alloc, EXPAND_SWITCH_UNKNOWN1, infoBlock->mFileOffset, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD,
                                        0x20, nullptr, out);
            DCInvalidateRange(mEntries, infoBlock->mFileOffset);

            mDirs = reinterpret_cast< SDIDirEntry* >(&mEntries[infoBlock->mDirOffset]);
            mFiles = nullptr;
            mExpandSizes = out;
            mStringTable = nullptr;

            u32 loopValue = 0;
            // mInfoBlock[infoBlock->mStringTableOffset]
            for (int i = 0; i < infoBlock->mStringTableOffset; i++) {
                SDIFileEntry* entry = reinterpret_cast< SDIFileEntry* >(&mEntries[i]);
                // Gets optimized for now, as mFlag is always 8
                if (((entry->mFlag >> 24) & 1) != 0) {
                    loopValue |= ((entry->mFlag >> 24) & 4);
                }
            }

            if (loopValue != 0) {
                u32* expandSizes = static_cast< u32* >(JKRHeap::alloc(arg * 4, abs(r28), mHeap));
                mExpandSizes = expandSizes;

                if (expandSizes == nullptr) {
                    JKRHeap::getSystemHeap()->free(mEntries);
                    mMountMode = 0;
                } else {
                    memset(expandSizes, 0, infoBlock->mNrFiles * 4);
                }
            } else {
                _64 = alloc[alloc[2]];
            }
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
