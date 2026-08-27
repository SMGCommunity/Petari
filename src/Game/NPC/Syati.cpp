#include "Game/NPC/Syati.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/PrizeRing.hpp"
#include "Game/Util.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"

namespace {
    const char* cBckForRipple[5] = {"WaitReverse", "Talk", "StartWait", "StartTalk", "Delight"};

    const char* cBckForTurn[5] = {"Turn", "TurnReverse"};

    const char* cBckForTurnSplash[5] = {"Turn", "TurnReverse"};

    static const Vec sMarioMoveLocalOffsetRing = {6.0f, -33.0f, -776.0f};
    static const Vec sMarioMoveLocalOffsetDeepSea = {6.0, -176.0, -590.0};

    static const Vec sTalkOffset = {16.0f, 192.0f, -300.0f};
    static const Vec sTalkOffsetDeepSea = {47.0f, 179.0f, -300.0f};

    static const Vec sTalkOffsetOnShore = {61.0f, 384.0f, 250.0f};
    static const Vec sTalkOffsetDelightDeepSea = {152.0f, 240.0f, 250.0f};
};  // namespace

f32 JMAAcosRadian_dummy(f32 f) {
    TVec3f _dummy;
    _dummy.set(sMarioMoveLocalOffsetDeepSea);
    _dummy.add(0.0f);
    return JMAAcosRadian(f);
}

Syati::Syati(const char* pName) : LiveActor(pName) {
    _A8 = 0.0f;
    _AC.x = 0.0f;
    _AC.y = 0.0f;
    _AC.z = 0.0f;
    _B8 = 0;
    _BC = 0;
    _C0 = -1;
    _C4 = 0;
    _C8 = 0xF0;
    _108 = 0;
    _13C = 0;
    _140 = 0;
    _144 = 5;
    _148 = -1;
    _14C = -1;
    _150 = -1;
    _154 = 0;
    _158 = -1;

    _8C.set(0.0f, 0.0f, 0.0f, 1.0f);
    _CC.zero();
    _D8.identity();
    _140 = new LiveActorGroup("リング保持", 5);
}

