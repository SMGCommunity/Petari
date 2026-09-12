#include "Game/NPC/StrayTico.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/NPC/CollectTico.hpp"
#include "Game/Util.hpp"

namespace {
    static const f32 sPushAccel = 0.3f;
    static const f32 sWaitTurnDegree = 2.0f;
    static const f32 sChaseTurnDegree = 4.0f;
    // static const f32 sChaseAccel = _;
    // static const f32 sChaseHeightAccel = _;
    static const f32 sChaseFric = 0.97f;
    static const f32 sChaseDistance = 400.0f;
    static const f32 sWarpTryDistance = 2000.0f;
    static const f32 sUpdateWarpDistance = 1600.0f;
    static const f32 sAwayDistance = 300.0f;
    static const f32 sUpVecBlendRate = 0.2f;
    static const f32 sFrontVecBlendRate = 0.2f;
    // static const f32 sSpinSideAccel = _;
    // static const f32 sSpinPullAccel = _;
    // static const f32 sSterPieceAccel = _;
    static const s32 sCompletePlacementTime = 40;
    static const s32 sCompleteRotateStartTime = 60;
    static const s32 sCompleteDemoTime = 200;
    static const f32 sCompleteDemoRotate = 4.0f;
    static const f32 sCompleteStartRadius = 450.0f;
};  // namespace

namespace NrvStrayTico {
    NEW_NERVE(StrayTicoNrvWait, StrayTico, Wait);
    NEW_NERVE(StrayTicoNrvGlad, StrayTico, Glad);
    NEW_NERVE(StrayTicoNrvChase, StrayTico, Chase);
    NEW_NERVE(StrayTicoNrvLauncherCamera, StrayTico, LauncherCamera);
    NEW_NERVE(StrayTicoNrvCompleteDemo, StrayTico, CompleteDemo);
};  // namespace NrvStrayTico

StrayTico::StrayTico(const char* pName, CollectTico* pHost)
    : LiveActor(pName), mHost(pHost), mItemBubbleModel(), mRotateQuat(0.0f, 0.0f, 0.0f, 1.0f), mFrontVec(0.0f, 0.0f, 1.0f), _B0(0.0f, 0.0f, 0.0f),
      _BC(0.0f, 0.0f, 0.0f), _C8(0.0f, 1.0f, 0.0f), _D4(0.0f, 0.0f, 1.0f), _EC(), _F0() {
}

void StrayTico::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("StrayTico", nullptr, false);
    MR::connectToSceneNpc(this);
    MR::makeQuatAndFrontFromRotate(&mRotateQuat, &mFrontVec, this);
    _B0.set(mPosition);
    MR::calcGravity(this);
    initNerve(&NrvStrayTico::StrayTicoNrvWait::sInstance);
    initSensor();
    initShadow();
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initBinder(60.0f, 20.0f, 0);
    MR::offBind(this);

    mItemBubbleModel = MR::createPartsModelMapObj(this, "アワ", "ItemBubble", nullptr);
    mItemBubbleModel->initFixedPosition(TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f), nullptr);

    MR::registerDemoSimpleCastAll(mItemBubbleModel);
    MR::startAction(mItemBubbleModel, "Move");

    MR::useStageSwitchWriteA(this, rIter);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
        mItemBubbleModel->kill();
    } else {
        makeActorAppeared();
        mItemBubbleModel->appear();
    }
}

void StrayTico::initSensor() {
    initHitSensor(2);
    MR::addHitSensorNpc(this, "Bubble", 8, 80.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorNpc(this, "Body", 8, 45.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::validateHitSensor(this, "Bubble");
    MR::invalidateHitSensor(this, "Body");
}

void StrayTico::initShadow() {
    MR::initShadowVolumeOval(this, TVec3f(40.0f, 40.0f, 20.0f));
    MR::setShadowDropPositionAtJoint(this, nullptr, "PowerStarC", TVec3f(0.0f, 0.0f, 0.0f));
    MR::onCalcShadow(this, nullptr);
}

void StrayTico::appear() {
    LiveActor::appear();
    startGlad();
}

void StrayTico::control() {
    MR::blendQuatUpFront(&mRotateQuat, -mGravity, mFrontVec, ::sUpVecBlendRate, ::sFrontVecBlendRate);

    if (isNerve(&NrvStrayTico::StrayTicoNrvCompleteDemo::sInstance)) {
        return;
    }

    if (isNerve(&NrvStrayTico::StrayTicoNrvWait::sInstance)) {
        MR::startLevelSound(this, "SE_SM_LV_STRAYTICO_WAIT");
    } else {
        MR::startLevelSound(this, "SE_SM_LV_STRAYTICO_WAIT_GRP");
    }
}

void StrayTico::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mRotateQuat);
}

