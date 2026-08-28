#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MessageSensorHolder : public LiveActor {
public:
    MessageSensorHolder(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&) override;
};
