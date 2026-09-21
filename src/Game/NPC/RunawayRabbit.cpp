#include "Game/NPC/RunawayRabbit.hpp"
#include "Game/Enemy/WalkerStateBlowDamage.hpp"
#include "Game/Enemy/WalkerStateRunaway.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/SpotMarkLight.hpp"
#include "Game/NPC/Rabbit.hpp"
#include "Game/NPC/RunawayRabbitCollect.hpp"
#include "Game/NPC/TrickRabbitUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/TalkUtil.hpp"

void RunawayRabbit_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.0f;
    (void)-1.0f;
    (void)2.0f;
}

namespace {
    static const f32 sLightLength = 1500.0f;
    static const f32 sBodyRadius = 70.0f;
    static const f32 sCatchRadius = 30.0f;
    static const f32 sBindRadius = 60.0f;
    static const f32 sBindYOffset = 60.0f;
    static const f32 sShadowRadius = 45.0f;
    static const f32 sUpVecBlendRate = 0.1f;
    static const f32 sFrontVecBlendRate = 0.2f;
    static const f32 sAirFric = 0.99f;
    static const f32 sAirGravityAccel = 1.0f;
    static const f32 sPushAccel = 0.5f;
    static const f32 sWaitTurnLimit = 20.0f;
    static const f32 sHoleAppearPowerH = 19.0f;
    static const f32 sHoleAppearPowerV = 25.0f;
    static const f32 sGroundAppearPowerH = 16.0f;
    static const f32 sGroundAppearPowerV = 25.0f;
    static const f32 sRunawayAccelMax = 1.8f;
    static const f32 sRunawayAccelMin = 0.5f;
    // bool sIsTireRunawayAccel;
    static const f32 sNoPressureAccel = 0.6f;
    static const f32 sPressureRange = 400.0f;
    static const s32 sPressureLimitTime = 2000;
    static const s32 sPressureDeccelStart = 200;
    static const s32 sPressureDeccelTimeLv1 = 240;
    static const s32 sPressureDeccelTimeLv2 = 500;
    static const s32 sPressureDeccelTimeLv3 = 800;
    // f32 sRunawayTurnStartLimit;
    // f32 sRunawayTurnLimit;
    // s32 sRunawayTurnDeccelTime;
    static const f32 sAwayTargetVelocityRate = 0.3f;
    // f32 sRunAccelbyBckRate;
    static const f32 sMinRunBckRate = 0.9f;
    static const f32 sMaxRunBckRate = 1.4f;
    static const s32 sMarioPoseBlendTime = 5;
    // const f32 sCaughtJumpPower;
    // f32 sCaughtJumpFrontPower;
    static const f32 sDamageTurnLimit = 3.0f;
    static const f32 sBlowAccelH = 25.0f;
    static const f32 sBlowAccelV = 30.0f;
    // s32 sDamageLandTime;
    static const f32 sStarPieceHitAccelH = 20.0f;
    static const f32 sStarPieceHitAccelV = 20.0f;
    static const f32 sNearSoundRadiusDefault = 600.0f;
    static const f32 sNearSoundRadiusEdge = 250.0f;
    static const s32 sNearSoundFxSend = 60;
    static const f32 sNearSoundMinVolume = 35.0f;
    static const f32 sNearSoundMaxVolume = 100.0f;
    static const s32 sTossStep = 10;

    class RunawayRabbitParam : public WalkerStateRunawayParam {
    public:
        RunawayRabbitParam() {
        }
    };
    static RunawayRabbitParam sParam = RunawayRabbitParam();
};  // namespace

