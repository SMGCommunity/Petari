#include "JSystem/JKernel/JKRCompArchive.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"

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
            if (!((file->mNameOffset)) && file->mFileData != NULL) {
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
