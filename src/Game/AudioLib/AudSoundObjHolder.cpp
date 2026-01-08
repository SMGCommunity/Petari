#include "Game/AudioLib/AudSoundObjHolder.hpp"

#include "Game/AudioLib/AudSoundObject.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>

AudSoundObjHolder::AudSoundObjHolder(JKRHeap* pHeap, long capacity) {
    mCapacity = capacity;
    mSize = 0;
    mArray = new (pHeap, 0) AudSoundObject*[capacity];
}

void AudSoundObjHolder::update() {
    for (int i = 0; i < mSize; i++) {
        mArray[i]->clearMapCode();
    }
}

void AudSoundObjHolder::add(AudSoundObject* pSound) {
    if (mSize < mCapacity) {
        mArray[mSize] = pSound;
        mSize++;
    }
};

void AudSoundObjHolder::remove(AudSoundObject* pSound) {
    int soundIndex = -1;

    // Find sound object index in mArray
    for (int i = mSize - 1; i >= 0; i--) {
        if (mArray[i] == pSound) {
            soundIndex = i;
            break;
        }
    }

    // Remove sound object if found
    if (soundIndex >= 0) {
        moveOver(soundIndex, mSize - 1);
        mSize--;
    }
}

void AudSoundObjHolder::moveOver(long initialIndex, long finalIndex) {
    for (int i = initialIndex; i < finalIndex; i++) {
        mArray[i] = mArray[i + 1];
    }
}
