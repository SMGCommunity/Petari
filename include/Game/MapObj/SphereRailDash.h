#pragma once

#include "Game/LiveActor/LiveActor.h"

class SphereRailDash : public LiveActor {
public:
    SphereRailDash(const char *);

    virtual ~SphereRailDash();
    virtual void init(const JMapInfoIter &);
    virtual u32 receiveOtherMsg(u32, HitSensor *, HitSensor *);

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

namespace NrvSphereRailDash {
    NERVE(SphereRailDashNrvWait);
    NERVE(SphereRailDashNrvRailMove);
};