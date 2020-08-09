#include "JKernel/JKRFileLoader.h"
#include "JSupport/JSUList.h"

#include "smg.h"

JKRFileLoader::JKRFileLoader() : JKRDisposer(), _18(this)
{
    _28 = 0;
    _2C = 0;
    _34 = 0;
}

void JKRFileLoader::unmount()
{
    if (_34 != 0)
    {
        _34--;

        if (_34 == 0 && this != nullptr)
        {
            delete this;
        }
    }
}

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