void Syati::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::useStageSwitchReadA(this, rIter);
    MR::getJMapInfoArg2NoInit(rIter, &_14C);
    initPose();
    initModelManagerWithAnm("Syati", NULL, false);
    _108 = MR::createJointDelegatorWithNullChildFunc(this, &Syati::calcHeadJoint, "Head");
    const char* _8 = "";
    MR::getObjectName(&_8, rIter);
    if (MR::isEqualString(_8, "Syati")) {
        initRings(rIter);
        MR::declarePowerStar(this);
        initRailRider(rIter);
        MR::moveCoordAndTransToRailPoint(this, 0);
        _13C = new RaceManagerLayout("レース管理用レイアウト");
        _13C->initWithoutIter();
    }
    initTalking(rIter);
    MR::connectToSceneNoSilhouettedMapObj(this);
    initHitSensor(3);
    MR::addHitSensorAtJointEnemySimple(this, "head", "jaw", 4, 200.0f, TVec3f(0.0f, -60.0f, 0.0f));
    MR::addHitSensorEnemySimple(this, "body", 4, 230.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointEnemySimple(this, "tail", "spin2", 4, 150.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::calcModelBoundingRadius(&_A8, this);
    MR::setClippingTypeSphere(this, _A8);
    MR::validateClipping(this);
    initEffectKeeper(1, 0, 0);

    if (_14C) {
        MR::initEffectSyncBck(this, "Ripple", ::cBckForRipple);
        MR::initEffectSyncBck(this, "TurnSplash", ::cBckForTurnSplash);
    } else
        MR::initEffectSyncBck(this, "Turn", ::cBckForTurn);

    MR::initShadowVolumeFlatModel(this, "ShadowVolumeOval");
    MR::setShadowDropLength(this, NULL, 2000.0f);
    MR::onCalcShadowOneTime(this, NULL);
    MR::calcGravity(this);
    initSound(8, false);

    if (MR::isExistRail(this))
        initNerve(&NrvSyati::SyatiWait::sInstance);
    else
        initNerve(&NrvSyati::SyatiHideOnShore::sInstance);

    makeActorAppeared();
}

void Syati::exeWait() {
    if (MR::isFirstStep(this)) {
        const char* pStr;

        if (_14C) {
            pStr = "WaitReverse";
        } else
            pStr = "WaitDeepSea";

        MR::startBck(this, pStr, NULL);
        MR::startBtk(this, pStr);
        MR::startBva(this, "Open");
    }

    if (MR::isOnSwitchA(this))
        MR::requestStartDemoMarioPuppetable(this, "開始デモ", &NrvSyati::SyatiFadeoutStartEvent::sInstance, &NrvSyati::SyatiWaitDemoStart::sInstance);
}

void Syati::exeFadeoutBeforeTalk() {
    if (MR::isFirstStep(this)) {
        MR::closeWipeFade(-1);
        return;
    }

    if (!MR::isWipeActive()) {
        if (isNerve(&NrvSyati::SyatiFadeoutStartEvent::sInstance))
            setNerve(&NrvSyati::SyatiWaitBlankStartEvent::sInstance);
        else
            setNerve(&NrvSyati::SyatiWaitBlankRetryEvent::sInstance);
    }
}

void Syati::exeWaitBlank() {
    if (MR::isFirstStep(this)) {
        _CC.set(_14C == 0 ? ::sMarioMoveLocalOffsetDeepSea : ::sMarioMoveLocalOffsetRing);
        _C4->_1C = 0;
        _C4->update();

        MR::tryPlayerKillTakingActor();
        MR::startBckPlayer("SwimWait", (const char*)nullptr);
        MR::makeQuatFromRotate(&_8C, this);
    }

    if (MR::isStep(this, 0x1E)) {
        if (isNerve(&NrvSyati::SyatiWaitBlankStartEvent::sInstance))
            setNerve(&NrvSyati::SyatiFadeinStartEvent::sInstance);
        else
            setNerve(&NrvSyati::SyatiFadeinRetryEvent::sInstance);
    }
}

void PlayerPoseSetterInWater::update() {
    // FIXME: pre-calulation for MR::cos happening but not table access
    // FIXME: out-of-order register transfer for result of offsetPos + offset
    // https://decomp.me/scratch/loE96

    _1C = (++_1C + 90) % 90;

    f32 f = MR::cos((_1C / 90.0f) * TWO_PI);

    TPos3f syatiMtx;
    syatiMtx.setQuat(_C);
    syatiMtx.setTrans(_0);

    TVec3f offsetPos;
    offsetPos.zero();
    if (_20 != nullptr) {
        offsetPos.set(*_20);
    }
    syatiMtx.mult(offsetPos, offsetPos);

    // Player model "up" is forward movement direction
    // Player model "front" is downwards to gravity
    TVec3f forward = _0;
    TVec3f syatiUp;
    syatiMtx.getYDir(syatiUp);
    syatiUp.scale(450.0f);
    forward.add(syatiUp);
    forward.sub(offsetPos);

    TVec3f offset;
    offset.zero();

    TPos3f baseMtx;
    MR::makeMtxUpFrontPos(&baseMtx, forward, *MR::getPlayerGravity(), offsetPos + offset);
    MR::setPlayerBaseMtx(baseMtx);
}

void Syati::exeFadeinBeforeTalk() {
    if (MR::isFirstStep(this)) {
        MR::openWipeFade(-1);
        MR::startMultiActorCameraTargetSelf(this, _BC, "会話", -1);
        MR::startBck(this, "Talk", nullptr);
        MR::startBtk(this, "Talk");

        if (isNerve(&NrvSyati::SyatiFadeinRetryEvent::sInstance) && MR::isEqualStageName("OceanPhantomCaveGalaxy") &&
            !MR::isPlayingStageBgmName("STM_GALAXY_05"))
            MR::stopStageBGM(0x3C);
    }
    _C4->update();

    if (!MR::isWipeActive()) {
        if (isNerve(&NrvSyati::SyatiFadeinStartEvent::sInstance))
            setNerve(&NrvSyati::SyatiTalkStartMission::sInstance);
        else
            setNerve(&NrvSyati::SyatiTalkRetryMission::sInstance);
    }
}

void Syati::exeTalkStartMission() {
    if (MR::isFirstStep(this)) {
        setupBalloonFollowMtx(TVec3f(_14C == 0 ? sTalkOffset : sTalkOffsetDeepSea));
    }

    updateBlink();
    _C4->update();

    if (MR::tryTalkForceWithoutDemoMarioPuppetableAtEnd(_B8)) {
        MR::endMultiActorCamera(this, _BC, "会話", false, -1);
        setNerve(&NrvSyati::SyatiReadyToStart::sInstance);
    }
}

void Syati::exeReadyToStart() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
        resetScore();
        MR::requestMovementOn(_13C);
        MR::startMultiActorCameraTargetSelf(this, _BC, "開始デモ", -1);
        MR::startBck(this, "TurnReverse", nullptr);
        MR::startBtk(this, "Normal");
        MR::startBva(this, "Open");
        MR::startSound(this, "SE_SM_SYATI_TURN", -1, -1);
        MR::startBckPlayer("SwimWait", (const char*)nullptr);
        MR::onCalcShadow(this, nullptr);
    }

    _C4->update();

    if (MR::isBckStopped(this))
        setNerve(&NrvSyati::SyatiCountDown::sInstance);
}

