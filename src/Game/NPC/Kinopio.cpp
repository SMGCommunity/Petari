#include "Game/NPC/Kinopio.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjConnector.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/NPC/TalkMessageFunc.hpp"
#include "Game/Util.hpp"

namespace NrvKinopio {
    NEW_NERVE(KinopioNrvReaction, Kinopio, Reaction);
    NEW_NERVE(KinopioNrvTakeOutStar, Kinopio, TakeOutStar);
    NEW_NERVE(KinopioNrvFar, Kinopio, Jump);
    NEW_NERVE(KinopioNrvNear, Kinopio, Jump);
    NEW_NERVE(KinopioNrvWait, Kinopio, Wait);
    NEW_NERVE(KinopioNrvStop0, Kinopio, Stop);
    NEW_NERVE(KinopioNrvStop1, Kinopio, Stop);
    NEW_NERVE(KinopioNrvAction, Kinopio, Action);
    NEW_NERVE(KinopioNrvAppear, Kinopio, Appear);
    NEW_NERVE(KinopioNrvEscape, Kinopio, Escape);
    NEW_NERVE(KinopioNrvStarWait, Kinopio, StarWait);
    NEW_NERVE(KinopioNrvMount, Kinopio, Mount);
    NEW_NERVE(KinopioNrvEvent, Kinopio, Event);
    NEW_NERVE(KinopioNrvEventAndTalk, Kinopio, EventAndTalk);
    NEW_NERVE(KinopioNrvEventTalking, Kinopio, EventAndTalk);
    NEW_NERVE(KinopioNrvEventFall, Kinopio, EventFall);
};  // namespace NrvKinopio

namespace {
    class Param : public AnimScaleParam {
    public:
        Param() {
            _10 = 15.0f;
            _14 = 1.4f;
            _20 = 0.3f;
        }
    };

    static Param sParam;

    // const double sDistancePlayerIsNear = 0;
    // const f32 sDistancePlayerIsFar = 0;
    // const double sDistancePlayerIsEvent = 0;
    // const double sTurnLimit = 0;
    // const double sUpBlendRate
};  // namespace

Kinopio::Kinopio(const char* pName)
    : NPCActor(pName), mObjArg0(), mBehavior(-1), mAppearBehavior(-1), mEquipment(-1), _17C(), mTakeOutStar(), mIsSpawnPowerStarMode(), _185() {
    _16C.set(0.0f, 0.0f, 0.0f, 1.0f);
}

void Kinopio::init(const JMapInfoIter& rIter) {
    MR::initDefaultPosAndQuat(this, rIter);
    TVec3f rotation(mRotation);
    rotation.scale(MR::toRadian(1.0f));
    _A0.setEuler(rotation);
    MR::getJMapInfoArg1NoInit(rIter, &mObjArg0);
    MR::getJMapInfoArg2NoInit(rIter, &mBehavior);
    MR::getJMapInfoArg3NoInit(rIter, &mIsSpawnPowerStarMode);
    MR::getJMapInfoArg4NoInit(rIter, &mAppearBehavior);
    MR::getJMapInfoArg7NoInit(rIter, &mEquipment);
    mScale.set(1.2f);
    initModelManagerWithAnm("Kinopio", nullptr, false);
    MR::connectToSceneNpc(this);
    MR::initLightCtrl(this);
    initSound(4, false);
    initHitSensor(2);
    MR::addHitSensorNpc(this, "body", 8, 60, TVec3f(0.0f, (f32)60, 0.0f));
    MR::addHitSensorNpc(this, "Pickel", 8, 60, TVec3f(0.0f, (f32)60, 48.0f));
    MR::invalidateHitSensor(this, "Pickel");
    s32 isInvalidateSensor = -1;
    MR::getJMapInfoArg6NoInit(rIter, &isInvalidateSensor);
    if (isInvalidateSensor != -1) {
        MR::invalidateHitSensor(this, "body");
    }
    initBinder(60, 60, 0);
    MR::offBind(this);
    MR::initShadowVolumeSphere(this, 36.0f);
    MR::onCalcShadowOneTime(this, nullptr);
    initEffectKeeper(0, nullptr, false);
    initNerve(&NrvKinopio::KinopioNrvWait::sInstance);
    MR::createTalkCtrl(this, rIter, "Kinopio", TVec3f(0.0f, 146.40001f, 0.0f), nullptr);
    MR::setDistanceToTalk(mMsgCtrl, 200.0f);
    MR::onRootNodeAutomatic(mMsgCtrl);
    MR::registerEventFunc(mMsgCtrl, TalkMessageFunc(this, &Kinopio::eventFunc));
    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        MR::moveCoordAndTransToRailStartPoint(this);
        MR::onCalcShadow(this, nullptr);
    }
    MR::initStarPointerTarget(this, 100.0f, TVec3f(0.0f));
    MR::addToAttributeGroupSearchTurtle(this);
    s32 isStrictLodDistances = -1;
    MR::getJMapInfoArg5NoInit(rIter, &isStrictLodDistances);
    mLodCtrl = MR::createLodCtrlNPC(this, rIter);
    if (isStrictLodDistances != -1) {
        mLodCtrl->setDistanceToMiddleAndLow(1500.0f, 2500.0f);
    }
    NPCActorItem item("Kinopio");
    MR::getNPCItemData(&item, mEquipment);
    equipment(item, false);
    mScaleController = new AnimScaleController(&sParam);
    JointController* controller = new JointController();
    MR::setJointControllerParam(controller, this, "body");
    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchReadB(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    setNerve(&NrvKinopio::KinopioNrvWait::sInstance);
}

