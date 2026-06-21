#pragma once

#include "Game/Player/PlayerEvent.hpp"

class EventDown : public EventSequence {
public:
    EventDown();

    void init(u16 eventFrame, u16 sequenceFrame);
    void sound(u16 eventFrame, u16 sequenceFrame);
    void missLayoutOpen(u16 eventFrame, u16 sequenceFrame);
};
