#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class BigFan;

class BigFanHolder : public DeriveActorGroup<BigFan> {
public:
    BigFanHolder();

    virtual ~BigFanHolder();

    void calcWindInfo(TVec3f*, const TVec3f&, f32*);
};

class BigFanFunction {
public:
    static void createBigFanHolder();
    static void calcWindInfo(TVec3f*, const TVec3f&, f32*);
    static void registerBigFan(BigFan*);
};