void Syati::exeCountDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Swim", nullptr);
        _13C->appear();
        _13C->playCountAndGo();
        _13C->hideRecordPane();
        _13C->hideBestRecordPane();
    }

    if (getNerveStep() % 0x3C == 0) {
        MR::startSystemSE("SE_SY_RACE_COUNT_DOWN", -1, -1);
    }

    MR::setBckRate(this, 0.5f);
    updateSwimCommon();
    _C4->update();

    if (!_13C->isPlayCountAnim()) {
        MR::endDemo(this, "開始デモ");
        MR::endMultiActorCamera(this, _BC, "開始デモ", true, -1);
        MR::startSystemSE("SE_SY_RACE_START", -1, -1);
        setNerve(&NrvSyati::SyatiSwim::sInstance);
    }
}

void Syati::exeSwim() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Swim", nullptr);
    }

    MR::setBckRate(this, 0.5);
    updateSwimCommon();

    if (isReadyToEmitRing()) {
        setNerve(&NrvSyati::SyatiEmitRing::sInstance);
    } else {
        if (MR::isRailReachedGoal(this)) {
            setNerve(&NrvSyati::SyatiReachToEnd::sInstance);
        }
    }
}

void Syati::exeEmitRing() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Screw", nullptr);
        MR::startSound(this, "SE_SM_SYATI_ROLL", -1, -1);
        emitRing();
    }

    updateSwimCommon();

    if (isReadyToEmitRing()) {
        setNerve(&NrvSyati::SyatiEmitRing::sInstance);
    } else {
        if (MR::isBckStopped(this)) {
            setNerve(&NrvSyati::SyatiSwim::sInstance);
        }
    }
}

void Syati::exeWaitStarAppeared() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
        MR::startBckWithInterpole(this, "Star", 0);
        MR::startBtk(this, "Normal");
        calcAnim();
        TVec3f stack_8;
        MR::copyJointPos(this, "Head", &stack_8);
        MR::requestAppearPowerStar(this, _148, stack_8);
    }

    if (MR::isGreaterStep(this, 1) && MR::isStageStatePowerStarAppeared()) {
        kill();
    }
}

