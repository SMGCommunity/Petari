#include "Game/Enemy/KuriboChief.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/ItemGenerator.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/Enemy/WalkerStateChase.hpp"
#include "Game/Enemy/WalkerStateFindPlayer.hpp"
#include "Game/Enemy/WalkerStateFunction.hpp"
#include "Game/Enemy/WalkerStateParam.hpp"
#include "Game/Enemy/WalkerStateStagger.hpp"
#include "Game/Enemy/WalkerStateWander.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/KeySwitch.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/os.h"
#include "revolution/types.h"

void KuriboChief_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.0f;
    (void)2.0f;
}

namespace {
    static const f32 sBinderSize = 300.0f;
    static const f32 sBinderYOffset = 300.0f;
    static const f32 sUpVecBlendRate = 0.05f;
    static const f32 sFrontVecBlendRate = 0.5f;
    static const f32 sTurnLimitDegree = 5.0f;
    static const s32 sAttackSuccessTime = 60;
    // sStarPieceAttackScaleAccel
    static const s32 sBlowDownLandTime = 5;
    // sPunchHorizontalAccel
    // sPunchUpperAccel
    // sPunchSceneStopTime
    static const s32 sBoundSoundStep = 27;
    static const s32 sSwoonSoundWait = 56;
    static const s32 sSpinSoundStartStep = 10;
    static const s32 sSpinSoundEndStep = 105;
};  // namespace

namespace {
    class KuriboChiefParam {
    public:
        KuriboChiefParam();

        WalkerStateParam mStateParam1;
        WalkerStateParam mStateParam2;
        WalkerStateStaggerParam mStaggerParam;
        WalkerStateFindPlayerParam mFindPlayerParam;
        WalkerStateChaseParam mChaseParam;
        WalkerStateWanderParam mWanderParam;
        AnimScaleParam mScaleParam;
    };

    KuriboChiefParam::KuriboChiefParam() {
        mStateParam1.mGravityAccel = 1.5f;
        mStateParam1.mAirFriction = 0.99f;
        mStateParam1.mGroundFriction = 0.93f;
        mStateParam1.mPlayerNearDistance = 1400.0f;
        mStateParam1.mPlayerSightFanDegreeH = 80.0f;
        mStateParam1.mPlayerSightFanDegreeV = 40.0f;
        mWanderParam.mSpeed = 0.1f;
        mWanderParam.mWaitTime = 120;
        mWanderParam.mWalkTime = 300;
        mWanderParam.mTurnMaxRateDegree = 1.0f;
        mChaseParam.mChaseSpeed = 0.2f;
        mStateParam2.mGravityAccel = 1.5f;
        mStateParam2.mAirFriction = 0.99f;
        mStateParam2.mGroundFriction = 0.6f;
        mStaggerParam.mStaggerTime = 240;
        mStaggerParam.mRotateRateDegree = 0.0f;
        mStaggerParam.mKickEnableStep = 70;
        mFindPlayerParam.mJumpStartStep = 56;
        mFindPlayerParam.mTurnMaxRateDegree = 2.0f;
        mFindPlayerParam.mJumpVelocity = 35.0f;
        mScaleParam._0 = 0.1f;
        mScaleParam._10 = 30.0f;
        mScaleParam._14 = 0.6f;
        mScaleParam._18 = 0.06f;
        mScaleParam._1C = 3.0f;
        mScaleParam._24 = 0.1f;
    }

    static KuriboChiefParam sParam;
};  // namespace

namespace NrvKuriboChief {
    NEW_NERVE(KuriboChiefNrvWander, KuriboChief, Wander);
    NEW_NERVE(KuriboChiefNrvFindPlayer, KuriboChief, FindPlayer);
    NEW_NERVE(KuriboChiefNrvChase, KuriboChief, Chase);
    NEW_NERVE(KuriboChiefNrvAttackSuccess, KuriboChief, AttackSuccess);
    NEW_NERVE(KuriboChiefNrvStagger, KuriboChief, Stagger);
    NEW_NERVE(KuriboChiefNrvTrample, KuriboChief, Trample);
    NEW_NERVE_ONEND(KuriboChiefNrvBindStarPointer, KuriboChief, BindStarPointer, BindStarPointer);
    NEW_NERVE(KuriboChiefNrvBlowDown, KuriboChief, BlowDown);
    NEW_NERVE(KuriboChiefNrvBlowDownLand, KuriboChief, BlowDownLand);
};  // namespace NrvKuriboChief