void Kinopio::exeReaction() {
    if (mBehavior == 7) {
        if (_D8) {
            MR::startSound(this, "SE_SM_NPC_TRAMPLED");
            MR::startSound(this, "SE_SV_KINOPIO_POINTING_SLEEP");
        }
        if (isPointingSe()) {
            MR::startDPDHitSound();
            MR::startSound(this, "SE_SV_KINOPIO_POINTING");
        }
        if (_D9) {
            MR::startSound(this, "SE_SM_NPC_TRAMPLED");
            MR::startSound(this, "SE_SV_KINOPIO_POINTING_SLEEP");
        }
        if (_DB) {
            MR::startSound(this, "SE_SM_NPC_TRAMPLED");
            MR::startSound(this, "SE_SV_KINOPIO_SPIN");
        }
    } else {
        if (_D8) {
            MR::startSound(this, "SE_SM_NPC_TRAMPLED");
            MR::startSound(this, "SE_SV_KINOPIO_TRAMPLED");
        }
        if (isPointingSe()) {
            MR::startDPDHitSound();
            MR::startSound(this, "SE_SV_KINOPIO_POINTING");
        }
        if (_D9) {
            MR::startSound(this, "SE_SV_KINOPIO_SPIN");
        }
        if (_DB) {
            MR::startSound(this, "SE_SV_KINOPIO_STAR_PIECE_HIT");
        }
    }
    if (MR::tryStartReactionAndPopNerve(this)) {
        return;
    }
}

bool Kinopio::isPlayerNotNearKinopio() {
    return !MR::isNearPlayer(this, 300.0f);
}

void Kinopio::exeJump() {
    s32 nextRailPoint;
    if (MR::isExistRail(this)) {
        nextRailPoint = MR::getNextRailPointNo(this);
    } else {
        nextRailPoint = 0;
    }
    if (MR::isFirstStep(this) && !MR::isExistRail(this)) {
        MR::onCalcShadowOneTime(this, nullptr);
    }
    if (MR::tryStartReactionAndPushNerve(this, &NrvKinopio::KinopioNrvReaction::sInstance)) {
        return;
    }
    if (MR::tryTalkNearPlayerAtEnd(mMsgCtrl)) {
        MR::startAction(this, "Notice");
    }
    if (MR::isActionContinuous(this)) {
        if (!isNerve(&NrvKinopio::KinopioNrvNear::sInstance)) {
            return;
        }
        if (MR::isActionStart(this, "Notce")) {
            turnToPlayer(mParam._8);
        }
        return;
    }
    if (isNerve(&NrvKinopio::KinopioNrvFar::sInstance)) {
        if (MR::isNearPlayer(this, 200.0f)) {
            MR::startAction(this, "Notice");
            setNerve(&NrvKinopio::KinopioNrvNear::sInstance);
            return;
        }
        MR::tryStartMoveTalkAction(this);
    } else {
        if (isPlayerNotNearKinopio()) {
            setNerve(&NrvKinopio::KinopioNrvFar::sInstance);
        }
        MR::tryStartTalkAction(this);
    }
    if (MR::isExistRail(this)) {
        if (MR::getNextRailPointNo(this) != nextRailPoint) {
            tryStartArgs();
        }
    }
}

