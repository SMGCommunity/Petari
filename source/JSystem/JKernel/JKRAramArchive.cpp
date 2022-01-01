#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRAramArchive.h"
#include "JSystem/JKernel/JKRHeap.h"

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
        if (mInfoBlock != NULL) {
            SDIFileEntry *current = mFiles;

            for (s32 i = 0; i < mInfoBlock->mNrFiles; i++) {
                if (current->mFileData != NULL) {
                    JKRHeap::free(current->mFileData, mHeap);
                }

                current++;
            }
        }

        if (mExpandSizes != NULL) {
            JKRHeap::free(mExpandSizes, NULL);
        }

        // _68
        // _64

        mIsMounted = false;
    }
}*/

/*s32 JKRAramArchive::getExpandedResSize(const void *pResource) const {
    if (mExpandSizes == NULL) {
        return getResSize(pResource);
    }

    SDIFileEntry *file = findPtrResource(pResource);

    if (file == NULL) {
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