void StrayTico::startGlad() {
    mItemBubbleModel->kill();
    MR::onCalcGravity(this);
    MR::onCalcShadow(this, nullptr);
    MR::onBind(this);
    MR::invalidateClipping(this);
    MR::validateHitSensor(this, "Body");
    MR::invalidateHitSensor(this, "Bubble");
    setNerve(&NrvStrayTico::StrayTicoNrvGlad::sInstance);
}

void StrayTico::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(&NrvStrayTico::StrayTicoNrvWait::sInstance) && MR::isSensorNpc(pReceiver) && MR::sendMsgPush(pReceiver, pSender)) {
        MR::addVelocityAwayFromTarget(this, MR::getSensorPos(pReceiver), ::sPushAccel);
    }

    if (isNerve(&NrvStrayTico::StrayTicoNrvWait::sInstance) && MR::isSensorPlayer(pReceiver)) {
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::emitEffect(this, "Break");
        MR::startSound(this, "SE_OJ_ITEM_BUBBLE_BREAK");
        startGlad();
    }
}

bool StrayTico::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceAttack(msg) && isNerve(&NrvStrayTico::StrayTicoNrvWait::sInstance)) {
        MR::limitedStarPieceHitSound();

        return true;
    }

    if (MR::isMsgStarPieceReflect(msg) && !isNerve(&NrvStrayTico::StrayTicoNrvWait::sInstance)) {
        if (isNerve(&NrvStrayTico::StrayTicoNrvChase::sInstance)) {
            TVec3f sensorDir;
            MR::calcSensorDirectionNormalize(&sensorDir, pSender, pReceiver);
            MR::setVelocity(this, sensorDir * 10.0f);
        }

        return true;
    }

    return false;
}

bool StrayTico::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(&NrvStrayTico::StrayTicoNrvWait::sInstance) && MR::isSensorNpc(pSender)) {
        MR::addVelocityAwayFromTarget(this, MR::getSensorPos(pSender), ::sPushAccel);

        return true;
    }

    return false;
}

bool StrayTico::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isEnableSpin() && MR::isMsgSpinStormRange(msg)) {
        MR::addVelocityClockwiseToTarget(this, MR::getSensorPos(pSender), 2.0f);
        MR::addVelocityMoveToTarget(this, MR::getSensorPos(pSender), 1.0f);

        return true;
    }

    return false;
}

bool StrayTico::requestCompleteDemo(const TVec3f& rParam1, const TVec3f& rParam2, f32 param3) {
    _E0 = mPosition;
    _BC = rParam1;
    _C8 = rParam2;
    _EC = param3;

    MR::normalize(&_C8);
    MR::makeAxisVerticalZX(&_D4, _C8);

    TVec3f vec(rParam1 - mPosition);
    vec.killElement(_C8);

    if (MR::normalizeOrZero(&vec)) {
        vec = _D4;
    }

    _F0 = MR::toRadian(MR::diffAngleSigned(vec, _D4, _C8));

    setNerve(&NrvStrayTico::StrayTicoNrvCompleteDemo::sInstance);

    return true;
}

void StrayTico::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }

    if (MR::isNearPlayer(this, 1000.0f)) {
        MR::turnDirectionToTargetDegree(this, &mFrontVec, *MR::getPlayerPos(), ::sWaitTurnDegree);
    }
}

void StrayTico::exeGlad() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Glad", nullptr);
        s32 noRescuedCount = mHost->calcNoRescuedCount();
        MR::startSound(this, "SE_SM_STRAYTICO_SPIN");
        MR::startSound(this, "SE_SM_STRAYTICO_GET", mHost->mTicoNum - noRescuedCount);

        if (noRescuedCount == 1) {
            setNerve(&NrvStrayTico::StrayTicoNrvChase::sInstance);
            return;
        }
    }

    if (MR::isBckStopped(this)) {
        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }

        setNerve(&NrvStrayTico::StrayTicoNrvChase::sInstance);
        MR::startSound(this, "SE_SV_STRAYTICO_GLAD");
    }
}

void StrayTico::exeChase() {
    if (MR::isFirstStep(this)) {
        MR::validateHitSensor(this, "Body");
        MR::invalidateHitSensor(this, "Bubble");
        MR::onBind(this);
        MR::tryStartBck(this, "Chase", nullptr);
        _B0.set(*MR::getPlayerCenterPos());
    }

    updateWarp();
    updateChase();

    if (MR::isActiveLauncherCamera() || MR::isActiveLauncherFlightCamera()) {
        setNerve(&NrvStrayTico::StrayTicoNrvLauncherCamera::sInstance);
    }
}