namespace NrvRunawayRabbit {
    NEW_NERVE(RunawayRabbitNrvNoActive, RunawayRabbit, NoActive);
    NEW_NERVE(RunawayRabbitNrvHide, RunawayRabbit, Hide);
    NEW_NERVE(RunawayRabbitNrvAppear, RunawayRabbit, Appear);
    NEW_NERVE(RunawayRabbitNrvRunaway, RunawayRabbit, Runaway);
    NEW_NERVE(RunawayRabbitNrvTryCaughtDemo, RunawayRabbit, TryCaughtDemo);
    NEW_NERVE(RunawayRabbitNrvCaught, RunawayRabbit, Caught);
    NEW_NERVE(RunawayRabbitNrvCaughtTalk, RunawayRabbit, CaughtTalk);
    NEW_NERVE(RunawayRabbitNrvCaughtEnd, RunawayRabbit, CaughtEnd);
    NEW_NERVE(RunawayRabbitNrvStop, RunawayRabbit, Stop);
    NEW_NERVE(RunawayRabbitNrvBlowDamage, RunawayRabbit, BlowDamage);
};  // namespace NrvRunawayRabbit

RunawayRabbit::RunawayRabbit(const char* pName, RunawayRabbitCollect* pCollect)
    : LiveActor(pName), mStateRunaway(), mStateBlowDamage(), mCollect(pCollect), mFootPrint(), mSpotMarkLight(), mMsgCtrl(), mPoseQuat(0, 0, 0, 1),
      mFrontVec(0, 0, 1), mPlayerPoseQuat(0, 0, 0, 1), mPlayerBindPos(0, 0, 1), mGroupId(-1), mRunawayLevel(), mHideLocation(HideLocation_None),
      mPressureTimer(), mNotCaughtableTimer(), mIsActive(true), mIsLinked(), mNearSoundRadius(-1.0f) {
}

void RunawayRabbit::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    s32 rabbitType = -1;
    MR::getJMapInfoArg2WithInit(rIter, &rabbitType);
    initModelManagerWithAnm(rabbitType == 0 ? "TrickRabbitBaby" : "TrickRabbit", nullptr, false);
    MR::connectToSceneNpc(this);
    MR::initLightCtrl(this);
    MR::makeQuatAndFrontFromRotate(&mPoseQuat, &mFrontVec, this);
    MR::onCalcGravity(this);

    MR::addBaseMatrixFollowTarget(this, rIter, nullptr, MR::createBaseMatrixFollowValidateDelegator(this, &RunawayRabbit::isValidFollow));
    mStateRunaway = new WalkerStateRunaway(this, &mFrontVec, &::sParam);
    mStateBlowDamage = new WalkerStateBlowDamage(this, &mFrontVec, nullptr);
    MR::getJMapInfoArg0WithInit(rIter, &mGroupId);
    MR::getJMapInfoArg1WithInit(rIter, &mHideLocation);
    MR::getJMapInfoArg3WithInit(rIter, &mNearSoundRadius);

    if (mNearSoundRadius <= 0.0f) {
        mNearSoundRadius = ::sNearSoundRadiusDefault;
    }

    s32 cameraRegisterVec = -1;
    MR::getJMapInfoArg7WithInit(rIter, &cameraRegisterVec);

    if (cameraRegisterVec != -1) {
        MR::declareCameraRegisterVec(this, cameraRegisterVec, &mPosition);
    }

    mSpotMarkLight = new SpotMarkLight(this, 100.0f, ::sLightLength, nullptr);
    mSpotMarkLight->initWithoutIter();
    initSensor();
    initBinder(::sBindRadius, ::sBindYOffset, 0);
    MR::onCalcGravity(this);
    initShadow();
    initFootPrint();
    initSound(6, false);
    initEffectKeeper(0, nullptr, false);
    MR::hideModel(this);
    MR::invalidateHitSensors(this);
    initNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvNoActive));

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::listenStageSwitchOnAppear(this, MR::Functor(this, &RunawayRabbit::startRunnaway));
    }

    makeActorAppeared();
}

void RunawayRabbit::initShadow() {
    MR::initShadowVolumeSphere(this, ::sShadowRadius);
    TVec3f zeroVec(0.0f, 0.0f, 0.0f);
    MR::setShadowDropPositionAtJoint(this, nullptr, "Spine", zeroVec);
}

void RunawayRabbit::initFootPrint() {
    mFootPrint = TrickRabbitUtil::createRabbitFootPrint(this);
}

void RunawayRabbit::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
}