KuriboChief::KuriboChief(const char* pName) : LiveActor(pName) {
    mScaleController = nullptr;
    mStateWander = nullptr;
    mStateFindPlayer = nullptr;
    mStateChase = nullptr;
    mStateStagger = nullptr;
    mStateBindStarPointer = nullptr;
    mItemGenerator = nullptr;
    mKeySwitch = nullptr;
    _AC.x = 0.0f;
    _AC.y = 0.0f;
    _AC.z = 0.0f;
    _AC.w = 1.0f;
    _BC.set(0.0f, 0.0f, 1.0f);
}

KuriboChief::~KuriboChief() {
}

void KuriboChief::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("KuriboChief", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    MR::onCalcGravity(this);
    mItemGenerator = new ItemGenerator();
    initEffectKeeper(1, nullptr, false);
    MR::initShadowFromCSV(this, "Shadow");
    mScaleController = new AnimScaleController(&::sParam.mScaleParam);
    MR::initStarPointerTargetAtJoint(this, "Body", 350.0f, TVec3f(350.0f, 0.0f, 0.0f));
    initSensor();
    initBinder(::sBinderSize, ::sBinderYOffset, 0);
    initNerve(&NrvKuriboChief::KuriboChiefNrvWander::sInstance);
    initSound(6, false);
    initState();
    initKeySwitch(rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    MR::useStageSwitchSleep(this, rIter);
    MR::addBaseMatrixFollowTarget(this, rIter, nullptr, nullptr);
    MR::declareStarPiece(this, 6);

    s32 arg = -1;
    MR::getJMapInfoArg7WithInit(rIter, &arg);
    if (arg != -1) {
        MR::declareCameraRegisterVec(this, arg, &mPosition);
    }

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void KuriboChief::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
}

void KuriboChief::initSensor() {
    LiveActor::initHitSensor(8);
    MR::addHitSensorAtJointEnemy(this, "Body", "Body", 8, 150.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointEnemy(this, "Head1", "Head", 8, 300.0f, TVec3f(120.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointEnemy(this, "Head2", "Head", 8, 150.0f, TVec3f(360.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointEnemy(this, "LegL", "LegL", 8, 130.0f, TVec3f(60.0f, -50.0f, 0.0f));
    MR::addHitSensorAtJointEnemy(this, "LegR", "LegR", 8, 130.0f, TVec3f(60.0f, -50.0f, 0.0f));
    MR::addHitSensorEnemy(this, "Punch", 8, 400.0f, TVec3f(0.0f, 150.0f, 0.0f));
}

void KuriboChief::initState() {
    mStateFindPlayer = new WalkerStateFindPlayer(this, &_BC, &::sParam.mStateParam1, &::sParam.mFindPlayerParam);
    mStateWander = new WalkerStateWander(this, &_BC, &::sParam.mStateParam1, &::sParam.mWanderParam);
    mStateChase = new WalkerStateChase(this, &_BC, &::sParam.mStateParam1, &::sParam.mChaseParam);
    mStateStagger = new WalkerStateStagger(this, &_BC, &::sParam.mStateParam2, &::sParam.mStaggerParam);
    mStateBindStarPointer = new WalkerStateBindStarPointer(this, mScaleController);
}

void KuriboChief::initKeySwitch(const JMapInfoIter& rIter) {
    if (MR::useStageSwitchWriteA(this, rIter)) {
        mKeySwitch = new KeySwitch("鍵スイッチ");
        mKeySwitch->initKeySwitchByOwner(rIter);
    }
}

void KuriboChief::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    if (MR::isExistStageSwitchA(rIter)) {
        pArchiveList->addArchive("KeySwitch");
    }
}

void KuriboChief_FORCE_MATCH_DATA() {
    OSReport("head");
}

void KuriboChief::kill() {
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    if (mKeySwitch != nullptr) {
        mKeySwitch->appearKeySwitch(mPosition);
    } else {
        mItemGenerator->generate(this);
    }

    LiveActor::kill();
}

void KuriboChief::control() {
    if (mScaleController != nullptr) {
        mScaleController->updateNerve();
    }

    MR::blendQuatFromGroundAndFront(&_AC, this, _BC, ::sUpVecBlendRate, ::sFrontVecBlendRate);
}

void KuriboChief::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _AC);
    MR::setBaseScale(this, mScaleController->_C * mScale);
}

void KuriboChief::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (pSender != getSensor("Punch") &&
        ((isEnableAttack() || !MR::isSensorPlayer(pReceiver)) && !MR::isSensorEnemy(pReceiver) ||
         !MR::sendMsgPushAndKillVelocityToTarget(this, pReceiver, pSender)) &&
        isEnableAttack() && MR::isSensorPlayer(pReceiver) && !MR::sendMsgEnemyAttack(pReceiver, pSender)) {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool KuriboChief::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver != getSensor("Punch")) {
        if (MR::isMsgStarPieceReflect(msg)) {
            mScaleController->startHitReaction();
            return true;
        }

        if (MR::isMsgInvincibleAttack(msg)) {
            return requestBlowDown(pSender, pReceiver);
        }

        return false;
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        if (isEnableKick()) {
            return requestBlowDown(pSender, pReceiver);
        } else {
            return requestStagger(pSender, pReceiver);
        }
    }

    return false;
}

bool KuriboChief::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver == getSensor("Punch")) {
        return false;
    }

    if (MR::isMsgToEnemyAttackBlow(msg)) {
        return requestStagger(pSender, pReceiver);
    }

    if (MR::isMsgToEnemyAttackShockWave(msg)) {
        return requestStagger(pSender, pReceiver);
    }

    return false;
}

