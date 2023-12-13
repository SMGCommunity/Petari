#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MessageSensorHolder : public LiveActor {
public:
    MessageSensorHolder(const char *);

    virtual ~MessageSensorHolder();
    virtual void init(const JMapInfoIter &);
};