void RunawayRabbit::initSensor() {
    initHitSensor(2);
    MR::addHitSensorAtJointEnemy(this, "Body", "Spine", 8, ::sBodyRadius, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointEnemy(this, "Catch", "Spine", 8, ::sCatchRadius, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initStarPointerTargetAtJoint(this, "Spine", ::sBodyRadius, TVec3f(0, 0, 0));
}

void RunawayRabbit::appear() {
    LiveActor::appear();
    MR::emitEffect(this, "AppearSmoke");
}

void RunawayRabbit::control() {
    updatePose();
    if (mNotCaughtableTimer > 0) {
        mNotCaughtableTimer--;
    }

    if (MR::isBindedGroundWater(this)) {
        MR::setSeVersion(this, Rabbit::SeVersion_WATER);
    } else {
        MR::setSeVersion(this, Rabbit::SeVersion_NORMAL);
    }
}

void RunawayRabbit::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mPoseQuat);
}

void RunawayRabbit::updatePose() {
    MR::blendQuatUpFront(&mPoseQuat, -mGravity, mFrontVec, ::sUpVecBlendRate, ::sFrontVecBlendRate);
}

void RunawayRabbit::updateBindActorMatrix() {
    TPos3f mtx;
    mtx.setQT(mPlayerPoseQuat, mPlayerBindPos);
    MR::setPlayerBaseMtx(mtx);
}

void RunawayRabbit::activate() {
    if (isNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvNoActive))) {
        setNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvHide));
    }
}

void RunawayRabbit::startRunnaway() {
    if (mIsActive) {
        MR::showModel(this);
        MR::invalidateClipping(this);
        MR::validateHitSensors(this);
        setNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvAppear));
        mCollect->noticeAppearRabbit(this);
    }
}

void RunawayRabbit::incrementRunawayLevel() {
    if (mRunawayLevel < 2) {
        mRunawayLevel++;
    }
}

void RunawayRabbit::setLastMessage() {
    MR::forwardNodeCurrentBranchLeft(mMsgCtrl);
}

void RunawayRabbit::setMessage() {
    MR::forwardNodeCurrentBranchRight(mMsgCtrl);
}

void RunawayRabbit::setNotCaughtable() {
    mNotCaughtableTimer = 5;
}

void RunawayRabbit::startJumpSound() {
    if (MR::isBindedGroundWater(this)) {
        MR::startSound(this, "SE_SM_RABBIT_JUMP_WATER");
    } else {
        MR::startSound(this, "SE_SM_RABBIT_JUMP");
    }
}

void RunawayRabbit::setMsgCtrl(TalkMessageCtrl* pMsgCtrl) {
    mMsgCtrl = pMsgCtrl;
}

void RunawayRabbit::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (pSender == getSensor("Catch") && isCaughtable()) {
            mCollect->noticeCaughtRabbit(this);
            MR::requestStartDemoMarioPuppetable(this, "捕まり", GET_NERVE(RunawayRabbit, RunawayRabbitNrvCaught),
                                                GET_NERVE(RunawayRabbit, RunawayRabbitNrvTryCaughtDemo));
        } else if (isCaught()) {
            MR::sendMsgPush(pReceiver, pSender);
        }
    } else {
        MR::sendMsgPushAndKillVelocityToTarget(this, pReceiver, pSender);
    }
}

bool RunawayRabbit::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerTrample(msg) && isCaught()) {
        return true;
    }
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg) && isEnableBlow()) {
        MR::setVelocitySeparateHV(this, pSender, pReceiver, ::sStarPieceHitAccelH, ::sStarPieceHitAccelV);
        MR::limitedStarPieceHitSound();
        MR::startSound(this, "SE_SM_RABBIT_STAR_PIECE_HIT");
        MR::startSound(this, "SE_SV_RABBIT_STAR_PIECE_HIT");
        setNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvBlowDamage));
        return true;
    }

    return false;
}

bool RunawayRabbit::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgToEnemyAttackBlow(msg) && isEnableBlow()) {
        MR::setVelocitySeparateHV(this, pSender, pReceiver, ::sBlowAccelH, ::sBlowAccelV);
        setNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvBlowDamage));
        return true;
    }
    return false;
}

