#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class LavaGeyser : public LiveActor {
public:
    LavaGeyser(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void startClipped();
    virtual void updateHitSensor(HitSensor*);
    virtual void attackSensor(HitSensor*, HitSensor*);

    /* 8C */ s32 _8c;
    /* 90 */ s32 _90;
    /* 94 */ TVec3f _94;
    /* A0 */ TVec3f _A0;
    /* AC */ f32 _AC;
    /* B0 */ f32 _B0;  // one of these may be a different data type
    /* B4 */ f32 _B4;
    /* B8 */ f32 _B8;

    void exeWait();
    void exeWaitSwitch();
    void exeSign();
    void exeShootUp();
    void exeShootKeep();
    void exeShootDown();
};
