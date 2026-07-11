#include "Game/Boss/KoopaFigureBall.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"

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

// 89% and a big mess
void KoopaFigureBall::control() {
    MR::moveAndTurnToPlayer(this, &mFront, mMoveParam->_0, mMoveParam->_4, mMoveParam->_8, mMoveParam->_C);

    if (MR::isBindedWall(this)) {
        MR::addVelocityJump(this, ::sWallJumpSpeed);
    }

    f32 angle = MR::toDegree(mVelocity.dot(mFront) / mRadius);
    mAngle += angle;
    TVec3f direction = mGravity * angle;
    TVec3f vec = mPosition;
    vec += direction;
    MR::makeMtxUpFrontPos(&mBaseMtx, -mGravity, mFront, vec);

    TVec3f vec1;
    TVec3f vec2;
    mBaseMtx.getYDirInline(vec1);
    mBaseMtx.getTransInline(vec2);
    vec2 += vec1 * mRadius;
    mBaseMtx.setTrans(vec2);

    TPos3f mtx;
    mtx.makeRotate(TVec3f(1.0f, 0.0f, 0.0f), MR::toRadian(mAngle));
    mBaseMtx.concat(mBaseMtx, mtx);
}

MtxPtr KoopaFigureBall::getBaseMtx() const {
    return (MtxPtr)&mBaseMtx;
}