void Syati::exeReachToEnd() {
    if (MR::isFirstStep(this))
        MR::startBck(this, "Turn", nullptr);

    if (MR::isBckStopped(this))
        setNerve(&NrvSyati::SyatiWaitAllRingDisappear::sInstance);
}

void Syati::exeWaitAllRingDisappear() {
    if (MR::isFirstStep(this)) {
        const char* pStr = _14C == 0 ? "WaitReverse" : "WaitDeepSea";

        MR::startBck(this, pStr, nullptr);
        MR::startBtk(this, pStr);
        MR::resetNode(_B8);
        MR::forwardNode(_B8);
    }

    if (_140->getLivingActorNum() == 0 && _154 != _144)
        MR::requestStartDemoMarioPuppetable(this, "再挑戦デモ", &NrvSyati::SyatiFadeoutRetryEvent::sInstance,
                                            &NrvSyati::SyatiWaitDemoStart::sInstance);
}

void Syati::exeTalkRetryMission() {
    if (MR::isFirstStep(this)) {
        setupBalloonFollowMtx(TVec3f(_14C == 0 ? ::sTalkOffsetDeepSea : ::sTalkOffset));
        MR::startBck(this, "Failure", nullptr);
        MR::startBtk(this, "Failure");

        if (MR::isEqualStageName("OceanPhantomCaveGalaxy") && !MR::isPlayingStageBgmName("STM_GALAXY_05"))
            MR::startStageBGM("MBGM_GALAXY_05", false);
    }

    _C4->update();
    updateBlink();

    if (MR::tryTalkForceWithoutDemoMarioPuppetableAtEnd(_B8)) {
        MR::endDemo(this, "再挑戦デモ");
        MR::endMultiActorCamera(this, _BC, "会話", true, -1);
        setNerve(&NrvSyati::SyatiForceKill::sInstance);
    }
}

void Syati::exeForceKill() {
    if (MR::isFirstStep(this))
        MR::forceKillPlayerByWaterRace();
}

void Syati::exeHideOnShore() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        MR::invalidateHitSensors(this);
        MR::setDistanceToTalk(_B8, 700.0f);

        const Vec* pVec = _14C == 0 ? &::sTalkOffsetDelightDeepSea : &::sTalkOffsetOnShore;

        setupBalloonFollowMtx(TVec3f(*pVec));
    }

    switch (_C0) {
    case 0:
        if (MR::isStageStatePowerStarAppeared())
            setNerve(&NrvSyati::SyatiWaitOnShore::sInstance);
        break;
    case 1:
        setNerve(&NrvSyati::SyatiWaitTalkNormal::sInstance);
        break;
    }
}

void Syati::exeWaitOnShore() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::validateHitSensors(this);
        const char* pStr = _14C == 0 ? "Delight" : "DelightDeepSea";

        MR::startBck(this, pStr, nullptr);
        MR::startBtk(this, pStr);
        MR::startBva(this, "Open");
        MR::onCalcShadowOneTime(this, nullptr);
    }

    updateBlink();
    MR::tryTalkNearPlayer(_B8);
}

void Syati::exeWaitTalkNormal() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::validateHitSensors(this);
        MR::startBckWithInterpole(this, "StartWait", 0xF);
        MR::startBtk(this, "StartWait");
        MR::startBva(this, "Open");
    }

    updateBlink();
    MR::tryTalkNearPlayer(_B8);

    if (MR::isNearPlayer(_B8, -1.0f))
        setNerve(&NrvSyati::SyatiTalkNormal::sInstance);
}

void Syati::exeTalkNormal() {
    if (MR::isFirstStep(this)) {
        MR::startBckWithInterpole(this, "StartTalk", 0xF);
        MR::startBtk(this, "StartTalk");
    }

    updateBlink();
    MR::tryTalkNearPlayer(_B8);

    if (!MR::isNearPlayer(_B8, -1.0f))
        setNerve(&NrvSyati::SyatiWaitTalkNormal::sInstance);
}

