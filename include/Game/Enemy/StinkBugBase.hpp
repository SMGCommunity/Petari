#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class HitSensor;

class StinkBugBase : public LiveActor {
public:
    StinkBugBase(const char*);

    virtual void init(const JMapInfoIter&);

    void setDashVelocity(f32);
    bool tryTurnSearch(f32);
    bool tryTurnDashSign(f32);
    void fixInitPos();
    bool isPlayerInTerritory(f32, f32, f32, f32) const;
    bool isHitHorn(HitSensor*, HitSensor*, f32) const;

    TVec3f _8C;
    TVec3f _98;
    TVec3f _A4;
    f32 _B0;
    /* 0xB4 */ f32 mRadius;
    bool _B8;
};
