#include "Game/MapObj/OceanSmallTurtle.hpp"
#include "Game/AreaObj/MercatorTransformCube.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/Util.hpp"

namespace NrvOceanSmallTurtle {
    NEW_NERVE(OceanSmallTurtleNrvWait, OceanSmallTurtle, Wait);
    NEW_NERVE(OceanSmallTurtleNrvFloatSignal, OceanSmallTurtle, FloatSignal);
    NEW_NERVE(OceanSmallTurtleNrvFloatUp, OceanSmallTurtle, FloatUp);
    NEW_NERVE(OceanSmallTurtleNrvCatchRail, OceanSmallTurtle, CatchRail);
    NEW_NERVE(OceanSmallTurtleNrvRailMove, OceanSmallTurtle, RailMove);
};  // namespace NrvOceanSmallTurtle

namespace {
    // static const f32 sRisingSpeed;
    static const f32 sRisingSpeedAtFloatSignal = 0.5f;
    static const f32 sCatchRailSpeed = 2.0f;
    static const f32 sJudgeForCatchRail = 100.0f;
    static const f32 sBoundAmpMax = 20.0f;
    static const s32 sBoundTime = 150;
    static const f32 sStartFloatPower = 1.0f;
    static const f32 sRambleSide = 1.0f;
    static const f32 sRambleRotX = 1.0f;
    static const f32 sDefaultAccelRate = 20.0f;
    static const f32 sAccelMax = 40.0f;
    static const s32 sStepForFloatSignal = 60;
    static const s32 sStepToRisingForFloatSignal = 120;
};  // namespace

void OceanSmallTurtle_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)MR::epsilon();
    (void)0.5f;
    (void)3.0f;
    (void)-1.0f;
    (void)2.0f;
}

OceanSmallTurtle::OceanSmallTurtle(const char* pName)
    : LiveActor(pName), mRailMover(), mFloatUpSpeed(), mAccelRate(::sDefaultAccelRate), mStartNearestRailPos(0.0f), mFloatDelayTimer(), mArg1(),
      mBoundTimer(), mMercatorPos(0.0f), mBoundDisplacement(), mRailClippingPos(0.0f), mRailClippingRadius(), mStartRotationX(), mIsOnPlayer(),
      isBound(), isStartFloat() {
    mEffectMtx.identity();
}

void OceanSmallTurtle::init(const JMapInfoIter& rIter) {
    MR::getJMapInfoArg0NoInit(rIter, &mAccelRate);
    mAccelRate /= 100.0f;

    MR::getJMapInfoArg1NoInit(rIter, &mArg1);
    MR::getJMapInfoRotate(rIter, &mInfoRotate);

    MR::initDefaultPosForMercator(this, rIter, true);
    mMercatorPos.set(mPosition);

    initModelManagerWithAnm("OceanSmallTurtle", nullptr, false);
    mEffectMtx.set(getBaseMtx());

    MR::connectToSceneCollisionMapObj(this);

    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);

    MR::initCollisionParts(this, "OceanSmallTurtle", getSensor("body"), nullptr);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initRailRider(rIter);

    MR::calcNearestRailPosForMercator(&mStartNearestRailPos, this, ::sJudgeForCatchRail);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    MR::calcRailClippingInfoForMercator(&mRailClippingPos, &mRailClippingRadius, this, 100.0f, 500.0f);
    mRailMover = MR::createMapPartsRailMoverForMercator(this, rIter, false);

    MR::onCalcGravity(this);

    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOnB(this, MR::Functor(this, &OceanSmallTurtle::startFloat));
    }

    MR::tryRegisterDemoCast(this, rIter);
    initNerve(GET_NERVE(OceanSmallTurtle, OceanSmallTurtleNrvWait));
    makeActorAppeared();
}

void OceanSmallTurtle::control() {
    mIsOnPlayer = MR::isOnPlayer(getSensor(nullptr));

    if (isNerve(GET_NERVE(OceanSmallTurtle, OceanSmallTurtleNrvFloatUp))) {
        floatFloater();
    }

    boundFloater();

    mMercatorPos.add(mVelocity);

    MR::makeMtxUpNoSupportPos(&mEffectMtx, -mGravity, mMercatorPos);
}

void OceanSmallTurtle::calcAndSetBaseMtx() {
    TPos3f mtx;
    mtx.identity();
    mPosition.set(mMercatorPos);

    TPos3f rotateMtx;
    MR::makeMtxRotate(rotateMtx, mRotation);
    mtx.concat(rotateMtx);

    if (isNerve(GET_NERVE(OceanSmallTurtle, OceanSmallTurtleNrvRailMove))) {
        TPos3f rotateMtx2;
        MR::makeMtxRotate(rotateMtx2, mInfoRotate);
        mtx.concat(rotateMtx2);
    }

    mtx.setTrans(mPosition);

    if (isBound) {
        TVec3f up(-mGravity);
        TVec3f scaledUp;
        scaledUp.scale(mBoundDisplacement, up);
        mtx[0][3] += scaledUp.x;
        mtx[1][3] += scaledUp.y;
        mtx[2][3] += scaledUp.z;
    }

    MR::setBaseTRMtx(this, mtx);
}

void OceanSmallTurtle::exeWait() {
    if (MR::isFirstStep(this)) {
        mStartRotationX = mRotation.x;
    }

    TVec3f up;
    up.negate(mGravity);
    TVec3f scaledUp;
    scaledUp.scale(mArg1, up);

    if (MR::isInWater(this, scaledUp)) {
        if (mFloatDelayTimer >= 150.0f) {
            setNerve(GET_NERVE(OceanSmallTurtle, OceanSmallTurtleNrvFloatSignal));
        } else {
            mFloatDelayTimer += 1.0f;
        }
    }
}