void Syati::initRings(const JMapInfoIter& rIter) {
    MR::getJMapInfoArg0NoInit(rIter, &_144);
    MR::getJMapInfoArg3NoInit(rIter, &_150);

    for (int i = 0; i < 5; i++) {
        PrizeRing* pNewRing = new PrizeRing();
        pNewRing->initWithoutIter();
        _140->registerActor(pNewRing);
    }
}

PlayerPoseSetterInWater::PlayerPoseSetterInWater(const TVec3f& rVec, Syati* pSyati) : _0(rVec), _C(pSyati->_8C), _1C(0), _20(&pSyati->_CC) {
}

void Syati::initPose() {
    MR::makeQuatFromRotate(&_8C, this);
    _C4 = new PlayerPoseSetterInWater(mPosition, this);
}

void Syati::initTalking(const JMapInfoIter& rIter) {
    _B8 = MR::createTalkCtrl(this, rIter, "SyatiRing", TVec3f(0.0f, 0.0f, 0.0f), _D8);
    MR::setDistanceToTalk(_B8, 2500.0f);
    _BC = MR::createActorCameraInfo(rIter);
    MR::initMultiActorCamera(this, rIter, &_BC, "会話");
    MR::initMultiActorCamera(this, rIter, &_BC, "開始デモ");
    MR::getJMapInfoArg1NoInit(rIter, &_C0);
    const char* stack_8 = "\0";
    MR::getObjectName(&stack_8, rIter);

    if (!MR::isEqualString(stack_8, "Syati"))
        MR::onRootNodeAutomatic(_B8);
}

void Syati::updateSwimCommon() {
    s32 stack_8 = -1;
    MR::getRailPointArg1NoInit(this, MR::getCurrentRailPointNo(this), &stack_8);

    if (5000.0f <= MR::calcDistanceToPlayer(mPosition))
        stack_8 = 3;

    MR::moveCoordAndFollowTrans(this, (f32)stack_8);
    updatePoseByRail();

    if (!MR::isDead(_13C) && !_13C->isPlayCountAndGoAnim()) {
        _13C->kill();
    }
    MR::startLevelSound(this, "SE_SM_LV_SYATI_SWIM", -1, -1);
}

void Syati::updatePoseByRail() {
    TVec3f stack_64;
    MR::calcRailPosAtCoord(&stack_64, this, 600.0f + MR::getRailCoord(this));
    TVec3f stack_58;
    MR::calcRailPosAtCoord(&stack_58, this, MR::getRailCoord(this));
    stack_64.sub(stack_58);

    TVec3f stack_4C(MR::getRailDirection(this));
    MR::vecKillElement(stack_4C, mGravity, &stack_4C);
    MR::normalizeOrZero(&stack_4C);
    TVec3f stack_40(stack_64);
    MR::vecKillElement(stack_40, mGravity, &stack_40);

    if (!MR::isNearZero(stack_40))
        MR::normalize(&stack_40);
    else
        stack_40.set(stack_4C);

    MR::vecBlend(_9C, stack_40, &_9C, 0.04f);
    TVec3f stack_34 = stack_4C.cross(_9C);
    MR::normalizeOrZero(&stack_34);
    TQuat4f stack_24;
    stack_24.set(0.0f, 0.0f, 0.0f, 1.0f);
    TVec3f stack_18 = -mGravity;
    f32 dot = stack_4C.dot(_9C);
    f32 rad = MR::acos(dot);
    if (dot < 1.0f) {
        f32 dot2 = stack_34.dot(mGravity);
        if (dot2 < 0.0f) {
            stack_24.setEulerZ(-1.0f * rad);
        } else {
            if (0.0f < dot2) {
                stack_24.setEulerZ(rad);
            }
        }
    }

    if (!MR::isSameDirection(stack_64, stack_18)) {
        TQuat4f stack_8;
        MR::makeQuatFromVec(&stack_8, stack_64, stack_18);
        stack_8.mult(stack_8, stack_24);
        _8C.slerp(stack_8, 0.04);
    }
}

