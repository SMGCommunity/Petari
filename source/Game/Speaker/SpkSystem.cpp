#include "Game/Speaker/SpkSystem.h"
#include "Game/Speaker/SpkSpeakerCtrl.h"

SpkSystem::SpkSystem(JKRHeap *pHeap) : JASGlobalInstance(this) {
    mData = nullptr;

    if (!pHeap) {
        pHeap = JKRHeap::sCurrentHeap;
    }

    mHeap = pHeap;
    mMixBuffer = new(pHeap, 0) SpkMixingBuffer(pHeap);
    mSoundHolder = new(pHeap, 0) SpkSoundHolder();
    SpkSpeakerCtrl::setup();
    SpkSpeakerCtrl::setMixingBuffer(mMixBuffer);
}

void SpkSystem::setResource(JKRArchive *pArchive, u16 a2, u16 a3) {
    mData = new(mHeap, 0) SpkData(pArchive);
    mData->loadTable(a2);
    mData->loadWave(a3);
}

void SpkSystem::framework() {
    SpkSpeakerCtrl::framework();
    mSoundHolder->framework();
}

void SpkSystem::startSound(s32 a1, s32 a2, SpkSoundHandle *pHandle) {
    mSoundHolder->startSound(a1, a2, pHandle);
}

void SpkSystem::newSoundMemPool(s32 a1) {
    JASPoolAllocObject<SpkSystem>::memPool_.newMemPool(0x60, a1);
}

void SpkSystem::connect(s32 chan) {
    SpkSpeakerCtrl::connect(chan);
}

void SpkSystem::disconnect(s32 chan) {
    SpkSpeakerCtrl::disconnect(chan);
}

void SpkSystem::reconnect(s32 chan) {
    if (chan < 0) {
        for (int i = 0; i < 4; i++) {
            SpkSpeakerCtrl::reconnect(i);
        }
    }
    else {
        SpkSpeakerCtrl::reconnect(chan);
    }
}

void SpkSystem::extensionProcess(s32 a1, s32 a2) {
    SpkSpeakerCtrl::extensionProcess(a1, a2);
}

f32 SpkSystem::getDeviceVolume(s32 chan) {
    return SpkSpeakerCtrl::getDeviceVolume(chan);
}