#pragma once

#include "Game/Player/PlayerEvent.hpp"

class EventAbyss : public EventSequence {
public:
    EventAbyss();

    void init(u16 eventFrame, u16 sequenceFrame);
    void sound(u16 eventFrame, u16 sequenceFrame);
    void updateAfter();

    /* 0x24 */ s32 _24;
    /* 0x28 */ f32 _28;
};
