#pragma once

#include "Game/LiveActor/LiveActor.hpp"

static f32 zero = 0.0f;

class ShootingStar : public LiveActor {
public:
    ShootingStar(const char*);

    virtual ~ShootingStar();
    virtual void init(const JMapInfoIter&);
    virtual void control();

    void appearPreShooting();
    void exePreShooting();
    void exeShooting();
    void exeWaitForNextShoot();

    TVec3f _8C;
    TVec3f _98;
    TVec3f _A4;
    s32 _B0;
    s32 _B4;
    f32 _B8;
};