bool KuriboChief::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver == getSensor("Punch")) {
        return false;
    }

    if (MR::isMsgInhaleBlackHole(msg)) {
        mItemGenerator->setTypeNone();
        kill();
        return true;
    }

    if (MR::isMsgPlayerKick(msg) && MR::isSensorPlayer(pSender) && isEnableKick() && requestBlowDown(pSender, pReceiver)) {
        mItemGenerator->setTypeStarPeace(6);
        return true;
    }

    return false;
}

bool KuriboChief::requestStagger(HitSensor* pSender, HitSensor* pReceiver) {
    bool isDwn = isDown() == false;
    if (isDwn) {
        mStateStagger->setPunchDirection(pSender, pReceiver), setNerve(&NrvKuriboChief::KuriboChiefNrvStagger::sInstance);
        return true;
    }

    return false;
}

bool KuriboChief::requestBlowDown(HitSensor* pSender, HitSensor* pReceiver) {
    bool isDwn = isDown() == false;
    if (!isDwn) {
        return false;
    }

    MR::setVelocityBlowAttack(this, pSender, pReceiver, 25.0f, 35.0f, 4);
    setNerve(&NrvKuriboChief::KuriboChiefNrvBlowDown::sInstance);
    return true;
}

bool KuriboChief::tryFind() {
    if (mStateFindPlayer->isInSightPlayer()) {
        setNerve(&NrvKuriboChief::KuriboChiefNrvFindPlayer::sInstance);
        return true;
    }

    return false;
}

bool KuriboChief::tryPointBind() {
    if (mStateBindStarPointer->tryStartPointBind()) {
        setNerve(&NrvKuriboChief::KuriboChiefNrvBindStarPointer::sInstance);
        return true;
    }

    return false;
}

void KuriboChief::exeWander() {
    MR::updateActorState(this, mStateWander);
    if (!tryFind() && tryPointBind()) {
        return;
    }
}

void KuriboChief::exeFindPlayer() {
    if (!MR::updateActorStateAndNextNerve(this, mStateFindPlayer, &NrvKuriboChief::KuriboChiefNrvChase::sInstance)) {
        if (mStateFindPlayer->isFindJumpBegin()) {
            MR::startSound(this, "SE_EM_KURIBOCHIEF_FIND");
        }

        if (mStateFindPlayer->isLandStart()) {
            MR::startSound(this, "SE_EM_KURIBOCHIEF_LAND");
        }

        if (tryPointBind()) {
            return;
        }
    }
}

void KuriboChief::exeChase() {
    if (MR::updateActorStateAndNextNerve(this, mStateChase, &NrvKuriboChief::KuriboChiefNrvWander::sInstance)) {
        mStateWander->setWanderCenter(mPosition);
    }

    if (tryPointBind()) {
        return;
    }
}

