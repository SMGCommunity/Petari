#include "Game/Map/FishGroup.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"

void FishGroup_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)MR::epsilon();
}

namespace {
    static const s32 sDefaultFishNum = 10;
    static const f32 sDefaultGroupSpeed = 5.0f;
    static const f32 sFriction = 0.95f;
    static const f32 sWanderAccel = 1.0f;
    static const s32 sStepToWanderMoveMin = 60;
    static const s32 sStepToWanderMoveMax = 180;
    static const f32 sApproachAccel = 5.0f;
    static const s32 sStepToApproachMin = 5;
    static const s32 sStepToApproachMax = 30;
    static const f32 sDistanceToApproachMin = 100.0f;
    static const f32 sDistanceToApproachMax = 500.0f;
    static const f32 sEscapeAccel = 5.0f;
    static const s32 sStepToEscapeMin = 5;
    static const s32 sStepToEscapeMax = 20;
    static const f32 sDistanceSafe = 600.0f;
    static const f32 sDistanceToSpinEscape = 500.0f;
    static const s32 sTimeToSearchPlayerMin = 5;
    static const s32 sTimeToSearchPlayerMax = 10;
    static const f32 sDistanceToEscapeFromPlayer = 150.0f;
    static const f32 sRotateDotMax = 0.9f;
    static const f32 sRotateRate = 0.2f;
    static const f32 sDirectionMaxY = 0.99f;
    static const f32 sAnimFrameRate = 0.2f;
    static const f32 sFollowPosOffsetMax = 150.0f;
};  // namespace

namespace NrvFish {
    NEW_NERVE(FishNrvWander, Fish, Wander);
    NEW_NERVE(FishNrvApproach, Fish, Approach);
    NEW_NERVE(FishNrvEscape, Fish, Escape);
};  // namespace NrvFish

Fish::~Fish() {
}

FishGroup::~FishGroup() {
}

Fish::Fish(const char* pName, FishGroup* pGroup, int fishType, bool canEscape)
    : LiveActor(pName), mGroup(pGroup), mFishType(fishType), mCanEscape(canEscape), mFollowPos(0.0f, 0.0f, 0.0f),
      mFollowPosOffset(MR::getRandom(-::sFollowPosOffsetMax, ::sFollowPosOffsetMax), MR::getRandom(-::sFollowPosOffsetMax, ::sFollowPosOffsetMax),
                       MR::getRandom(-::sFollowPosOffsetMax, ::sFollowPosOffsetMax)),
      mFront(1.0f, 0.0f, 0.0f), mDistanceToApproach(MR::getRandom(::sDistanceToApproachMin, ::sDistanceToApproachMax)),
      mTimeToSearchPlayer(MR::getRandom(::sTimeToSearchPlayerMin, ::sTimeToSearchPlayerMax)), mStepToMove(), mEscapePos() {
}

void Fish::init(const JMapInfoIter& rIter) {
    updateFollowPointPos();
    mPosition.set(mFollowPos);
    mFront.set(MR::getRailDirection(mGroup));

    const char* names[] = {"FishA", "FishB", "FishC", "FishD", "FishE", "FishF"};
    initModelManagerWithAnm(names[mFishType], nullptr, false);
    MR::startBck(this, "Swim", nullptr);
    MR::connectToSceneEnvironment(this);
    initNerve(GET_NERVE(Fish, FishNrvWander));
    MR::invalidateClipping(this);
    makeActorAppeared();
}

void Fish::exeWander() {
    if (MR::isFirstStep(this)) {
        mStepToMove = 0;
    }

    if (tryEscapeFromPlayer()) {
        return;
    }

    if (--mStepToMove <= 0) {
        mVelocity.x += mFront.x * ::sWanderAccel;
        mVelocity.y += mFront.y * ::sWanderAccel;
        mVelocity.z += mFront.z * ::sWanderAccel;
        mStepToMove = MR::getRandom(::sStepToWanderMoveMin, ::sStepToWanderMoveMax);
    }

    if (mFollowPos.squared(mPosition) > mDistanceToApproach * mDistanceToApproach) {
        setNerve(GET_NERVE(Fish, FishNrvApproach));
    }
}

void Fish::exeApproach() {
    if (MR::isFirstStep(this)) {
        mStepToMove = 0;
    }

    if (tryEscapeFromPlayer()) {
        return;
    }

    if (--mStepToMove <= 0) {
        TVec3f front = mFollowPos;
        front.sub(mPosition);
        MR::normalize(&front);

        if (mFront.dot(front) > ::sRotateDotMax) {
            mFront.set(front);
        } else {
            TVec3f currFront = mFront;
            mFront.x = currFront.x * (1.0f - ::sRotateRate) + front.x * ::sRotateRate;
            mFront.y = currFront.y * (1.0f - ::sRotateRate) + front.y * ::sRotateRate;
            mFront.z = currFront.z * (1.0f - ::sRotateRate) + front.z * ::sRotateRate;

            if (MR::isNearZero(mFront, 0.01f)) {
                mFront.set(front);
            } else {
                MR::normalize(&mFront);
            }
        }

        mVelocity.x += mFront.x * ::sApproachAccel;
        mVelocity.y += mFront.y * ::sApproachAccel;
        mVelocity.z += mFront.z * ::sApproachAccel;
        mStepToMove = MR::getRandom(::sStepToApproachMin, ::sStepToApproachMax);
    }

    if (mFollowPos.squared(mPosition) < mDistanceToApproach * mDistanceToApproach) {
        setNerve(GET_NERVE(Fish, FishNrvWander));
    }
}