void StrayTico::exeLauncherCamera() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
        MR::offBind(this);
    }

    TVec3f vec;
    vec.scaleAdd(-400.0f, MR::getCamZdir(), *MR::getPlayerPos());

    MR::addVelocityMoveToTarget(this, vec, 0.5f);
    MR::addVelocityKeepHeight(this, vec, 150.0f, 0.5f, 200.0f);
    MR::attenuateVelocity(this, ::sChaseFric);

    if (MR::isActiveLauncherCamera()) {
        return;
    }

    if (MR::isActiveLauncherFlightCamera()) {
        return;
    }

    setNerve(&NrvStrayTico::StrayTicoNrvChase::sInstance);
}

void StrayTico::updateWarp() {
    bool isNotNearPlayer = !MR::isNearPlayerAnyTime(this, ::sWarpTryDistance);

    if (!isNotNearPlayer) {
        return;
    }

    bool isNotNearPlayer2 = !MR::isNearPlayer(_B0, ::sUpdateWarpDistance);

    if (isNotNearPlayer2) {
        _B0.set(*MR::getPlayerCenterPos());
    }

    if (MR::isJudgedToClipFrustum(mPosition, 200.0f) && MR::isJudgedToClipFrustum(_B0, 200.0f)) {
        MR::resetPosition(this, _B0);
    }
}

void StrayTico::updateChase() {
    bool isNotNearPlayer = !MR::isNearPlayerAnyTime(this, ::sChaseDistance);

    if (isNotNearPlayer) {
        MR::addVelocityMoveToTarget(this, *MR::getPlayerPos(), 0.5f);
        MR::tryStartBck(this, "Chase", nullptr);
    } else if (MR::isNearPlayerAnyTime(this, ::sAwayDistance)) {
        MR::addVelocityAwayFromTarget(this, *MR::getPlayerPos(), 0.5f);
        MR::tryStartBck(this, "Chase", nullptr);
    } else {
        MR::tryStartBck(this, "Wait", nullptr);
    }

    TVec3f playerFrontVec;
    MR::getPlayerFrontVec(&playerFrontVec);

    MR::turnDirectionDegree(this, &mFrontVec, playerFrontVec, ::sChaseTurnDegree);
    MR::addVelocityKeepHeight(this, *MR::getPlayerPos(), 150.0f, 0.5f, 200.0f);
    MR::attenuateVelocity(this, ::sChaseFric);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
}

void StrayTico::exeCompleteDemo() {
    TVec3f vec;

    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
        MR::tryStartBck(this, "CompleteDemo", nullptr);
        MR::offBind(this);
        MR::zeroVelocity(this);
    }

    if (MR::isLessStep(this, ::sCompletePlacementTime)) {
        MR::rotateVecDegree(&vec, _D4 * ::sCompleteStartRadius, _C8, _EC);
        vec.add(_BC);
        MR::vecBlend(_E0, vec, &mPosition, MR::calcNerveEaseInOutRate(this, ::sCompletePlacementTime));
    } else if (MR::isGreaterStep(this, ::sCompleteRotateStartTime)) {
        MR::rotateVecDegree(&mPosition, _D4 * MR::calcNerveValue(this, ::sCompleteRotateStartTime, ::sCompleteDemoTime, 0.0f, 900.0f), _C8,
                            MR::repeatDegree(_EC + MR::calcNerveEaseInValue(this, ::sCompleteRotateStartTime, ::sCompleteDemoTime, 0.0f, 900.0f)));
        mPosition.add(_BC);
    }

    MR::getPlayerFrontVec(&vec);
    MR::turnDirectionDegree(this, &mFrontVec, vec, ::sCompleteDemoRotate);
}

bool StrayTico::isEnableSpin() const {
    if (isNerve(&NrvStrayTico::StrayTicoNrvWait::sInstance) || isNerve(&NrvStrayTico::StrayTicoNrvGlad::sInstance) ||
        isNerve(&NrvStrayTico::StrayTicoNrvLauncherCamera::sInstance)) {
        return false;
    }

    return true;
}

bool StrayTico::isRescued() const {
    if (isNerve(&NrvStrayTico::StrayTicoNrvChase::sInstance) || isNerve(&NrvStrayTico::StrayTicoNrvLauncherCamera::sInstance)) {
        return true;
    }

    return false;
}

bool StrayTico::isCompleteDemoEnd() const {
    if (isNerve(&NrvStrayTico::StrayTicoNrvCompleteDemo::sInstance) && MR::isGreaterStep(this, ::sCompleteDemoTime)) {
        return true;
    }

    return false;
}

void StrayTico::noticeEnd() {
    if (MR::isValidSwitchA(this)) {
        MR::onSwitchA(this);
    }
}
