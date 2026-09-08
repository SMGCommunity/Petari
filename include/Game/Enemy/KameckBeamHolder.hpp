#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class KameckBeam;
class KameckBeamEventListener;
class KameckFireBall;
class KameckTurtle;

class KameckBeamHolder : public DeriveActorGroup< KameckBeam > {
public:
    /// @brief Creates a new `KameckBeamHolder`.
    KameckBeamHolder();
};

class KameckFireBallHolder : public DeriveActorGroup< KameckFireBall > {
public:
    /// @brief Creates a new `KameckFireBallHolder`.
    KameckFireBallHolder();
};

class KameckBeamTurtleHolder : public DeriveActorGroup< KameckTurtle > {
public:
    /// @brief Creates a new `KameckBeamTurtleHolder`.
    KameckBeamTurtleHolder();
};

namespace MR {
    void createKameckBeamHolder();
    void createKameckFireBallHolder();
    void createKameckBeamTurtleHolder();
    KameckBeam* startFollowKameckBeam(s32, MtxPtr, f32, const TVec3f&, KameckBeamEventListener*);
    KameckBeam* getKameckBeam();
    KameckFireBall* getKameckFireBall();
    KameckTurtle* getKameckBeamTurtle();
};  // namespace MR
