#pragma once

#include "Game/LiveActor/LiveActor.h"

class SnowplowSwitch : public LiveActor {
public:
    SnowplowSwitch(const char *);

    virtual ~SnowplowSwitch();
    virtual void init(const JMapInfoIter &);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    u8 _8C;
};