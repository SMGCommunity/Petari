#include "Game/Boss/KoopaFigureBall.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"

void KoopaFigureBall_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)MR::pi();
}

namespace {
    static const f32 sWallJumpSpeed = 15.0f;
};  // namespace

KoopaFigureBall::~KoopaFigureBall() {
}

KoopaFigureBall::KoopaFigureBall(const char* pName, Koopa* pKoopa, f32 myFloat, const MR::ActorMoveParam* pMoveParam)
    : LiveActor(pName), mKoopa(pKoopa), mFront(0.0f, 0.0f, 1.0f), mRadius(myFloat), mAngle(), mMoveParam(pMoveParam) {
    mBaseMtx.identity();
}

void KoopaFigureBall::init(const JMapInfoIter& rIter) {
    initBinder(mRadius, 0.0f, 0);

    MR::onCalcGravity(this);

    MR::invalidateClipping(this);

    makeActorDead();
}

void KoopaFigureBall::makeActorAppeared() {
    mPosition.set(mKoopa->mPosition);
    mVelocity.set(mKoopa->mVelocity);
    mFront.set(KoopaFunction::getKoopaFront(mKoopa));

    MR::calcGravity(this);

    mBaseMtx.identity();

    MR::makeMtxFrontUpPos(&mBaseMtx, mFront, -mGravity, mPosition);

    mAngle = 0.0f;

    LiveActor::makeActorAppeared();
}

void KoopaFigureBall::makeActorDead() {
    KoopaFunction::getKoopaFrontPtr(mKoopa)->set(mFront);
    LiveActor::makeActorDead();
}

void KoopaFigureBall::control() {
    MR::moveAndTurnToPlayer(this, &mFront, mMoveParam->_0, mMoveParam->_4, mMoveParam->_8, mMoveParam->_C);

    if (MR::isBindedWall(this)) {
        MR::addVelocityJump(this, ::sWallJumpSpeed);
    }

    mAngle += (mVelocity.dot(mFront) * 180.0f) / (mRadius * MR::pi());
    MR::makeMtxUpFrontPos(&mBaseMtx, -mGravity, mFront, mPosition + mGravity * mRadius);

    TVec3f yDir;
    TVec3f newTrans;
    mBaseMtx.getYDir(yDir);
    mBaseMtx.getTrans(newTrans);
    newTrans += yDir * mRadius;
    mBaseMtx.setTrans(newTrans);

    TPos3f rotMtx;
    rotMtx.makeRotate(TVec3f(1.0f, 0.0f, 0.0f), MR::toRadian(mAngle));
    mBaseMtx.concat(mBaseMtx, rotMtx);
}

MtxPtr KoopaFigureBall::getBaseMtx() const {
    return (MtxPtr)&mBaseMtx;
}
