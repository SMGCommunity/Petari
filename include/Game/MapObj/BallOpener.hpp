#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BindCone;

class BallOpener : public LiveActor {
public:
    BallOpener(const char*);

    virtual ~BallOpener();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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
