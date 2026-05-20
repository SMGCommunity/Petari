#include "Game/Boss/KoopaFigureBall.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"

KoopaFigureBall::KoopaFigureBall(const char* pName, Koopa* pKoopa, f32 myFloat, const MR::ActorMoveParam* pMoveParam)
    : LiveActor(pName), mKoopa(pKoopa), _90(0.0f, 0.0f, 1.0f), _9C(myFloat), _A0(), _D4(pMoveParam) {
    _A4.identity();
}

KoopaFigureBall::~KoopaFigureBall() {
}

void KoopaFigureBall::init(const JMapInfoIter& rIter) {
    initBinder(_9C, 0.0f, 0);

    MR::onCalcGravity(this);

    MR::invalidateClipping(this);

    makeActorDead();
}

void KoopaFigureBall::makeActorAppeared() {
    mPosition.set(mKoopa->mPosition);
    mVelocity.set(mKoopa->mVelocity);
    _90.set(KoopaFunction::getKoopaFront(mKoopa));

    MR::calcGravity(this);

    _A4.identity();

    MR::makeMtxFrontUpPos(&_A4, _90, -mGravity, mPosition);

    _A0 = 0.0f;

    LiveActor::makeActorAppeared();
}

void KoopaFigureBall::makeActorDead() {
    KoopaFunction::getKoopaFrontPtr(mKoopa)->set(_90);

    LiveActor::makeActorDead();
}

void KoopaFigureBall::control() {
    MR::moveAndTurnToPlayer(this, &_90, _D4->_0, _D4->_4, _D4->_8, _D4->_C);

    if (MR::isBindedWall(this)) {
        MR::addVelocityJump(this, 15.0f);
    }

    _A0 += (180.0f * mVelocity.dot(_90)) / (_9C * PI);

    MR::makeMtxUpFrontPos(&_A4, -mGravity, _90, mPosition + mGravity * _9C);

    TVec3f vec1;
    TVec3f vec2;
    _A4.getYDirInline(vec1);
    _A4.getZDirInline(vec2);
    _A4.setTrans(vec1 + vec2 * _9C);
}

MtxPtr KoopaFigureBall::getBaseMtx() const {
    return (MtxPtr)&_A4;
}
