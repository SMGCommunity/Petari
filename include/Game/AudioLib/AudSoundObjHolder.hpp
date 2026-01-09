#pragma once

#include <revolution/types.h>

class AudSoundObject;
class JKRHeap;

class AudSoundObjHolder {
public:
    /// @brief Creates a new AudSoundObjHolder
    AudSoundObjHolder(JKRHeap* pHeap, s32 capacity);
    void update();
    void add(AudSoundObject* pSound);
    void remove(AudSoundObject* pSsound);
    /// @brief Shifts to the left sounds from initialIndex to finalIndex in mArray to replace AudSoundObject stored in initialIndex
    void moveOver(s32 initialIndex, s32 finalIndex);

    /* 0x00 */ AudSoundObject** mArray;  ///< AudSoundObject storage
    /* 0x04 */ s32 mCapacity;            ///< Physical capacity of mArray
    /* 0x08 */ s32 mSize;                ///< Logical capacity of mArray
};
