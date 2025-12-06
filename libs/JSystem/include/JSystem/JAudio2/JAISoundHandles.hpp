#pragma once
#include "Game/AudioLib/AudSoundObject.hpp"
#include "Game/GameAudio/AudTalkSoundData.hpp"

class JAISoundHandle;

class JAISoundHandles {
public:
    u32* getHandleSoundID(JAISoundID);  // exact return type currently unknown

    /* 0x0 */ JAISoundHandle* mHandles;
    u32 _4;
};
