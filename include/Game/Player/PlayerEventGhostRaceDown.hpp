#pragma once

#include "Game/Player/PlayerEvent.hpp"

class EventGhostRaceDown : public EventSequence {
public:
    EventGhostRaceDown();

    void init(u16 eventFrame, u16 sequenceFrame);
    void sound(u16 eventFrame, u16 sequenceFrame);
};
