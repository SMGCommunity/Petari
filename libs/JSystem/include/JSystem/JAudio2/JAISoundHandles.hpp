#pragma once
#include "Game/GameAudio/AudTalkSoundData.hpp"
#include "Game/AudioLib/AudSoundObject.hpp"

class JAISoundHandle;

class JAISoundHandles {
public:
    u32* getHandleSoundID(JAISoundID); // exact return type currently unknown

    /* 0x0 */ JAISoundHandle* mHandles;
    u32                       _4;
};
