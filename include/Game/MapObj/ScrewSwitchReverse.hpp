#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ScrewSwitchReverse : public LiveActor {
public:
    ScrewSwitchReverse(const char*);

    virtual void init(const JMapInfoIter&);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void updateBindActorMtx();
    void endBind();

    void exeWait();
    void exeAdjust();
    void exeScrew();

    /* 0x8C */ LiveActor* mHost;
    /* 0x90 */ bool mIsForceJump;  // Obj_arg0
};
