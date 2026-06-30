#pragma once

#include "Game/Player/PlayerEvent.hpp"

class EventRaceDown : public EventSequence {
public:
    EventRaceDown();

    void init(u16 eventFrame, u16 sequenceFrame);
    void sound(u16 eventFrame, u16 sequenceFrame);
    bool checkCloseWipeStart(u16 sequenceFrame);
    void missLayoutOpen(u16 eventFrame, u16);
};