void Kinopio::exeStop() {
    if (MR::isFirstStep(this)) {
        _10C = 0.0f;
    }
    if (!(MR::tryStartReactionAndPushNerve(this, &NrvKinopio::KinopioNrvReaction::sInstance) || MR::tryStartMoveTalkAction(this))) {
        if (isNerve(&NrvKinopio::KinopioNrvStop0::sInstance)) {
            if (MR::isGreaterEqualStep(this, 60)) {
                setDefaultNerve();
            }
            return;
        }
        if (MR::isGreaterEqualStep(this, 120)) {
            setDefaultNerve();
        }
    }
}

void Kinopio::exeAction() {
    if (MR::isFirstStep(this)) {
        _10C = 0.0f;
    }
    MR::startMoveAction(this);
    if (!MR::isActionContinuous(this)) {
        if (MR::isEqualStringCase(MR::getPlayingBckName(this), "Tumble")) {
            MR::startAction(this, "Getup");
            setNerve(&NrvKinopio::KinopioNrvAction::sInstance);
        } else {
            setDefaultNerve();
        }
    }
}

void Kinopio::exeWait() {
    s32 nextRailPoint;
    if (MR::isExistRail(this)) {
        nextRailPoint = MR::getNextRailPointNo(this);
    } else {
        nextRailPoint = 0;
    }
    if (MR::isFirstStep(this) && !MR::isExistRail(this)) {
        MR::onCalcShadowOneTime(this, nullptr);
    }
    if (!MR::tryStartReactionAndPushNerve(this, &NrvKinopio::KinopioNrvReaction::sInstance)) {
        MR::tryTalkNearPlayerAndStartMoveTalkAction(this);
        if (MR::isExistRail(this) && MR::getNextRailPointNo(this) != nextRailPoint) {
            tryStartArgs();
        }
    }
}

void Kinopio::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Change");
        MR::invalidateClipping(this);
        MR::onBind(this);
        MR::onCalcShadow(this, nullptr);
    }

    MR::calcGravity(this);
    TVec3f fallSpeed(mGravity);
    fallSpeed.scale(0.6f);
    mVelocity.add(fallSpeed);
    mVelocity.mult(0.98f);
    faceToPlayer(0.997f);

    if (MR::isBinded(this)) {
        mVelocity.zero();
        if (MR::isBckStopped(this)) {
            MR::validateClipping(this);
            popNerve();
        }
    }
}

void Kinopio::exeEscape() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Escape");
        MR::startSound(this, "SE_SV_KINOPIO_PUHA");
        MR::startSound(this, "SE_SM_NPC_FLY_OUT");
        MR::invalidateClipping(this);
    }
    if (MR::isAnyAnimOneTimeAndStopped(this, "Escape")) {
        MR::validateClipping(this);
        popNerve();
    }
}

void Kinopio::exeMount() {
    if (MR::isFirstStep(this) && _17C->mParts == nullptr) {
        MR::calcGravity(this);
        MR::setBaseTRMtx(this, _A0);
        _17C->attachToUnder();
        _B0.set(0.0f, 0.0f, 0.0f, 1.0f);
        _A0.set(0.0f, 0.0f, 0.0f, 1.0f);
    }
    if (!MR::tryStartReactionAndPushNerve(this, &NrvKinopio::KinopioNrvReaction::sInstance)) {
        if (MR::tryTalkNearPlayerAndStartTalkAction(this)) {
            faceToPlayer(0.997f);
        } else {
            faceToDefault(0.997f);
        }
    }
}

void Kinopio::exeStarWait() {
    exeWait();
    if (MR::isTalkTalking(mMsgCtrl) && !_185) {
        MR::startStageBGM("MBGM_KINOPIO_TANKEN_B", false);
        _185 = true;
    }
}

void Kinopio::exeEventAndTalk() {
    MR::tryTalkRequest(mMsgCtrl);
    if (MR::isNearPlayer(mMsgCtrl, 600.0f)) {
        MR::tryTalkNearPlayer(mMsgCtrl);
    }
}

