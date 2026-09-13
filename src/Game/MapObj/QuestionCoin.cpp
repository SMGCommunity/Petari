#include "Game/MapObj/QuestionCoin.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/CoinRotater.hpp"
#include "Game/MapObj/SpinDriverUtil.hpp"
#include "Game/Util.hpp"

namespace {
    // cPoseTypeString
    // cShadowDropLengthType
    const f32 sHitRadius = 150.0f;
    const f32 sOffsetY = 210.0f;
    const f32 sInWaterScale = 2.0f;
    const f32 sShadowRadius = 120.0f;
    const f32 sShadowHeightMargin = 300.0f;
    const s32 sBindAnimFrame = 3;
    // sBindEndBlendFrame
    const s32 sBindStartBlendFrame = 0;
}  // namespace

namespace NrvQuestionCoin {
    NEW_NERVE(HostTypeWait, QuestionCoin, Wait);
    NEW_NERVE(HostTypeThrowActor, QuestionCoin, ThrowActor);
    NEW_NERVE(HostTypeCaught, QuestionCoin, Caught);
}  // namespace NrvQuestionCoin

QuestionCoin::QuestionCoin(const char* pName)
    : LiveActor(pName), mBoundActor(), mCameraInfo(), mLaunchSpeed(), mIsWeakGravity(), mCalcShadow(), mPoseType(), mShadowDropLengthType(),
      mShadowPos(gZeroVec), mShadowDropLength() {
    mBaseMtx.identity();
    mActorMtx.identity();
    mLaunchQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
    MR::createCoinRotater();
}

void QuestionCoin::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initFromJMapInfoArg(rIter);

    if (mPoseType == 0) {
        TPos3f rotation;
        MR::makeMtxTR(rotation.toMtxPtr(), mPosition, mRotation);
        TVec3f front;
        rotation.getZDir(front);
        MR::normalize(&front);
        MR::makeMtxUpFrontPos(&mBaseMtx, -mGravity, front, mPosition);
    } else {
        MR::makeMtxTR(mBaseMtx.toMtxPtr(), this);
    }

    initModelManagerWithAnm("QuestionCoin", nullptr, false);
    MR::startBpk(this, "QuestionCoin");
    MR::connectToSceneItemStrongLight(this);
    initEffectKeeper(0, nullptr, false);
    initHitSensor(1);
    f32 radius = sHitRadius * mScale.x;
    HitSensor* sensor = MR::addHitSensor(this, "binder", ATYPE_QUESTION_COIN_BIND, 16, radius, TVec3f(0.0f, sOffsetY * mScale.y, 0.0f));
    MR::setClippingTypeSphere(this, 2.0f * radius, &sensor->mPosition);
    initShadow();
    MR::initActorCamera(this, rIter, &mCameraInfo);
    initNerve(GET_NERVE(QuestionCoin, HostTypeWait));
    MR::needStageSwitchWriteDead(this, rIter);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::listenStageSwitchOnAppear(this, MR::FunctorV0M< QuestionCoin*, void (QuestionCoin::*)() >(this, &QuestionCoin::appear));
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void QuestionCoin::initAfterPlacement() {
    if (MR::isInWater(mPosition)) {
        getSensor("binder")->mRadius = sInWaterScale * getSensor("binder")->mRadius;
    }

    TPos3f rotation;
    rotation.setRotateDegree(mRotation);
    TVec3f up;
    rotation.getYDir(up);
    TVec3f side;
    rotation.getXDir(side);
    TPos3f launchMtx;
    launchMtx.identity();
    MR::makeMtxUpSide(&launchMtx, up, side);
    launchMtx.getQuat(mLaunchQuat);

    f32 dropLength = 1000.0f;

    if (mShadowDropLengthType == 1) {
        dropLength = 1500.0f;
    } else if (mShadowDropLengthType == 2) {
        dropLength = 2000.0f;
    }

    TVec3f gravity(gZeroVec);
    MR::calcGravityAndDropShadowVectorOrZero(this, &gravity, nullptr, 0);

    if (MR::isNearZero(gravity)) {
        gravity.set(-up);
    }

    mGravity.set(gravity);
    SpinDriverUtil::setShadowAndClipping(this, &mShadowPos, dropLength, sShadowHeightMargin, &mShadowDropLength);
    MR::setShadowDropPositionPtr(this, nullptr, &getSensor("binder")->mPosition);
}

void QuestionCoin::appear() {
    LiveActor::appear();
    MR::showModel(this);
    MR::emitEffect(this, "Appear");

    if (MR::isValidSwitchDead(this)) {
        MR::offSwitchDead(this);
    }

    setNerve(GET_NERVE(QuestionCoin, HostTypeWait));
}

void QuestionCoin::kill() {
    mBoundActor = nullptr;

    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    LiveActor::kill();
}

