#include "Game/NPC/Kinopio.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjConnector.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/NPC/TalkMessageFunc.hpp"
#include "Game/Util.hpp"

void Kinopio_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.000003814697265625f;
}

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
        Param();
    };

    Param::Param() {
        _10 = 15.0f;
        _14 = 1.4f;
        _20 = 0.3f;
    }

    static Param sParam;

    static const f32 sDistancePlayerIsNear = 200.0f;
    static const f32 sDistancePlayerIsFar = 300.0f;
    static const f32 sDistancePlayerIsEvent = 450.0f;
    static const f32 sTurnLimit = 0.997f;
    static const f32 sUpVecBlendRate = 1.0f;
    static const f32 sDistanceEventPlayerIsNear = 600.0f;
    static const s32 sTimeToCancelEvent = 180;
    static const f32 sDistanceLodMiddle = 1500.0f;
    static const f32 sDistanceLodLow = 2500.0f;
    static const f32 sFallGravity = 0.6f;
    static const f32 sFallFrictionRate = 0.98f;
    static const s32 sLandingTime = 3;
    static const s32 sPickelIndex = 2;
    static const f32 sDefaultScale = 1.2f;
    static const f32 sWalkSpeed = 0.83f;
    static const f32 sRunSpeed = 3.32f;
};  // namespace

