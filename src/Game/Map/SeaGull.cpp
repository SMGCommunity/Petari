#include "Game/Map/SeaGull.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

void SeaGull_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)-1.0f;
}

namespace NrvSeaGull {
    NEW_NERVE(SeaGullNrvHoverFront, SeaGull, HoverFront);
    NEW_NERVE(SeaGullNrvHoverLeft, SeaGull, HoverLeft);
    NEW_NERVE(SeaGullNrvHoverRight, SeaGull, HoverRight);
}  // namespace NrvSeaGull

SeaGull::SeaGull(SeaGullGroup* pGroup)
    : LiveActor("カモメ"), mSeaGullGroup(pGroup), mPointIndex(), mIsReverse(), mTargetUpdateTimer(MR::getRandom(0L, 180L)), mTargetPosition(),
      mUp(0.0f, 1.0f, 0.0f), mFront(0.0f, 0.0f, 1.0f), mBankedUp(0.0f, 1.0f, 0.0f), mSide(1.0f, 0.0f, 0.0f), mHoverTimer(), mBankAngle(),
      mGlideTimer(), mLiftTimer(), mChirpTimer() {
}

void SeaGull::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::calcActorAxis(&mSide, &mBankedUp, &mFront, this);
    MR::calcActorAxisY(&mUp, this);
    initModelManagerWithAnm("SeaGull", nullptr, false);
    MR::startBck(this, "Fly");
    MR::setBckFrameAtRandom(this);
    MR::connectToSceneEnvironment(this);
    f32 railLength = MR::getRailTotalLength(mSeaGullGroup);
    f32 maxRailCoord = railLength - 1.0f;
    f32 railCoord = MR::getRandom(1.0f, maxRailCoord);
    mPointIndex = railCoord / 500.0f;
    MR::calcRailPosAtCoord(&mPosition, mSeaGullGroup, railCoord);
    mIsReverse = MR::isHalfProbability();
    mTargetPosition = mSeaGullGroup->updatePosInfo(&mPointIndex, mIsReverse);

    TVec3f side(mSide);
    TVec3f forward(mFront);
    f32 scale = MR::getRandom(-1.0f, 1.0f);
    side.scale(scale);
    scale = MR::getRandom(-1.0f, 1.0f);
    forward.scale(scale);
    mFront.set(side);
    mFront.add(forward);

    if (MR::isNearZero(mFront)) {
        mFront.x = 0.0f;
        mFront.y = 0.0f;
        mFront.z = 1.0f;
    } else {
        MR::normalize(&mFront);
    }

    initNerve(GET_NERVE(SeaGull, SeaGullNrvHoverFront));
    initSound(4, false);
    mChirpTimer = MR::getRandom(60L, 480L);
    MR::invalidateClipping(this);
    makeActorAppeared();
}

void SeaGull::exeHoverFront() {
    if (MR::isFirstStep(this)) {
        mHoverTimer = MR::getRandom(0L, 60L);
    }

    mBankAngle *= 0.995f;

    if (mHoverTimer >= 0) {
        mHoverTimer--;
    } else {
        TVec3f forward(*mTargetPosition);
        forward.sub(mPosition);

        if (mPosition.distance(*mTargetPosition) > 500.0f) {
            f32 prod = mSide.dot(forward);

            if (prod > 0.0f) {
                setNerve(GET_NERVE(SeaGull, SeaGullNrvHoverLeft));
            } else {
                setNerve(GET_NERVE(SeaGull, SeaGullNrvHoverRight));
            }
        }
    }
}

void SeaGull::exeHoverLeft() {
    if (MR::isFirstStep(this)) {
        mHoverTimer = MR::getRandom(60L, 120L);
    }

    mBankAngle -= 0.1f;

    if (MR::isStep(this, mHoverTimer)) {
        setNerve(GET_NERVE(SeaGull, SeaGullNrvHoverFront));
    }
}

void SeaGull::exeHoverRight() {
    if (MR::isFirstStep(this)) {
        mHoverTimer = MR::getRandom(60L, 120L);
    }

    mBankAngle += 0.1f;

    if (MR::isStep(this, mHoverTimer)) {
        setNerve(GET_NERVE(SeaGull, SeaGullNrvHoverFront));
    }
}