bool RunawayRabbit::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayer(pSender) && isEnableBlow()) {
        MR::addVelocityFromPush(this, ::sPushAccel, pSender, pReceiver);
        return true;
    }

    return false;
}

bool RunawayRabbit::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return MR::isMsgTouchPlantItem(msg);
}

void RunawayRabbit::exeNoActive() {
}

void RunawayRabbit::exeHide() {
    if (!mIsActive) {
        return;
    }

    f32 distToPlayer = MR::calcDistanceToPlayer(this);
    if (distToPlayer <= mNearSoundRadius) {
        s32 fxSend = mHideLocation == HideLocation_Hole ? ::sNearSoundFxSend : 0;
        s32 volume = ::sNearSoundMaxVolume;
        f32 fadeStartDist = mNearSoundRadius - ::sNearSoundRadiusEdge;

        if (distToPlayer >= fadeStartDist) {
            volume = MR::getLinerValueFromMinMax(distToPlayer, fadeStartDist, mNearSoundRadius, ::sNearSoundMaxVolume, ::sNearSoundMinVolume);
        }

        MR::startLevelSound(this, "SE_SV_LV_RABBIT_NEAR2", volume, fxSend);

        if (mHideLocation == HideLocation_Hole) {
            MR::startLevelSound(this, "SE_SM_LV_RABBIT_RUS_HOLE", volume, fxSend);
        } else {
            MR::startLevelSound(this, "SE_SM_LV_RABBIT_RUS_LEAVES", volume, fxSend);
        }
    }
}

void RunawayRabbit::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Jump");

        switch (mHideLocation) {
        case HideLocation_Hole:
            MR::setVelocitySeparateHV(this, mFrontVec, ::sHoleAppearPowerH, ::sHoleAppearPowerV);
            break;
        default:
            MR::setVelocitySeparateHV(this, mPosition - *MR::getPlayerPos(), ::sGroundAppearPowerH, ::sGroundAppearPowerV);
            break;
        }

        MR::startSound(this, "SE_SM_RABBIT_APPEAR");
        MR::startSystemSE("SE_SM_RUNAWAY_RABBIT_APP_ME");
    }

    MR::turnDirectionDegree(this, &mFrontVec, mVelocity, ::sWaitTurnLimit);
    MR::addVelocityToGravity(this, ::sAirGravityAccel);
    MR::attenuateVelocity(this, ::sAirFric);
    if (MR::isGreaterStep(this, 5) && MR::isBindedGround(this)) {
        setNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvRunaway));
        mPressureTimer = 0;
    }
}

void RunawayRabbit::exeRunaway() {
    MR::updateActorState(this, mStateRunaway);
    if (!mStateRunaway->isRunning()) {
        return;
    }
    if (MR::checkPassBckFrame(this, 3.0f)) {
        startJumpSound();
    }
    f32 runawaySpeed = ::sNoPressureAccel;
    if (MR::isNearPlayerHorizontal(this, ::sPressureRange)) {
        if (mPressureTimer < ::sPressureLimitTime) {
            mPressureTimer++;
        }

        s32 runawayTimer = mPressureTimer;
        s32 deccelTime;
        switch (mRunawayLevel) {
        case 0:
            deccelTime = ::sPressureDeccelTimeLv1;
            break;
        case 1:
            deccelTime = ::sPressureDeccelTimeLv2;
            break;
        case 2:
            deccelTime = ::sPressureDeccelTimeLv3;
            break;
        default:
            deccelTime = ::sPressureDeccelTimeLv1;
            break;
        }

        f32 rate = MR::clamp(static_cast< f32 >(runawayTimer - ::sPressureDeccelStart) / deccelTime, 0.0f, 1.0f);
        runawaySpeed = MR::getLinerValue(rate, ::sRunawayAccelMax, ::sRunawayAccelMin, 1.0f);

        if (runawaySpeed < 1.3f) {
            MR::startAction(this, "RunTired");
        } else {
            MR::startAction(this, "Run");
        }
    }

    if (MR::isBindedGroundWater(this)) {
        runawaySpeed *= ::sAwayTargetVelocityRate;
    }

    mStateRunaway->mRunawaySpeed = runawaySpeed;
    MR::setBckRate(this, MR::clamp(runawaySpeed, ::sMinRunBckRate, ::sMaxRunBckRate));
}