Kinopio::Kinopio(const char* pName)
    : NPCActor(pName), mObjArg0(), mBehavior(-1), mAppearBehavior(-1), mGoodsIndex(-1), _17C(), mTakeOutStar(), mIsSpawnPowerStarMode(), _185() {
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
    MR::getJMapInfoArg7NoInit(rIter, &mGoodsIndex);
    mScale.set(::sDefaultScale);
    initModelManagerWithAnm("Kinopio", nullptr, false);
    MR::connectToSceneNpc(this);
    MR::initLightCtrl(this);
    initSound(4, false);
    initHitSensor(2);
    MR::addHitSensorNpc(this, "body", 8, 50.0f * ::sDefaultScale, TVec3f(0.0f, 50.0f * ::sDefaultScale, 0.0f));
    MR::addHitSensorNpc(this, "Pickel", 8, 50.0f * ::sDefaultScale, TVec3f(0.0f, 50.0f * ::sDefaultScale, 40.0f * ::sDefaultScale));
    MR::invalidateHitSensor(this, "Pickel");
    s32 isInvalidateSensor = -1;
    MR::getJMapInfoArg6NoInit(rIter, &isInvalidateSensor);
    if (isInvalidateSensor != -1) {
        MR::invalidateHitSensor(this, "body");
    }

    initBinder(50.0f * ::sDefaultScale, 50.0f * ::sDefaultScale, 0);
    MR::offBind(this);
    MR::initShadowVolumeSphere(this, 30.0f * ::sDefaultScale);
    MR::onCalcShadowOneTime(this, nullptr);
    initEffectKeeper(0, nullptr, false);
    initNerve(GET_NERVE(Kinopio, KinopioNrvWait));
    mMsgCtrl = MR::createTalkCtrl(this, rIter, "Kinopio", TVec3f(0.0f, 122.0f * ::sDefaultScale, 0.0f), nullptr);
    MR::setDistanceToTalk(mMsgCtrl, ::sDistancePlayerIsNear);
    MR::onRootNodeAutomatic(mMsgCtrl);
    MR::registerEventFunc(mMsgCtrl, TalkMessageFuncM< Kinopio*, bool (Kinopio::*)(u32) >(this, &Kinopio::eventFunc));
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
        mLodCtrl->setDistanceToMiddleAndLow(::sDistanceLodMiddle, ::sDistanceLodLow);
    }

    NPCActorItem item("Kinopio");
    MR::getNPCItemData(&item, mGoodsIndex);
    equipment(item, false);
    mScaleController = new AnimScaleController(&sParam);
    mDelegator = MR::createJointDelegatorWithNullChildFunc< Kinopio >(this, &Kinopio::calcJointScale, "body");

    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchReadB(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    MR::tryRegisterDemoCast(this, rIter);
    setNerve(GET_NERVE(Kinopio, KinopioNrvWait));
    setDefaults();
    _12C = ::sDistancePlayerIsEvent;
    mParam.setDefaultAction();
    mParam.setTalkAction("Talk", "Talk");
    setTalkAction("Walk");
    _124 = true;
    _10C = ::sWalkSpeed;
    switch (mBehavior) {
    case -1:
        if (mGoodsIndex == ::sPickelIndex) {
            mParam.setMoveAction("WaitPickel", "WaitPickel");
            mParam.setTalkAction("TalkPickel", "TalkPickel");
            setTalkAction("WalkPickel");
            break;
        }

        setNerve(GET_NERVE(Kinopio, KinopioNrvFar));
        break;
    case 0:
        mParam.setNoTurnAction("SpinWait1");
        break;
    case 1:
        mParam.setNoTurnAction("SpinWait2");
        break;
    case 2:
        mParam.setNoTurnAction("SpinWait3");
        break;
    case 3:
        _128 = false;
        MR::setDistanceToTalk(mMsgCtrl, 400.0f);
        break;
    case 5:
        _128 = false;
        mParam.setSingleAction("SwimWait");
        break;
    case 7:
        _128 = false;
        mParam.setNoTurnAction("Sleep");
        break;
    case 9:
        mParam.setMoveAction("KinopioGoodsWeapon", "KinopioGoodsWeaponTurn");
        mParam.setTalkAction("KinopioGoodsWeapon", "KinopioGoodsWeaponTurn");
        setTalkAction("KinopioGoodsWeaponWalk");
        break;
    case 10:
        mParam.setSingleAction("Joy");
        break;
    case 11:
        mParam.setNoTurnAction("Rightened");
        break;
    case 12: {
        mParam.setSingleAction("StarPieceWait");
        const char* pWaitAction = "StarPieceWait";
        _11C = "KinopioGoodsStarPieceWalk";
        _120 = pWaitAction;
        break;
    }
    case 13:
        setTalkAction("Getaway");
        _10C = ::sRunSpeed;
        break;
    case 8:
        mIsSpawnPowerStarMode = true;
        break;
    case 6:
        mParam.setNoTurnAction("Pickel");
        MR::validateHitSensor(this, "Pickel");
        break;
    case 4:
        _17C = new MapObjConnector(this);
        MR::invalidateClipping(this);
        mParam._0 = false;
        mParam._1 = false;
        MR::onBind(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
        setNerve(GET_NERVE(Kinopio, KinopioNrvMount));
        break;
    }

    if (mIsSpawnPowerStarMode) {
        mTakeOutStar = new TakeOutStar(this, "TakeOutStar", "TakeOutStar", GET_NERVE(Kinopio, KinopioNrvTakeOutStar));
        MR::declarePowerStar(this);
        setNerve(GET_NERVE(Kinopio, KinopioNrvStarWait));
    }

    MR::useStageSwitchSleep(this, rIter);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        if (mBehavior != 4) {
            if (mAppearBehavior == 0) {
                pushNerve(GET_NERVE(Kinopio, KinopioNrvEscape));
            } else {
                pushNerve(GET_NERVE(Kinopio, KinopioNrvAppear));
            }
        }

        makeActorDead();
    } else {
        makeActorAppeared();
    }

    MR::startBrk(this, "ColorChange");
    MR::setBrkFrameAndStop(this, mObjArg0);
}