void QuestionCoin::calcAndSetBaseMtx() {
    TPos3f mtx;
    mtx.identity();

    if (isNerve(GET_NERVE(QuestionCoin, HostTypeWait))) {
        mtx.concat(mBaseMtx, MR::getCoinInWaterRotateYMatrix());
    } else {
        mtx.set(mBaseMtx);
    }

    MR::setBaseTRMtx(this, mtx);
}

bool QuestionCoin::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return MR::isMsgStarPieceReflect(msg);
}

bool QuestionCoin::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg) && isThrowActor() && isNerve(GET_NERVE(QuestionCoin, HostTypeWait))) {
        mBoundActor = pSender->mHost;
        mActorMtx.set(mBoundActor->getBaseMtx());
        setNerve(GET_NERVE(QuestionCoin, HostTypeThrowActor));
        return true;
    }

    if (MR::isMsgItemGet(msg) && !isThrowActor() && isNerve(GET_NERVE(QuestionCoin, HostTypeWait))) {
        setNerve(GET_NERVE(QuestionCoin, HostTypeCaught));
        return true;
    }

    if (MR::isMsgUpdateBaseMtx(msg)) {
        if (isNerve(GET_NERVE(QuestionCoin, HostTypeThrowActor))) {
            updateActorMtx();
        }

        return true;
    }

    return false;
}

void QuestionCoin::startClipped() {
    MR::forceDeleteEffect(this, "Light");
    LiveActor::startClipped();
}

void QuestionCoin::endClipped() {
    LiveActor::endClipped();
    MR::emitEffect(this, "Light");
}

void QuestionCoin::initFromJMapInfoArg(const JMapInfoIter& rIter) {
    MR::getJMapInfoArg0NoInit(rIter, &mLaunchSpeed);
    s32 gravityType = -1;
    MR::getJMapInfoArg1NoInit(rIter, &gravityType);
    mIsWeakGravity = gravityType == 0;
    MR::getJMapInfoArg2NoInit(rIter, &mPoseType);
    MR::getJMapInfoArg3NoInit(rIter, &mShadowDropLengthType);
    s32 calcShadow = -1;
    MR::getJMapInfoArg4NoInit(rIter, &calcShadow);
    mCalcShadow = calcShadow != -1;
}

void QuestionCoin::initShadow() {
    MR::initShadowVolumeSphere(this, sShadowRadius * mScale.x);

    if (mCalcShadow) {
        MR::onCalcShadow(this, nullptr);
    }
}

void QuestionCoin::updateActorMtx() {
    f32 rate = 1.0f;

    if (MR::isLessEqualStep(this, sBindStartBlendFrame) && sBindStartBlendFrame > 0) {
        rate = MR::calcNerveRate(this, sBindStartBlendFrame);
    }

    TQuat4f startQuat;
    mActorMtx.getQuat(startQuat);
    TQuat4f quat(static_cast< const Quaternion& >(startQuat));
    quat.slerp(mLaunchQuat, rate);
    TVec3f startPos;
    mActorMtx.getTrans(startPos);
    TVec3f position;
    JMAVECLerp(&startPos, &getSensor("binder")->mPosition, &position, rate);
    TPos3f mtx;
    mtx.setQT(quat, position);
    MR::setBaseTRMtx(mBoundActor, mtx);
}

void QuestionCoin::exeThrowActor() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Get");
        MR::startSystemSE("SE_SY_QUESTION_COIN", -1, -1);
        MR::tryRumblePadMiddle(this, 0);
        MR::hideModel(this);
        MR::startBck(mBoundActor, "QuestionCoinJumpStart", nullptr);
        MR::startSound(mBoundActor, "SE_PV_JUMP_JOY");

        if (MR::isExistActorCamera(mCameraInfo)) {
            MR::startActorCameraNoTarget(this, mCameraInfo, -1);
        }
    }

    if (MR::isStep(this, sBindAnimFrame)) {
        TPos3f rotation;
        MR::makeMtxRotate(rotation.toMtxPtr(), mRotation);
        TVec3f velocity;
        rotation.getYDir(velocity);
        velocity.setLength(mLaunchSpeed);
        mBoundActor->mVelocity.set(velocity);
        MR::startBck(mBoundActor, "QuestionCoinJump", nullptr);

        if (mIsWeakGravity) {
            MR::endBindAndPlayerForceWeakGravityJump(this, velocity);
        } else {
            MR::endBindAndPlayerJump(this, velocity, 3);
        }

        MR::endActorCameraAtLanding(this, mCameraInfo, -1);
        kill();
    }
}

void QuestionCoin::exeCaught() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Get");
        MR::startSystemSE("SE_SY_QUESTION_COIN", -1, -1);
        MR::tryRumblePadMiddle(this, 0);
        MR::hideModel(this);
        kill();
    }
}

QuestionCoin::~QuestionCoin() {
}
