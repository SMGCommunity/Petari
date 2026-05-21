#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Koopa;

class KoopaFigureBall : public LiveActor {
public:
    KoopaFigureBall(const char*, Koopa*, f32, const MR::ActorMoveParam*);

    virtual ~KoopaFigureBall();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual MtxPtr getBaseMtx() const;
    virtual void control();

private:
    /* 0x8C */ Koopa* mKoopa;
    /* 0x90 */ TVec3f _90;
    /* 0x9C */ f32 _9C;
    /* 0xA0 */ f32 _A0;
    /* 0xA4 */ TPos3f _A4;
    /* 0xD4 */ const MR::ActorMoveParam* _D4;
};
