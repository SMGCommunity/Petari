#include "Game/AudioLib/AudSoundObjHolder.hpp"

/// @brief Creates a new AudSoundObjHolder
AudSoundObjHolder::AudSoundObjHolder(JKRHeap* heap, long capacity) {
    mCapacity = capacity;
    mSize = 0;
    mArray = new (heap, 0) AudSoundObject*[capacity];
}

void AudSoundObjHolder::update() {
    for (int i = 0; i < mSize; i++) {
        mArray[i]->clearMapCode();
    }
}

void AudSoundObjHolder::add(AudSoundObject* sound) {
    if (mSize < mCapacity) {
        mArray[mSize] = sound;
        mSize++;
    }
};

void AudSoundObjHolder::remove(AudSoundObject* sound) {
    int soundIndex = -1;

    // Find sound object index in mArray
    for (int i = mSize - 1; i >= 0; i--) {
        if (mArray[i] == sound) {
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

/// @brief Shifts to the right sounds from initialIndex to finalIndex in mArray to replace AudSoundObject stored in initialIndex
void AudSoundObjHolder::moveOver(long initialIndex, long finalIndex) {
    for (int i = initialIndex; i < finalIndex; i++) {
        mArray[i] = mArray[i - 1];
    }
}
