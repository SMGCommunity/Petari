#include "Game/System/GameSystem.h"
#include "Game/System/GameSystemException.h"
#include "Game/System/FileRipper.h"
#include "Game/System/HeapMemoryWatcher.h"
#include "Game/SingletonHolder.h"
#include "nw4r/lyt/lyt_init.h"
#include "Game/Util.h"

/*
void main(void) {
    __asm {
        li r3, 4
        oris r3, r3, 4
        mtspr 0x392, r3
        li r3, 5
        oris r3, r3, 5
        mtspr 0x393, r3
        li r3, 6
        oris r3, r3, 6
        mtspr 0x394, r3
        li r3, 7
        oris r3, r3, 7
        mtspr 0x395, r3
    };

    DVDInit();
    VIInit();
    //HeapMemoryWatcher::createRootHeap();
    OSInitMutex(&MR::MutexHolder<0>::sMutex);
    OSInitMutex(&MR::MutexHolder<1>::sMutex);
    OSInitMutex(&MR::MutexHolder<2>::sMutex);
    nw4r::lyt::LytInit();
    MR::setLayoutDefaultAllocator();

    if (!SingletonHolder<HeapMemoryWatcher>::sInstance) {
        SingletonHolder<HeapMemoryWatcher>::sInstance = new HeapMemoryWatcher();
    }

    SingletonHolder<HeapMemoryWatcher>::sInstance->setCurrentHeapToStationedHeap();
    JKRHeap* napaHeap = MR::getStationedHeapNapa();
    //FileRipper::setup(0x20000, napaHeap);
    //GameSystemException::init();
    //MR::initAcosTable();

    if (!SingletonHolder<GameSystem>::sInstance) {
        SingletonHolder<GameSystem>::sInstance = new GameSystem();
    }

    SingletonHolder<GameSystem>::sInstance->init();
    GameSystem* gameSystem = SingletonHolder<GameSystem>::sInstance;

    while (true) {
        gameSystem->frameLoop();
    }    
}
*/