void Syati::updateNumRingPassed() {
    bool b = 0;
    for (int i = 0; i < _140->mObjectCount; i++) {
        PrizeRing* pPrizeRing = (PrizeRing*)_140->getActor(i);

        if (MR::isDead(pPrizeRing))
            continue;

        if (pPrizeRing->isReadyToPass()) {
            pPrizeRing->setNervePass();
            b = true;
            _154++;
            MR::incPlayerLife(1);
        }

        if (pPrizeRing->isReadyToKill()) {
            pPrizeRing->kill();
        } else
            continue;

        if (_154 == _144) {
            killAllRings();
            setNerve(&NrvSyati::SyatiWaitStarAppeared::sInstance);
        }
    }

    if (b && _154 != _144)
        syncNumRingLeftToActiveRings();
}

void Syati::updateBlink() {
    if ((_C8 -= 1) < 0) {
        MR::startBva(this, "Blink");
        s16 frame = MR::getBvaCtrl(this)->mEnd;
        u32 rand = MR::getRandom((s32)0x78, (s32)0xF0);
        _C8 = frame + rand;
    } else if (MR::isBvaStopped(this)) {
        MR::startBva(this, "Open");
    }
}

bool Syati::isReadyToEmitRing() const {
    if (_154 == _144)
        return false;

    if (!_140->getDeadActor())
        return false;

    if (MR::getCurrentRailPointNo(this) == _158)
        return false;

    f32 dist = 0.0f;
    MR::calcDistanceToNextRailPoint(this, &dist);

    if (600.0f < dist) {
        return false;
    }

    s32 arg = -1;
    s32 point = MR::getNextRailPointNo(this);
    MR::getRailPointArg0NoInit(this, point, &arg);
    return (bool)!arg;
}

void Syati::syncNumRingLeftToActiveRings() {
    PrizeRing* pPrizeRing;

    for (int i = 0; i < 5; i++) {
        pPrizeRing = (PrizeRing*)_140->getActor(i);
        if (MR::isDead(pPrizeRing))
            continue;

        if (pPrizeRing->isPassed())
            continue;

        pPrizeRing->setNumber(_144 - _154);
    }
}

void Syati::resetScore() {
    _154 = 0;
    _158 = -1;
    MR::resetNode(_B8);
    killAllRings();
    MR::moveCoordToRailPoint(this, 0);
}

void Syati::killAllRings() {
    for (int i = 0; i < 5; i++) {
        PrizeRing* pPrizeRing = (PrizeRing*)_140->getActor(i);

        if (!MR::isDead(pPrizeRing))
            pPrizeRing->kill();
    }
}

void Syati::emitRing() {
    PrizeRing* pPrizeRing = (PrizeRing*)_140->getDeadActor();
    TVec3f stack_20;
    f32 coord = MR::getRailCoord(this);
    coord += 600.0f;
    MR::calcRailPosAtCoord(&stack_20, this, coord);
    pPrizeRing->mPosition.set(stack_20);
    TVec3f stack_14;
    MR::calcRailDirectionAtCoord(&stack_14, this, coord);
    TPos3f stack_2C;
    MR::calcMtxFromGravityAndZAxis(&stack_2C, this, mGravity, stack_14);

    stack_2C.getEulerDegree(pPrizeRing->mRotation);
    pPrizeRing->appear();
    pPrizeRing->setNumber(_144 - _154);

    if (_150 > 0)
        pPrizeRing->setLife(_150);

    _158 = MR::getCurrentRailPointNo(this);
}

void Syati::setupBalloonFollowMtx(const TVec3f& rVec) {
    _D8.setQuat(_8C);
    _D8.setTrans(mPosition);
    TVec3f stack_8(rVec);
    _D8.mult(stack_8, stack_8);
    _D8.setTrans(stack_8);
}

