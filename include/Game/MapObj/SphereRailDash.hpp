#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SphereRailDash : public LiveActor {
public:
    SphereRailDash(const char*);

    /* 0x08 */ virtual ~SphereRailDash();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeRailMove();

    HitSensor* _8C;
    TVec3f _90;
    f32 _9C;
    f32 _A0;
    TVec3f _A4;
    f32 _B0;
    f32 _B4;
    f32 _B8;
    u32 _BC;
};
