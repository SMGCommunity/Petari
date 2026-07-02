#pragma once

#include "Game/Player/PlayerEvent.hpp"

class EventGameOver : public EventSequence {
public:
    EventGameOver();

    void init(u16 eventFrame, u16 sequenceFrame);
    void sound(u16 eventFrame, u16 sequenceFrame);
    void updateAfter();
    void reqLayout(u16 eventFrame, u16 sequenceFrame);
    void finish(u16 eventFrame, u16 sequenceFrame);

    /* 0x24 */ s32 _24;
    /* 0x28 */ f32 _28;
};
