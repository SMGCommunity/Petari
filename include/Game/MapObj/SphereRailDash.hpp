#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SphereRailDash : public LiveActor {
public:
    SphereRailDash(const char*);

    virtual ~SphereRailDash();
    virtual void init(const JMapInfoIter&);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeRailMove();

    /* 0x08C */ HitSensor* _8C;
    /* 0x090 */ TVec3f _90;
    /* 0x09C */ f32 _9C;
    /* 0x0A0 */ f32 _A0;
    /* 0x0A4 */ TVec3f _A4;
    /* 0x0B0 */ f32 _B0;
    /* 0x0B4 */ f32 _B4;
    /* 0x0B8 */ f32 _B8;
    /* 0x0BC */ s32 _BC;
};
