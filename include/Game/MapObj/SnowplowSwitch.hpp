#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SnowplowSwitch : public LiveActor {
public:
    SnowplowSwitch(const char*);

    virtual ~SnowplowSwitch();
    virtual void init(const JMapInfoIter&);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    /* 0x8C */ bool mSnowDiscovered;

    // Fill word alignment. Not used.
    /* 0x8D */ u8 _8D[3];
};