void KuriboChief::exeStagger() {
    if (!MR::updateActorStateAndNextNerve(this, mStateStagger, &NrvKuriboChief::KuriboChiefNrvWander::sInstance)) {
        if (mStateStagger->isStaggerStart()) {
            MR::startSound(this, "SE_EM_KURIBOCHIEF_BLOW");
            MR::startBlowHitSound(this);
        }

        if (MR::isStep(this, ::sBoundSoundStep)) {
            MR::startSound(this, "SE_EM_KURIBOCHIEF_BOUND");
        }

        if (mStateStagger->isSwooning(::sSwoonSoundWait)) {
            MR::startLevelSound(this, "SE_EM_LV_SWOON_S");
        }

        if (mStateStagger->isSpinning(::sSpinSoundStartStep, ::sSpinSoundEndStep)) {
            MR::startLevelSound(this, "SE_EM_LV_KURIBOCHIEF_STAGGER");
        }

        if (mStateStagger->isRecoverStart()) {
            MR::startSound(this, "SE_EM_KURIBOCHIEF_RECOVER");
        }
    }
}

void KuriboChief::exeTrample() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_KURIBOCHIEF_TRAMPLE");
    }

    if (MR::isGreaterStep(this, 30)) {
        setNerve(&NrvKuriboChief::KuriboChiefNrvWander::sInstance);
    }
}

void KuriboChief::exeBindStarPointer() {
    MR::updateActorStateAndNextNerve(this, mStateBindStarPointer, &NrvKuriboChief::KuriboChiefNrvWander::sInstance);
}

void KuriboChief::endBindStarPointer() {
    mStateBindStarPointer->kill();
}

void KuriboChief::exeAttackSuccess() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Hit");
    }

    MR::turnDirectionToPlayerDegree(this, &_BC, ::sTurnLimitDegree);
    WalkerStateFunction::calcPassiveMovement(this, &::sParam.mStateParam1);

    if (MR::isGreaterStep(this, ::sAttackSuccessTime)) {
        setNerve(&NrvKuriboChief::KuriboChiefNrvWander::sInstance);
    }

    if (tryPointBind()) {
        return;
    }
}

void KuriboChief::exeBlowDown() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "BlowDown");
        MR::startBlowHitSound(this);
        MR::startSound(this, "SE_EM_KURIBOCHIEF_BLOW");
        MR::startSystemSE("SE_SY_VS_BOSS_LAST_HIT");
    }

    WalkerStateFunction::calcPassiveMovement(this, &::sParam.mStateParam1);
    MR::turnDirectionDegree(this, &_BC, -mVelocity, 30.0f);

    if (MR::isGreaterStep(this, ::sBlowDownLandTime) && MR::isBindedGround(this)) {
        setNerve(&NrvKuriboChief::KuriboChiefNrvBlowDownLand::sInstance);
        MR::zeroVelocity(this);
    }
}

void KuriboChief::exeBlowDownLand() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "BlowDownLand");
        MR::startSound(this, "SE_EM_KURIBOCHIEF_BOUND_LAST");
    }

    if (MR::isBckStopped(this)) {
        MR::emitEffect(this, "Death");
        MR::startSound(this, "SE_EM_KURIBOCHIEF_RUN");
        kill();
    }
}

bool KuriboChief::isEnableAttack() const {
    if (isNerve(&NrvKuriboChief::KuriboChiefNrvWander::sInstance) || isNerve(&NrvKuriboChief::KuriboChiefNrvFindPlayer::sInstance) ||
        isNerve(&NrvKuriboChief::KuriboChiefNrvChase::sInstance)) {
        return true;
    }

    return false;
}

bool KuriboChief::isEnableKick() const {
    if (isNerve(&NrvKuriboChief::KuriboChiefNrvStagger::sInstance)) {
        return mStateStagger->isEnableKick();
    }

    return false;
}

bool KuriboChief::isDown() const {
    if (isNerve(&NrvKuriboChief::KuriboChiefNrvBlowDown::sInstance) || isNerve(&NrvKuriboChief::KuriboChiefNrvBlowDownLand::sInstance)) {
        return true;
    }

    return false;
}