void Kinopio::exeReaction() {
    if (mBehavior == 7) {
        if (_D8) {
            MR::startSound(this, "SE_SM_NPC_TRAMPLED");
            MR::startSound(this, "SE_SV_KINOPIO_POINTING_SLEEP");
        }

        if (isPointingSe()) {
            MR::startDPDHitSound();
            MR::startSound(this, "SE_SV_KINOPIO_POINTING_SLEEP");
        }

        if (_D9) {
            MR::startSound(this, "SE_SM_NPC_TRAMPLED");
            MR::startSound(this, "SE_SV_KINOPIO_POINTING_SLEEP");
        }

        if (_DB) {
            MR::startSound(this, "SE_SM_NPC_TRAMPLED");
            MR::startSound(this, "SE_SV_KINOPIO_POINTING_SLEEP");
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
    return !MR::isNearPlayer(this, ::sDistancePlayerIsFar);
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

    if (MR::tryStartReactionAndPushNerve(this, GET_NERVE(Kinopio, KinopioNrvReaction))) {
        return;
    }

    if (MR::tryTalkNearPlayerAtEnd(mMsgCtrl)) {
        MR::startAction(this, "Notice");
    }

    if (MR::isActionContinuous(this)) {
        if (!isNerve(GET_NERVE(Kinopio, KinopioNrvNear))) {
            return;
        }

        if (MR::isActionStart(this, "Notice")) {
            turnToPlayer(mParam._8);
        }

        return;
    }

    if (isNerve(GET_NERVE(Kinopio, KinopioNrvFar))) {
        if (MR::isNearPlayer(this, ::sDistancePlayerIsNear)) {
            MR::startAction(this, "Notice");
            setNerve(GET_NERVE(Kinopio, KinopioNrvNear));
            return;
        }

        MR::tryStartMoveTalkAction(this);
    } else {
        if (isPlayerNotNearKinopio()) {
            setNerve(GET_NERVE(Kinopio, KinopioNrvFar));
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

    if (!(MR::tryStartReactionAndPushNerve(this, GET_NERVE(Kinopio, KinopioNrvReaction)) || MR::tryStartMoveTalkAction(this))) {
        if (isNerve(GET_NERVE(Kinopio, KinopioNrvStop0))) {
            if (MR::isGreaterEqualStep(this, 60)) {
                setDefaultNerve();
            }
        } else if (MR::isGreaterEqualStep(this, 120)) {
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
            setNerve(GET_NERVE(Kinopio, KinopioNrvAction));
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

    if (!MR::tryStartReactionAndPushNerve(this, GET_NERVE(Kinopio, KinopioNrvReaction))) {
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
    fallSpeed.scale(::sFallGravity);
    mVelocity.add(fallSpeed);
    mVelocity.mult(::sFallFrictionRate);

    faceToPlayer(::sTurnLimit);

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

    if (!MR::tryStartReactionAndPushNerve(this, GET_NERVE(Kinopio, KinopioNrvReaction))) {
        if (MR::tryTalkNearPlayerAndStartTalkAction(this)) {
            faceToPlayer(::sTurnLimit);
        } else {
            faceToDefault(::sTurnLimit);
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
    if (MR::isNearPlayer(mMsgCtrl, ::sDistanceEventPlayerIsNear)) {
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
    fallSpeed.scale(::sFallGravity);
    mVelocity.add(fallSpeed);
    mVelocity.mult(::sFallFrictionRate);

    faceToPlayer(::sTurnLimit);

    if (!tryCancelEvent() && MR::isBindedGround(this) && MR::isGreaterStep(this, ::sLandingTime)) {
        mVelocity.zero();
        MR::validateClipping(this);
        MR::turnQuatYDirRate(&_A0, _A0, -mGravity, ::sUpVecBlendRate);
        setInitPose();
        MR::tryForwardNode(mMsgCtrl);
        setNerve(GET_NERVE(Kinopio, KinopioNrvWait));
    }
}

void Kinopio::exeEvent() {
}

void Kinopio::exeTakeOutStar() {
}

void Kinopio::calcAndSetBaseMtx() {
    if (isNerve(GET_NERVE(Kinopio, KinopioNrvEvent)) || isNerve(GET_NERVE(Kinopio, KinopioNrvEventAndTalk)) ||
        isNerve(GET_NERVE(Kinopio, KinopioNrvEventTalking))) {
        return;
    }

    if (_17C != nullptr) {
        _17C->connect();
        TPos3f mtx(getBaseMtx());
        mtx.getQuat(_16C);

        TQuat4f rotation;
        rotation.mult(_16C, _A0);
        rotation.normalize();
        MR::setBaseTRMtx(this, rotation);
    } else {
        NPCActor::calcAndSetBaseMtx();
    }
}

bool Kinopio::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensor(pReceiver, "Pickel")) {
        return false;
    }

    return NPCActor::receiveMsgPlayerAttack(msg, pSender, pReceiver);
}

bool Kinopio::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_NPC_EVENT_START) {
        MR::offRootNodeAutomatic(mMsgCtrl);
        MR::onCalcAnim(this);
        setNerve(GET_NERVE(Kinopio, KinopioNrvEvent));
        return true;
    }

    if (msg == ACTMES_NPC_EVENT_END) {
        setNerve(GET_NERVE(Kinopio, KinopioNrvEventFall));
        return true;
    }

    if (msg == ACTMES_NPC_EVENT_TALK_ENABLE) {
        if (!isNerve(GET_NERVE(Kinopio, KinopioNrvEvent))) {
            return false;
        }

        setNerve(GET_NERVE(Kinopio, KinopioNrvEventAndTalk));
        return true;
    }

    if (msg == ACTMES_NPC_EVENT_TALK_DISABLE) {
        if (!(isNerve(GET_NERVE(Kinopio, KinopioNrvEventAndTalk)) || isNerve(GET_NERVE(Kinopio, KinopioNrvEventTalking)))) {
            return false;
        }

        setNerve(GET_NERVE(Kinopio, KinopioNrvEvent));
        return true;
    }

    if (msg == ACTMES_NPC_TALK_NEXT) {
        MR::tryForwardNode(mMsgCtrl);
        return true;
    }

    return false;
}

void Kinopio::faceToVector(const TVec3f& rDirection, f32 turnLimit) {
    TVec3f direction(rDirection);
    TVec3f up(-mGravity);
    TVec3f currentUp;
    TVec3f currentFront;
    TQuat4f inverse;
    f32 length = _16C.squared();

    if (length <= JGeometry::TUtil< f32 >::epsilon()) {
        inverse.set< f32 >(0.0f, 0.0f, 0.0f, 1.0f);
    } else {
        inverse.x = -_16C.x / length;
        inverse.y = -_16C.y / length;
        inverse.z = -_16C.z / length;
        inverse.w = _16C.w / length;
    }

    _A0.getYDir(currentUp);
    _A0.getZDir(currentFront);
    MR::normalize(&direction);
    inverse.transform(up);
    inverse.transform(direction);
    MR::turnVecToVecCosOnPlane(&currentFront, direction, currentUp, turnLimit);
    MR::blendQuatUpFront(&_A0, up, currentFront, ::sUpVecBlendRate, 1.0f);
}

void Kinopio::faceToPlayer(f32 turnLimit) {
    TVec3f direction(*MR::getPlayerPos());
    direction.sub(mPosition);
    faceToVector(direction, turnLimit);
}

void Kinopio::faceToDefault(f32 turnLimit) {
    TVec3f defaultUp;
    TVec3f defaultFront;
    TVec3f front;
    _B0.getYDir(defaultUp);
    _B0.getZDir(defaultFront);
    _A0.getZDir(front);
    MR::turnVecToVecCosOnPlane(&front, defaultFront, defaultUp, turnLimit);
    MR::blendQuatUpFront(&_A0, defaultUp, front, ::sUpVecBlendRate, 1.0f);
}

void Kinopio::setDefaultNerve() {
    if (mBehavior == 13) {
        _10C = ::sRunSpeed;
    } else {
        _10C = ::sWalkSpeed;
    }

    if (mBehavior == -1) {
        if (MR::isNearPlayer(this, ::sDistancePlayerIsNear)) {
            setNerve(GET_NERVE(Kinopio, KinopioNrvNear));
        } else {
            setNerve(GET_NERVE(Kinopio, KinopioNrvFar));
        }
    } else {
        setNerve(GET_NERVE(Kinopio, KinopioNrvWait));
    }
}

bool Kinopio::tryStartArgs() {
    s32 pointArg0;
    MR::getCurrentRailPointArg0WithInit(this, &pointArg0);
    switch (pointArg0) {
    case 0:
        setNerve(GET_NERVE(Kinopio, KinopioNrvStop0));
        break;
    case 1:
        setNerve(GET_NERVE(Kinopio, KinopioNrvStop1));
        break;
    case 2:
        MR::startAction(this, "Notice");
        setNerve(GET_NERVE(Kinopio, KinopioNrvAction));
        break;
    case 3:
        MR::startAction(this, "Tumble");
        setNerve(GET_NERVE(Kinopio, KinopioNrvAction));
        break;
    default:
        return false;
    }

    return true;
}

bool Kinopio::tryCancelEvent() {
    if (MR::isGreaterStep(this, ::sTimeToCancelEvent)) {
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