void Kinopio::exeEventFall() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
        MR::onBind(this);
        MR::onCalcShadow(this, nullptr);
    }
    if (MR::isBindedWall(this) || MR::isBindedRoof(this)) {
        mVelocity.zero();
    }

    MR::calcGravity(this);
    TVec3f fallSpeed(mGravity);
    fallSpeed.scale(0.6f);
    mVelocity.add(fallSpeed);
    mVelocity.mult(0.98f);
    faceToPlayer(0.997f);

    if (!tryCancelEvent() && MR::isBindedGround(this) && MR::isGreaterStep(this, 3)) {
        mVelocity.zero();
        MR::validateClipping(this);
        MR::turnQuatYDirRate(&_A0, _A0, -mGravity, 1.0f);
        setInitPose();
        MR::tryForwardNode(mMsgCtrl);
        setNerve(&NrvKinopio::KinopioNrvWait::sInstance);
    }
}

void Kinopio::exeEvent() {
}

void Kinopio::exeTakeOutStar() {
}

// void Kinopio::calcAndSetBaseMtx() {}

bool Kinopio::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensor(pReceiver, "Pickel")) {
        return false;
    }
    NPCActor::receiveMsgPlayerAttack(msg, pSender, pReceiver);
}

bool Kinopio::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_NPC_EVENT_START) {
        MR::offRootNodeAutomatic(mMsgCtrl);
        MR::onCalcAnim(this);
        setNerve(&NrvKinopio::KinopioNrvEvent::sInstance);
        return true;
    }
    if (msg == ACTMES_NPC_EVENT_END) {
        setNerve(&NrvKinopio::KinopioNrvEventFall::sInstance);
        return true;
    }
    if (msg == ACTMES_NPC_EVENT_TALK_ENABLE) {
        if (!isNerve(&NrvKinopio::KinopioNrvEvent::sInstance)) {
            return false;
        }
        setNerve(&NrvKinopio::KinopioNrvEventAndTalk::sInstance);
        return true;
    }
    if (msg == ACTMES_NPC_EVENT_TALK_DISABLE) {
        if (!(isNerve(&NrvKinopio::KinopioNrvEventAndTalk::sInstance) || isNerve(&NrvKinopio::KinopioNrvEventTalking::sInstance))) {
            return false;
        }
        setNerve(&NrvKinopio::KinopioNrvEvent::sInstance);
        return true;
    }
    if (msg == ACTMES_NPC_TALK_NEXT) {
        MR::tryForwardNode(mMsgCtrl);
        return true;
    }
    return false;
}

void Kinopio::faceToVector(const TVec3f& rVec, f32 f) {
    TVec3f up(-mGravity);
    TQuat4f vec4;
    f32 sqrt = _16C.dot(_16C);
    if (sqrt <= MR::epsilon()) {
        vec4.set(0.0f, 0.0f, 0.0f, 1.0f);
    } else {
    }
    TVec3f dirY, dirZ;
    _A0.getYDir(dirY);
    _A0.getZDir(dirZ);
    MR::normalize(&mGravity);
    vec4.transform(mVelocity);
    vec4.transform(mGravity);
}

void Kinopio::setDefaultNerve() {
    if (mBehavior == 13) {
        _10C = 3.32f;
    } else {
        _10C = 0.83f;
    }
    if (mBehavior == -1) {
        if (MR::isNearPlayer(this, 200.0f)) {
            setNerve(&NrvKinopio::KinopioNrvNear::sInstance);
        } else {
            setNerve(&NrvKinopio::KinopioNrvFar::sInstance);
        }
    } else {
        setNerve(&NrvKinopio::KinopioNrvWait::sInstance);
    }
}

bool Kinopio::tryStartArgs() {
    s32 pointArg0;
    MR::getCurrentRailPointArg0WithInit(this, &pointArg0);
    switch (pointArg0) {
    case 0:
        setNerve(&NrvKinopio::KinopioNrvStop0::sInstance);
        break;
    case 1:
        setNerve(&NrvKinopio::KinopioNrvStop1::sInstance);
        break;
    case 2:
        MR::startAction(this, "Notice");
        setNerve(&NrvKinopio::KinopioNrvAction::sInstance);
        break;
    case 3:
        MR::startAction(this, "Tumble");
        setNerve(&NrvKinopio::KinopioNrvAction::sInstance);
        break;
    default:
        return false;
        break;
    }
    return true;
}

bool Kinopio::tryCancelEvent() {
    if (MR::isGreaterStep(this, 180)) {
        kill();
        return true;
    }
    return false;
}

bool Kinopio::eventFunc(u32) {
    if (mTakeOutStar->takeOut()) {
        if (!MR::isEqualStageName("FishTunnelGalaxy")) {
            MR::startLastStageBGM();
        }
        return true;
    }
    return false;
}
