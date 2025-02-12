#include "JSystem/JKernel/JKRAram.hpp"
#include "JSystem/JKernel/JKRAramArchive.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"

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

/*JKRAramArchive::~JKRAramArchive() {
    if (mIsMounted == true) {
        if (mInfoBlock != nullptr) {
            SDIFileEntry *current = mFiles;

            for (s32 i = 0; i < mInfoBlock->mNrFiles; i++) {
                if (current->mFileData != nullptr) {
                    JKRHeap::free(current->mFileData, mHeap);
                }

                current++;
            }
        }

        if (mExpandSizes != nullptr) {
            JKRHeap::free(mExpandSizes, nullptr);
        }

        // 0x68
        // 0x64

        mIsMounted = false;
    }
}*/

/*s32 JKRAramArchive::getExpandedResSize(const void *pResource) const {
    if (mExpandSizes == nullptr) {
        return getResSize(pResource);
    }

    SDIFileEntry *file = findPtrResource(pResource);

    if (file == nullptr) {
        return -1;
    }

    if ((file->mFlag & FILE_FLAG_COMPRESSED) == 0) {
        return getResSize(pResource);
    }

    u32 expandSize = getExpandSize(file);

    if (expandSize != 0) {
        return expandSize;
    }

    JKRAram::aramToMainRam()
}*/
