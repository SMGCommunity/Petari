#include "JSystem/JKernel/JKRDvdArchive.hpp"

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