bool Syati::calcHeadJoint(TPos3f* pPos, const JointControllerInfo& rInfo) {
    bool nrv = false;
    nrv = isNerve(&NrvSyati::SyatiWaitOnShore::sInstance) || isNerve(&NrvSyati::SyatiWaitTalkNormal::sInstance) ||
          isNerve(&NrvSyati::SyatiTalkNormal::sInstance);

    if (!nrv)
        return false;

    TPos3f stack_3C;
    TVec3f stack_30(*MR::getPlayerPos());
    stack_30.sub(mPosition);
    MR::normalizeOrZero(&stack_30);

    if (MR::isNearZero(stack_30, 0.001f))
        return false;

    TVec3f stack_24(stack_30);
    stack_24.y = 0.0f;
    MR::normalizeOrZero(&stack_24);

    if (MR::isNearZero(stack_24, 0.001f))
        return false;

    TVec3f stack_18;
    _8C.getZDir(stack_18);
    MR::normalize(&stack_18);
    TQuat4f stack_8;
    stack_8.set(0.0f, 0.0f, 0.0f, 1.0f);
    MR::turnQuat(&stack_8, stack_8, stack_18, stack_24, 0.52359879f);
    stack_3C.setQuat(stack_8);
    stack_3C.zeroTrans();
    pPos->concat(*pPos, stack_3C);
    return true;
}

void Syati::control() {
    MR::setClippingRangeIncludeShadow(this, &_AC, _A8);
    updateNumRingPassed();
}

void Syati::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _8C);
    _108->registerCallBack();
}

void Syati::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (MR::sendMsgEnemyAttackFlipMaximum(pReceiver, pSender)) {
            MR::startSound(this, "SE_SM_SYATI_TRAMPLED", -1, -1);
        }
    } else
        MR::sendMsgPush(pReceiver, pSender);
}

bool Syati::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgJetTurtleAttack(msg) || MR::isMsgStarPieceReflect(msg))
        return true;

    return false;
}

Syati::~Syati() {
}

namespace NrvSyati {
    SyatiWait(SyatiWait::sInstance);
    SyatiFadeoutStartEvent(SyatiFadeoutStartEvent::sInstance);
    SyatiWaitBlankStartEvent(SyatiWaitBlankStartEvent::sInstance);
    SyatiFadeinStartEvent(SyatiFadeinStartEvent::sInstance);
    SyatiTalkStartMission(SyatiTalkStartMission::sInstance);
    SyatiReadyToStart(SyatiReadyToStart::sInstance);
    SyatiCountDown(SyatiCountDown::sInstance);
    SyatiSwim(SyatiSwim::sInstance);
    SyatiEmitRing(SyatiEmitRing::sInstance);
    SyatiWaitStarAppeared(SyatiWaitStarAppeared::sInstance);
    SyatiReachToEnd(SyatiReachToEnd::sInstance);
    SyatiWaitAllRingDisappear(SyatiWaitAllRingDisappear::sInstance);
    SyatiFadeoutRetryEvent(SyatiFadeoutRetryEvent::sInstance);
    SyatiWaitBlankRetryEvent(SyatiWaitBlankRetryEvent::sInstance);
    SyatiFadeinRetryEvent(SyatiFadeinRetryEvent::sInstance);
    SyatiTalkRetryMission(SyatiTalkRetryMission::sInstance);
    SyatiForceKill(SyatiForceKill::sInstance);
    SyatiHideOnShore(SyatiHideOnShore::sInstance);
    SyatiWaitOnShore(SyatiWaitOnShore::sInstance);
    SyatiWaitTalkNormal(SyatiWaitTalkNormal::sInstance);
    SyatiTalkNormal(SyatiTalkNormal::sInstance);
    SyatiWaitDemoStart(SyatiWaitDemoStart::sInstance);
};  // namespace NrvSyati