void RunawayRabbit::exeTryCaughtDemo() {
}

void RunawayRabbit::exeCaught() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "TossStart");
        MR::startBckPlayer("TossStart");
        MR::startSound(this, "SE_SM_RABBIT_CAUGHT");
        MR::startSoundPlayer("SE_PV_CATCH", -1);
        mSpotMarkLight->kill();
        MR::makeQuatRotateDegree(&mPlayerPoseQuat, *MR::getPlayerRotate());
        mPlayerBindPos.set(*MR::getPlayerPos());
    }

    f32 nerveRate = MR::calcNerveRate(this, ::sMarioPoseBlendTime);
    mPlayerPoseQuat.slerp(mPoseQuat, nerveRate);
    MR::vecBlend(mPlayerBindPos, mPosition, &mPlayerBindPos, nerveRate);

    if (!MR::isBindedGround(this)) {
        MR::addVelocityToGravity(this, 2.0f);
        MR::attenuateVelocity(this, ::sAirFric);
    } else {
        MR::zeroVelocity(this);
    }
    updateBindActorMatrix();
    if (MR::isGreaterEqualStep(this, 7) && MR::isBindedGround(this)) {
        MR::zeroVelocity(this);
        setNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvCaughtTalk));
    }
}

void RunawayRabbit::exeCaughtTalk() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "TossWait");
        MR::startBckPlayer("TossWait");
    }
    mPlayerBindPos.set(mPosition);
    MR::startLevelSound(this, "SE_SM_LV_RABBIT_STRUGGLE");
    MR::zeroVelocity(this);
    updateBindActorMatrix();

    if (mMsgCtrl == nullptr || MR::tryTalkForceWithoutDemoMarioPuppetableAtEnd(mMsgCtrl)) {
        setNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvCaughtEnd));
    }
}

void RunawayRabbit::exeCaughtEnd() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(this);
        MR::startAction(this, "Toss");
        MR::startBckPlayer("Toss");
    }
    if (MR::isStep(this, ::sTossStep)) {
        MR::startSoundPlayer("SE_PV_THROW", -1);
    }
    updateBindActorMatrix();
    if (MR::isBckStopped(this)) {
        MR::endDemo(this, "捕まり");
        setNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvStop));
    }
}

void RunawayRabbit::exeStop() {
    if (MR::isFirstStep(this)) {
        kill();
    } else {
        MR::turnDirectionToPlayerDegree(this, &mFrontVec, ::sDamageTurnLimit);
        if (MR::isBindedGround(this)) {
            MR::zeroVelocity(this);
        } else {
            MR::attenuateVelocity(this, ::sAirFric);
            MR::addVelocityToGravity(this, ::sAirGravityAccel);
        }
    }
}

void RunawayRabbit::exeBlowDamage() {
    MR::updateActorStateAndNextNerve(this, mStateBlowDamage, GET_NERVE(RunawayRabbit, RunawayRabbitNrvRunaway));
}

bool RunawayRabbit::isCaught() const {
    return isNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvStop));
}

bool RunawayRabbit::isCaughtable() const {
    if (isRunnaway() && mNotCaughtableTimer == 0) {
        return true;
    }

    return false;
}

bool RunawayRabbit::isRunnaway() const {
    if (isNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvAppear)) || isNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvTryCaughtDemo)) ||
        isNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvCaught)) || isNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvCaughtTalk)) ||
        isNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvCaughtEnd)) || isNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvStop))) {
        return false;
    }

    return true;
}

bool RunawayRabbit::isChasing() const {
    if (isNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvAppear)) || isNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvRunaway)) ||
        isNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvBlowDamage))) {
        return true;
    }

    return false;
}

bool RunawayRabbit::isEnableBlow() const {
    return isNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvRunaway));
}

bool RunawayRabbit::isValidFollow(s32 id) const {
    if (isNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvNoActive)) || isNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvHide)) ||
        isNerve(GET_NERVE(RunawayRabbit, RunawayRabbitNrvStop))) {
        return false;
    }

    return true;
}