void OceanSmallTurtle::exeFloatSignal() {
    f32 speedAndDir = 3.0f * (getNerveStep() % 8 < 4 ? ::sRambleSide : -::sRambleSide);
    TVec3f sideVec;
    MR::calcSideVec(&sideVec, this);

    TVec3f velocity;

    velocity.scale(speedAndDir, sideVec);
    mVelocity.set(velocity);

    MR::startLevelSound(this, "SE_OJ_LV_O_SML_TURTLE_SIGN");

    mRotation.x += getNerveStep() % 12 < 6 ? ::sRambleRotX : -::sRambleRotX;

    if (MR::isGreaterStep(this, ::sStepForFloatSignal)) {
        mVelocity.scale(::sRisingSpeedAtFloatSignal, -mGravity);
    }

    if (MR::isStep(this, ::sStepToRisingForFloatSignal)) {
        setNerve(GET_NERVE(OceanSmallTurtle, OceanSmallTurtleNrvFloatUp));
    }
}

void OceanSmallTurtle::exeFloatUp() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_O_SML_TURTLE_FLOAT_ST");
    }

    MR::startLevelSound(this, "SE_OJ_LV_O_SML_TURTLE_FLOAT");

    if (isEndSpringWater()) {
        MR::startSound(this, "SE_OJ_O_SML_TURTLE_FLOAT_ED");
        setNerve(GET_NERVE(OceanSmallTurtle, OceanSmallTurtleNrvCatchRail));
        return;
    }

    if (isNeedsForBound()) {
        mBoundTimer = 0;
        isBound = true;
    }
}

void OceanSmallTurtle::exeCatchRail() {
    if (!catchNearestRailPos()) {
        return;
    }

    mVelocity.zero();
    mRailMover->start();

    MR::convertMercatorPlaneToSphereTransAndRotate(&mMercatorPos, &mRotation, mRailMover->_28, true);

    setNerve(GET_NERVE(OceanSmallTurtle, OceanSmallTurtleNrvRailMove));
}

void OceanSmallTurtle::exeRailMove() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        MR::initAndSetRailClippingForMercator(&mRailClippingPos, this, 100.0f, 500.0f);
        MR::setClippingTypeSphere(this, mRailClippingRadius, &mRailClippingPos);
    }

    mRailMover->movement();
    MR::startLevelSound(this, "SE_OJ_LV_O_SML_TURTLE_MOVE");

    MR::convertMercatorPlaneToSphereTransAndRotate(&mMercatorPos, &mRotation, mRailMover->_28, true);

    if (isNeedsForBound()) {
        mBoundTimer = 0;
        isBound = true;
    }
}

void OceanSmallTurtle::startFloat() {
    if (isNerve(GET_NERVE(OceanSmallTurtle, OceanSmallTurtleNrvWait))) {
        MR::invalidateClipping(this);
    }
}

bool OceanSmallTurtle::boundFloater() {
    if (!isBound) {
        return false;
    }

    if (mBoundTimer == 0) {
        MR::startSound(this, "SE_OJ_O_SML_TURTLE_RIPPLE");
    }

    if (mBoundTimer == ::sBoundTime) {
        isBound = false;
        mBoundDisplacement = 0.0f;
        mBoundTimer = 0;
        return true;
    }

    f32 boundAmp = (::sBoundAmpMax - (mBoundTimer * (1.0f / 7.5f)));

    mBoundDisplacement = boundAmp * MR::sinDegree(mBoundTimer * 9.6f);

    mBoundTimer++;
    return false;
}

bool OceanSmallTurtle::catchNearestRailPos() {
    if (mStartNearestRailPos.squared(mMercatorPos) < ::sJudgeForCatchRail) {
        return true;
    }

    TVec3f vec;
    vec.sub(mStartNearestRailPos, mMercatorPos);

    MR::normalize(&vec);

    mVelocity.scale(::sCatchRailSpeed, vec);
    return false;
}

void OceanSmallTurtle::floatFloater() {
    TVec3f up;
    up.negate(mGravity);
    TVec3f scaledUp;
    scaledUp.scale(mArg1, up);

    if (MR::isInWater(this, scaledUp)) {
        if (mFloatDelayTimer >= 150.0f) {
            mVelocity.scale(::sStartFloatPower + mFloatUpSpeed, up);
            mFloatDelayTimer = 150.0f;
            if (mFloatUpSpeed < ::sAccelMax) {
                mFloatUpSpeed += mAccelRate;
            }
        } else {
            mFloatDelayTimer += 1.0f;
        }
    } else {
        if (!isStartFloat) {
            MR::emitEffect(this, "Ripple");
            MR::setEffectHostMtx(this, "Ripple", mEffectMtx);
            mBoundTimer = 0;
            isBound = true;
        }

        isStartFloat = true;
        mFloatUpSpeed = 0.0f;
        mRotation.x = mStartRotationX;
        mVelocity.zero();
    }
}

bool OceanSmallTurtle::isNeedsForBound() {
    if (mIsOnPlayer && MR::isPlayerJumpRising() && MR::checkPlayerActionTrigger()) {
        return true;
    }

    if (!mIsOnPlayer && MR::isOnPlayer(getSensor(nullptr))) {
        return true;
    }

    return false;
}

bool OceanSmallTurtle::isEndSpringWater() {
    return mVelocity.isZero() && isStartFloat && !isBound;
}
