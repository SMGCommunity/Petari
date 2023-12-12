#pragma once

#include "Game/LiveActor/LiveActorGroup.h"
#include "Game/LiveActor/LiveActor.h"

class BigFan;

class BigFanHolder : public DeriveActorGroup<BigFan> {
public:
    BigFanHolder();

    virtual ~BigFanHolder();

    void calcWindInfo(TVec3f *, const TVec3f &, f32 *);
};

class BigFanFunction {
public:
    static void createBigFanHolder();
    static void calcWindInfo(TVec3f *, const TVec3f &, f32 *);
    static void registerBigFan(BigFan *);
};
