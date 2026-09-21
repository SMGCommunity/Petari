#include "Game/MapObj/SpringWaterFloaterSpot.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DJoint.hpp>

void SpringWaterFloaterSpot_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.0f;
    (void)-1.0f;
}

namespace {
    // static const sRisingSpeed;
    // static const sRisingSpeedAtFloatSignal;
    // static const sBoundAmpMax;
    // static const sBoundTime;
    // static const sStartFloatPower;
    // static const sRambleSide;
    // static const sRambleRotX;
    // static const sDefaultAccelRate;
    // static const sAccelMax;
    // static const sStepForFloatSignal;
    // static const sStepToRisingForFloatSignal;
    // static const sEffectRippleOffset;
    // static const sWaveSeStepsMin;
    // static const sWaveSeStepsMax;
};  // namespace

namespace NrvSpringWaterFloaterSpot {
    NEW_NERVE(SpringWaterFloaterSpotNrvWait, SpringWaterFloaterSpot, Wait);
    NEW_NERVE(SpringWaterFloaterSpotNrvFloatSignal, SpringWaterFloaterSpot, FloatSignal);
    NEW_NERVE(SpringWaterFloaterSpotNrvFloatUp, SpringWaterFloaterSpot, FloatUp);
    NEW_NERVE(SpringWaterFloaterSpotNrvFloatEnd, SpringWaterFloaterSpot, FloatEnd);
};  // namespace NrvSpringWaterFloaterSpot

SpringWaterFloaterSpot::SpringWaterFloaterSpot(const char* pName)
    : LiveActor(pName), _8C(), _90(20.0f), mHeight(), _A4(), mBoundStep(), _AC(0.0f, 0.0f, 0.0f), mBoundSize(), _C8(), mNeedsBound(), _CA(),
      mFloatTimer() {
}

void SpringWaterFloaterSpot::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    mInitialRotation = mRotation;

    const char* objectName;
    MR::getObjectName(&objectName, rIter);
    initModelManagerWithAnm(objectName, nullptr, false);

    _CC.set(getBaseMtx());

    MR::connectToSceneCollisionMapObj(this);

    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, objectName, getSensor("body"), nullptr);

    initEffectKeeper(0, nullptr, false);
    initSound(4, 0);

    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);

    MR::onCalcGravity(this);

    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOnB(this, MR::Functor(this, &SpringWaterFloaterSpot::startFloat));
    }

    MR::tryRegisterDemoCast(this, rIter);

    MR::getJMapInfoArg0NoInit(rIter, &_90);
    _90 /= 100.0f;

    MR::getJMapInfoArg1NoInit(rIter, &_A4);

    _AC.set(mPosition);

    initNerve(GET_NERVE(SpringWaterFloaterSpot, SpringWaterFloaterSpotNrvWait));

    makeActorAppeared();
}

void SpringWaterFloaterSpot::control() {
    _C8 = MR::isOnPlayer(getSensor(nullptr));

    if (isNerve(GET_NERVE(SpringWaterFloaterSpot, SpringWaterFloaterSpotNrvFloatUp))) {
        floatFloater();
    }

    boundFloater();

    _AC.add(mVelocity);
    MR::makeMtxUpNoSupportPos(&_CC, -mGravity, _AC);

    TVec3f vec20;
    TVec3f vec14(0.0f, -70.0f + _A4, 0.0f);
    PSMTXMultVec(_CC, vec14, vec20);
    _CC.setTrans(vec20);
}

void SpringWaterFloaterSpot::calcAndSetBaseMtx() {
    mPosition.set(_AC);

    TPos3f baseMtx;
    MR::makeMtxTR(baseMtx, this);

    if (mNeedsBound) {
        TVec3f offset;
        offset.scale(mBoundSize, -mGravity);
        baseMtx[0][3] += offset.x;
        baseMtx[1][3] += offset.y;
        baseMtx[2][3] += offset.z;
    }

    MR::setBaseTRMtx(this, baseMtx);
}

void SpringWaterFloaterSpot::exeWait() {
    TVec3f vec1C;
    vec1C.scale(_A4, -mGravity);

    if (MR::isInWater(this, vec1C)) {
        if (mHeight >= 90.0f) {
            setNerve(GET_NERVE(SpringWaterFloaterSpot, SpringWaterFloaterSpotNrvFloatSignal));
        } else {
            mHeight += 1.0f;
        }
    }
}

