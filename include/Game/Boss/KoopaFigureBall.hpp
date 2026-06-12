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

    virtual ~KoopaFigureBall();
    virtual void init(const JMapInfoIter& rIter);
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual MtxPtr getBaseMtx() const;
    virtual void control();

    /* 0x8C */ Koopa* mKoopa;
    /* 0x90 */ TVec3f mFront;
    /* 0x9C */ f32 mRadius;
    /* 0xA0 */ f32 mAngle;
    /* 0xA4 */ TPos3f mBaseMtx;
    /* 0xD4 */ const MR::ActorMoveParam* mMoveParam;
};
