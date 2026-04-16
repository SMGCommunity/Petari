#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution.h>

class KameckBeamEventListener;
class KameckBeam;
class KameckTurtle;
class KameckFireBall;

class KameckBeamHolder : public DeriveActorGroup< KameckBeam > {
public:
    KameckBeamHolder();

    virtual ~KameckBeamHolder();
};

class KameckFireBallHolder : public DeriveActorGroup< KameckFireBall > {
public:
    KameckFireBallHolder();

    virtual ~KameckFireBallHolder();
};

class KameckBeamTurtleHolder : public DeriveActorGroup< KameckTurtle > {
public:
    KameckBeamTurtleHolder();

    virtual ~KameckBeamTurtleHolder();
};

namespace MR {
    void createKameckBeamHolder();
    void createKameckFireBallHolder();
    void createKameckBeamTurtleHolder();

    void setKameckBeamCollisionFilter(LiveActor*);

    KameckBeam* startFollowKameckBeam(s32, MtxPtr, f32, const TVec3f&, KameckBeamEventListener*);
    KameckBeam* getKameckBeam();
};  // namespace MR
