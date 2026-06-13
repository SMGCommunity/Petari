#include "Game/Speaker/SpkSystem.hpp"
#include "Game/Speaker/SpkSpeakerCtrl.hpp"

SpkSystem::SpkSystem(JKRHeap* pHeap) : JASGlobalInstance(true), mData(nullptr) {
    if (pHeap == nullptr) {
        pHeap = JKRGetCurrentHeap();
    }

    mHeap = pHeap;
    mMixBuffer = new (pHeap, 0) SpkMixingBuffer(pHeap);
    mSoundHolder = new (pHeap, 0) SpkSoundHolder();
    SpkSpeakerCtrl::setup();
    SpkSpeakerCtrl::setMixingBuffer(mMixBuffer);
}

void DUMMY() {
    // NOTE: JSUList<SpkSound> dtor is being emitted here, as well as JASGlobalInstance<SpkSystem>. Unsure as to exactly why
    SpkSoundHolder dummy;
}

void SpkSystem::setResource(JKRArchive* pArchive, u16 tableOffset, u16 waveOffset) {
    mData = new (mHeap, 0) SpkData(pArchive);
    mData->loadTable(tableOffset);
    mData->loadWave(waveOffset);
}

void SpkSystem::framework() {
    SpkSpeakerCtrl::framework();
    mSoundHolder->framework();
}

void SpkSystem::startSound(s32 padChannel, s32 waveID, SpkSoundHandle* pHandle) {
    mSoundHolder->startSound(padChannel, waveID, pHandle);
}

void SpkSystem::newSoundMemPool(s32 n) {
    JASPoolAllocObject< SpkSound >::memPool_.newMemPool(n);
}

void SpkSystem::connect(s32 padChannel) {
    SpkSpeakerCtrl::connect(padChannel);
}

void SpkSystem::disconnect(s32 padChannel) {
    SpkSpeakerCtrl::disconnect(padChannel);
}

void SpkSystem::reconnect(s32 padChannel) {
    if (padChannel < 0) {
        for (int i = 0; i < 4; i++) {
            SpkSpeakerCtrl::reconnect(i);
        }
    } else {
        SpkSpeakerCtrl::reconnect(padChannel);
    }
}

void SpkSystem::extensionProcess(s32 a1, s32 a2) {
    SpkSpeakerCtrl::extensionProcess(a1, a2);
}

f32 SpkSystem::getDeviceVolume(s32 padChannel) {
    return SpkSpeakerCtrl::getDeviceVolume(padChannel);
}