void SeaGull::control() {
    updateHover();

    mVelocity.mult(0.99f);

    f32 mag = mVelocity.length();

    if (mag > 10.0f) {
        MR::normalize(&mVelocity);
        mVelocity.mult(10.0f);
    }

    mSide.cross(mBankedUp, mFront);
    MR::normalize(&mSide);
    mBankedUp.cross(mFront, mSide);
    MR::normalize(&mBankedUp);
    mTargetUpdateTimer--;

    if (mTargetUpdateTimer <= 0) {
        mTargetPosition = mSeaGullGroup->updatePosInfo(&mPointIndex, mIsReverse);
        mTargetUpdateTimer = 0xB4;
    }

    if (mChirpTimer <= 0) {
        MR::startSound(this, "SE_OJ_SEAGULL_CHIRP");
        mChirpTimer = MR::getRandom(60L, 480L);
    } else {
        mChirpTimer--;
    }

    if (MR::isJudgedToClipFrustum300m(mPosition, 200.0f)) {
        if (!MR::isHiddenModel(this)) {
            MR::hideModel(this);
        }
    } else {
        if (MR::isHiddenModel(this)) {
            MR::showModel(this);
        }
    }
}

void SeaGull::updateHover() {
    if (MR::abs(mBankAngle) > 0.01f) {
        mUp.set(mGravity);
        mUp.mult(-1.0f);
        mBankAngle = MR::clamp(mBankAngle, -30.0f, 30.0f);

        TPos3f rotation;
        rotation.identity();
        const f32 angle = mBankAngle;
        rotation.setRotate(mFront, PI_180 * angle);
        mBankedUp.set(mUp);
        rotation.mult(mBankedUp, mBankedUp);
        rotation.setRotate(mUp, -(PI_180 * (0.01f * mBankAngle)));
        rotation.mult(mFront, mFront);
    }

    TVec3f forward(mFront);
    forward.scale(0.05f);
    mVelocity.add(forward);

    if (mLiftTimer > 0) {
        TVec3f lift(mBankedUp);
        lift.scale(0.04f);
        mVelocity.add(lift);
        mLiftTimer--;

        if (mLiftTimer <= 0) {
            mGlideTimer = MR::getRandom(60L, 300L);
        }
    } else {
        mVelocity.y -= 0.005f;
        TVec3f toTarget(*mTargetPosition);
        toTarget.sub(mPosition);

        f32 height = toTarget.dot(mUp);
        if (height < 500.0f) {
            mGlideTimer = 300;
        } else {
            mGlideTimer--;

            if (height > 500.0f || mGlideTimer <= 0) {
                mLiftTimer = MR::getRandom(30L, 180L);
            }
        }
    }
}

void SeaGull::calcAndSetBaseMtx() {
    TPos3f mtx;
    mtx.identity();
    mtx.mMtx[0][0] = mSide.x;
    mtx.mMtx[1][0] = mSide.y;
    mtx.mMtx[2][0] = mSide.z;
    mtx.mMtx[0][1] = mBankedUp.x;
    mtx.mMtx[1][1] = mBankedUp.y;
    mtx.mMtx[2][1] = mBankedUp.z;
    mtx.mMtx[0][2] = mFront.x;
    mtx.mMtx[1][2] = mFront.y;
    mtx.mMtx[2][2] = mFront.z;
    mtx.mMtx[0][3] = mPosition.x;
    mtx.mMtx[1][3] = mPosition.y;
    mtx.mMtx[2][3] = mPosition.z;
    MR::setBaseTRMtx(this, mtx);
}

SeaGullGroup::SeaGullGroup(const char* pName) : LiveActor(pName), mPointCount(), mPoints() {
}

void SeaGullGroup::init(const JMapInfoIter& rIter) {
    s32 birdCount = 10;
    MR::getJMapInfoArg0NoInit(rIter, &birdCount);
    initRailRider(rIter);
    mPosition.set(MR::getRailPos(this));
    mPointCount = static_cast< s32 >(MR::getRailTotalLength(this) / 500.0f) + 1;
    f32 interval = MR::getRailTotalLength(this) / mPointCount;
    mPoints = new TVec3f[mPointCount];

    for (s32 i = 0; i < mPointCount; i++) {
        MR::calcRailPosAtCoord(&mPoints[i], this, interval * i);
    }

    for (s32 i = 0; i < birdCount; i++) {
        SeaGull* pSeaGull = new SeaGull(this);
        pSeaGull->init(rIter);
    }

    MR::invalidateClipping(this);
    makeActorAppeared();
}

TVec3f* SeaGullGroup::updatePosInfo(s32* pPointIndex, bool isReverse) const {
    if (isReverse) {
        (*pPointIndex)--;

        if (*pPointIndex <= 0) {
            *pPointIndex = mPointCount - 1;
        }
    } else {
        s32 val = mPointCount;
        s32 next = *pPointIndex + 1;
        *pPointIndex = next;

        if (next >= val) {
            *pPointIndex = 0;
        }
    }

    return &mPoints[*pPointIndex];
}

SeaGull::~SeaGull() {
}

SeaGullGroup::~SeaGullGroup() {
}
