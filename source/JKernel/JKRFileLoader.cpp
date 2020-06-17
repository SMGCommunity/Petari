#include "JKernel/JKRFileLoader.h"
#include "JSupport/JSUList.h"

//JSUPtrList JKRFileLoader::sVolumeList = JSUList<JKRFileLoader>();

void JKRFileLoader::initializeVolumeList()
{
    OSInitMutex(&JKRFileLoader::sVolumeListMutex);
}

void JKRFileLoader::prependVolumeList(JSULink<JKRFileLoader> *link)
{
    OSLockMutex(&JKRFileLoader::sVolumeListMutex);
    JKRFileLoader::sVolumeList.prepend(link);
    OSUnlockMutex(&JKRFileLoader::sVolumeListMutex);
}

void JKRFileLoader::removeVolumeList(JSULink<JKRFileLoader> *link)
{
    OSLockMutex(&JKRFileLoader::sVolumeListMutex);
    JKRFileLoader::sVolumeList.remove(link);
    OSUnlockMutex(&JKRFileLoader::sVolumeListMutex);
}