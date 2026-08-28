#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BindCone;

class BallOpener : public LiveActor {
public:
    BallOpener(const char*);

    /* 0x08 */ virtual ~BallOpener();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x48 */ virtual void control();
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeSetCenter();
    void exeOpen();
    void bindHole();

    BindCone* _8C;
    HitSensor* mSensor;  // 0x90
    TVec3f _94;
    TVec3f _A0;
    TVec3f _AC;
    TVec3f _B8;
    s32 _C4;
};
