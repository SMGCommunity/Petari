#include "System/Game/GameSystem.h"
#include "Util/ModelUtil.h"
#include <revolution/os.h>

void main()
{
    OSInitFastCast();
    DVDInit();
    VIInit();
    // HeapMemoryWatcher::createRootHeap()
    OSInitMutex(&MR::MutexHolder<0>::sMutex);
    OSInitMutex(&MR::MutexHolder<1>::sMutex);
    OSInitMutex(&MR::MutexHolder<2>::sMutex);
    nw4r::lyt::LytInit();
}