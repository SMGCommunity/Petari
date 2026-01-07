#pragma once

#include "Game/AudioLib/AudSoundObject.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"

class AudSoundObjHolder {
public:
    AudSoundObjHolder(JKRHeap* heap, long capacity);
    void update();
    void add(AudSoundObject* sound);
    void remove(AudSoundObject* sound);
    void moveOver(long initialIndex, long finalIndex);

    /* 0x00 */ AudSoundObject** mArray;  ///< AudSoundObject storage
    /* 0x04 */ long mCapacity;           ///< Physical capacity of mArray
    /* 0x08 */ long mSize;               ///< Logical capacity of mArray
};
