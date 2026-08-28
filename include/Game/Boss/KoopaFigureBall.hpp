#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class Koopa;

namespace MR {
    struct ActorMoveParam;
};  // namespace MR

class KoopaFigureBall : public LiveActor {
public:
    KoopaFigureBall(const char* pName, Koopa* pKoopa, f32, const MR::ActorMoveParam* pMoveParam);

    /* 0x08 */ virtual ~KoopaFigureBall();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x30 */ virtual void makeActorDead();
    /* 0x38 */ virtual MtxPtr getBaseMtx() const;
    /* 0x48 */ virtual void control();

    /* 0x8C */ Koopa* mKoopa;
    /* 0x90 */ TVec3f mFront;
    /* 0x9C */ f32 mRadius;
    /* 0xA0 */ f32 mAngle;
    /* 0xA4 */ TPos3f mBaseMtx;
    /* 0xD4 */ const MR::ActorMoveParam* mMoveParam;
};