void Fish::exeEscape() {
    if (MR::isFirstStep(this)) {
        mStepToMove = 0;
    }

    if (--mStepToMove <= 0) {
        TVec3f front = mPosition;
        front.sub(*mEscapePos);
        MR::normalize(&front);

        mVelocity.x += front.x * ::sEscapeAccel;
        mVelocity.y += front.y * ::sEscapeAccel;
        mVelocity.z += front.z * ::sEscapeAccel;

        mFront.set(mVelocity);
        MR::normalize(&mFront);

        mStepToMove = MR::getRandom(::sStepToEscapeMin, ::sStepToEscapeMax);
    }

    if (mEscapePos->squared(mPosition) > ::sDistanceSafe * ::sDistanceSafe) {
        setNerve(GET_NERVE(Fish, FishNrvApproach));
    }
}

void Fish::control() {
    mVelocity.scale(::sFriction);
    if (!mVelocity.isZero()) {
        MR::setBckRate(this, mVelocity.length() * ::sAnimFrameRate);
    }

    if (mCanEscape && MR::isPadSwing(WPAD_CHAN0)) {
        if (!isNerve(GET_NERVE(Fish, FishNrvEscape)) && MR::calcDistanceToPlayer(this) < ::sDistanceToSpinEscape) {
            mEscapePos = MR::getPlayerPos();
            setNerve(GET_NERVE(Fish, FishNrvEscape));
        }
    }

    if (MR::isNearZero(mFront)) {
        if (MR::isNearZero(mVelocity)) {
            mFront.set(1.0f, 0.0f, 0.0f);
        } else {
            mFront.set(mVelocity);
        }
    }

    if (MR::abs(mFront.y) > ::sDirectionMaxY) {
        if (mFront.x > 0.0f) {
            mFront.x += 0.1f;
        } else {
            mFront.x -= 0.1f;
        }
        MR::normalize(&mFront);
    }

    updateFollowPointPos();

    if (MR::isJudgedToClipFrustum300m(mPosition, 100.0f)) {
        if (!MR::isHiddenModel(this)) {
            MR::hideModel(this);
        }
    } else {
        if (MR::isHiddenModel(this)) {
            MR::showModel(this);
        }
    }
}

void Fish::calcAndSetBaseMtx() {
    TPos3f mtx;
    mtx.identity();
    MR::makeMtxFrontUp(&mtx, mFront, mGroup->mUp);
    mtx.setTrans(mPosition);
    MR::setBaseTRMtx(this, mtx);
}

bool Fish::tryEscapeFromPlayer() {
    if (!mCanEscape) {
        return false;
    }

    if (--mTimeToSearchPlayer <= 0) {
        if (MR::calcDistanceToPlayer(this) < ::sDistanceToEscapeFromPlayer) {
            mEscapePos = MR::getPlayerPos();
            setNerve(GET_NERVE(Fish, FishNrvEscape));
            return true;
        }
        mTimeToSearchPlayer = MR::getRandom(::sTimeToSearchPlayerMin, ::sTimeToSearchPlayerMax);
    }

    return false;
}

void Fish::updateFollowPointPos() {
    mFollowPos.set(MR::getRailPos(mGroup));
    mFollowPos.add(mFollowPosOffset);
}

FishGroup::FishGroup(const char* pName)
    : LiveActor(pName), mUp(0.0f, 1.0f, 0.0f), mFishNum(::sDefaultFishNum), mFish(), mSpeed(::sDefaultGroupSpeed), mClippingCenter(0.0f, 0.0f, 0.0f) {
}

void FishGroup::init(const JMapInfoIter& rIter) {
    MR::connectToSceneEnemyMovement(this);
    MR::initDefaultPos(this, rIter);
    MR::calcActorAxisY(&mUp, this);
    initRailRider(rIter);
    MR::moveCoordToNearestPos(this, mPosition);

    int fishType = 0;
    MR::getJMapInfoArg0NoInit(rIter, &mFishNum);

    bool canEscape = false;
    MR::getJMapInfoArg1NoInit(rIter, &canEscape);

    bool calcGravity = false;
    MR::getJMapInfoArg2NoInit(rIter, &calcGravity);
    if (calcGravity) {
        MR::onCalcGravity(this);
    }
    const char* objName;
    MR::getObjectName(&objName, rIter);
    if (strcmp(objName, "FishGroupA") == 0) {
        fishType = 0;
    } else if (strcmp(objName, "FishGroupB") == 0) {
        fishType = 1;
    } else if (strcmp(objName, "FishGroupC") == 0) {
        fishType = 2;
    } else if (strcmp(objName, "FishGroupD") == 0) {
        fishType = 3;
    } else if (strcmp(objName, "FishGroupE") == 0) {
        fishType = 4;
    } else if (strcmp(objName, "FishGroupF") == 0) {
        fishType = 5;
    }

    mFish = new Fish*[mFishNum];
    for (int i = 0; i < mFishNum; i++) {
        mFish[i] = new Fish("魚", this, fishType, canEscape);
        mFish[i]->initWithoutIter();
    }

    MR::initAndSetRailClipping(&mClippingCenter, this, 100.0f, 500.0f);
    makeActorAppeared();
}

void FishGroup::startClipped() {
    LiveActor::startClipped();
    for (int i = 0; i < mFishNum; i++) {
        mFish[i]->startClipped();
    }
}

void FishGroup::endClipped() {
    LiveActor::endClipped();
    for (int i = 0; i < mFishNum; i++) {
        mFish[i]->endClipped();
    }
}

void FishGroup::control() {
    mUp.set(-mGravity);
    MR::moveCoordAndFollowTrans(this, mSpeed);
}