void SpringWaterFloaterSpot::exeFloatSignal() {
    f32 scale = 3.0f * (getNerveStep() % 8 < 4 ? 1.0f : -1.0f);

    TVec3f sideVec;
    MR::calcSideVec(&sideVec, this);

    TVec3f scaledSideVec;
    scaledSideVec.scale(scale, sideVec);
    mVelocity.set(scaledSideVec);

    if (isModelLongSide()) {
        mRotation.x += getNerveStep() % 12 < 6 ? 1.0f : -1.0f;
    }

    if (MR::isGreaterStep(this, 60)) {
        mVelocity.scale(0.5f, -mGravity);
    }

    MR::startLevelSound(this, "SE_OJ_LV_SPR_FLOATER_SIGN");

    if (MR::isStep(this, 120)) {
        setNerve(GET_NERVE(SpringWaterFloaterSpot, SpringWaterFloaterSpotNrvFloatUp));
    }
}

void SpringWaterFloaterSpot::exeFloatUp() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_SPR_FLOATER_FLOAT_ST");
    }

    MR::startLevelSound(this, "SE_OJ_LV_SPR_FLOATER_FLOAT");

    if (isNeedsForBound()) {
        mBoundStep = 0;
        mNeedsBound = true;
    }

    if (MR::isNearZero(mVelocity) && !mNeedsBound) {
        setNerve(GET_NERVE(SpringWaterFloaterSpot, SpringWaterFloaterSpotNrvFloatEnd));
    }
}

void SpringWaterFloaterSpot::exeFloatEnd() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();

        MR::validateClipping(this);

        MR::startSound(this, "SE_OJ_SPR_FLOATER_FLOAT_ED");

        mFloatTimer = MR::getRandom(60l, 120l);
    }

    mFloatTimer--;

    if (mFloatTimer <= 0) {
        MR::startSound(this, "SE_OJ_SPR_FLOATER_RIPPLE");
        mFloatTimer = MR::getRandom(60l, 120l);
    }

    if (isNeedsForBound()) {
        mBoundStep = 0;
        mNeedsBound = true;
    }
}

void SpringWaterFloaterSpot::startFloat() {
    if (isNerve(GET_NERVE(SpringWaterFloaterSpot, SpringWaterFloaterSpotNrvWait))) {
        MR::invalidateClipping(this);
    }
}

bool SpringWaterFloaterSpot::boundFloater() {
    if (!mNeedsBound) {
        return false;
    }

    if (mBoundStep == 0) {
        MR::startSound(this, "SE_OJ_SPR_FLOATER_BOUND");
    }

    if (mBoundStep == 150) {
        mNeedsBound = false;
        mBoundSize = 0.0f;
        mBoundStep = 0;
        return true;
    }

    f32 scale = 20.0f - mBoundStep * (2.0f / 15.0f);
    mBoundSize = scale * MR::sinDegree(9.6f * static_cast< f32 >(mBoundStep));
    mBoundStep++;

    return false;
}

void SpringWaterFloaterSpot::floatFloater() {
    TVec3f up;
    up.negate(mGravity);

    TVec3f scaledUp;
    scaledUp.scale(_A4, up);

    if (MR::isInWater(this, scaledUp)) {
        if (mHeight >= 90.0f) {
            mVelocity.scale(1.0f + _8C, up);

            mHeight = 90.0f;

            if (_8C < 40.0f) {
                _8C += _90;
            }

            return;
        }

        mHeight += 1.0f;

        return;
    }

    if (!_CA) {
        MR::emitEffect(this, "Ripple");
        MR::setEffectHostMtx(this, "Ripple", _CC);

        mBoundStep = 0;
        mNeedsBound = true;
    }

    _CA = true;
    _8C = 0.0f;

    mRotation.set(mInitialRotation);
    mVelocity.zero();
}

bool SpringWaterFloaterSpot::isModelLongSide() {
    J3DJoint* joint = MR::getJoint(this, static_cast< u16 >(0));
    return (joint->mMax.x + joint->mMax.z) / 2.0f > joint->mMax.y;
}

bool SpringWaterFloaterSpot::isNeedsForBound() {
    if (_C8 && MR::isPlayerJumpRising() && MR::checkPlayerActionTrigger()) {
        return true;
    }

    if (!_C8 && MR::isOnPlayer(getSensor(nullptr))) {
        return true;
    }

    return false;
}
