#include "JKernel/JKRFileLoader.h"
#include "JSupport/JSUList.h"

//JSUPtrList JKRFileLoader::sVolumeList = JSUList<JKRFileLoader>();

void JKRFileLoader::initializeVolumeList()
{
    OSInitMutex(&JKRFileLoader::sVolumeListMutex);
}