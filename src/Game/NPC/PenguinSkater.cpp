#include "Game/NPC/PenguinSkater.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/NPC/TalkDirector.hpp"

namespace {
    static const f32 sProvokeRangeIn = 1000.0f;
    static const f32 sProvokeRangeOut = 2000.0f;
    static const f32 sSwitchRange = 3000.0f;
    static const f32 sAwaySpeed = 14.0f;
    static const f32 sSwitchSpeed = 10.0f;
    static const f32 sDemoSpeed = 18.0f;
    // static const f32 sAwayAccele = ;
    // static const f32 sAwayBrake = ;
    // static const f32 sBlendRatio = ;
    // static const f32 sSameDirection = ;
    // static const f32 sSamePosition = ;
    static const s32 sMarioPoseBlendTime = 5;
    static const s32 sDemoSkateSeStartTiming = 33;
}  // namespace

namespace NrvPenguinSkater {
    NEW_NERVE(PenguinSkaterNrvReaction, PenguinSkater, Reaction);
    NEW_NERVE(PenguinSkaterNrvTalk, PenguinSkater, Talk);
    NEW_NERVE(PenguinSkaterNrvWait, PenguinSkater, Wait);
    NEW_NERVE(PenguinSkaterNrvDemo, PenguinSkater, Demo);
    NEW_NERVE(PenguinSkaterNrvAway, PenguinSkater, Away);
    NEW_NERVE(PenguinSkaterNrvTurn, PenguinSkater, Turn);
    NEW_NERVE(PenguinSkaterNrvProvoke, PenguinSkater, Provoke);
    NEW_NERVE(PenguinSkaterNrvCaught, PenguinSkater, Caught);
    NEW_NERVE(PenguinSkaterNrvFadeOut, PenguinSkater, FadeOut);
    NEW_NERVE(PenguinSkaterNrvFadeIn, PenguinSkater, FadeIn);
    NEW_NERVE(PenguinSkaterNrvGiveUp, PenguinSkater, GiveUp);
    NEW_NERVE(PenguinSkaterNrvSitDown, PenguinSkater, SitDown);
    NEW_NERVE(PenguinSkaterNrvSwitch, PenguinSkater, Switch);
};  // namespace NrvPenguinSkater

PenguinSkater::PenguinSkater(const char* pName) : NPCActor(pName), mCameraInfo(0), _160(0), _164(0), mRail(0), _1AC(0) {
    _1B4.set< f32 >(0.0f, 0.0f, 0.0f, 1.0f);
    _1C4.set(0.0f, 0.0f, 0.0f);
}

void PenguinSkater::init(const JMapInfoIter& rIter) {
    NPCActorCaps caps("PenguinSkater");
    caps.setDefault();
    caps.mWaitNerve = &NrvPenguinSkater::PenguinSkaterNrvWait::sInstance;
    caps.mObjectName = "Penguin";
    if (MR::isConnectedWithRail(rIter)) {
        caps.mRailRider = true;
        MR::initMultiActorCamera(this, rIter, &mCameraInfo, "開始");
        MR::initMultiActorCamera(this, rIter, &mCameraInfo, "終了");
        MR::joinToGroupArray(this, rIter, nullptr, 32);
    } else {
        caps.mWaitNerve = &NrvPenguinSkater::PenguinSkaterNrvTalk::sInstance;
    }
    initialize(rIter, caps);
    setDefaults();
    mParam._14 = "Wait";
    mParam._18 = "Turn";
    mParam._1C = "TalkTurn";
    mParam._20 = "Talk";
    MR::startBrk(this, "ColorChange");
    MR::setBrkFrameAndStop(this, 0.0f);
    MR::offRootNodeAutomatic(mMsgCtrl);
    MR::useStageSwitchWriteA(this, rIter);
}

void PenguinSkater::initAfterPlacement() {
    if (MR::isExistRail(this)) {
        LiveActorGroup* grp = MR::getGroupFromArray(this);
        for (s32 i = 0; i < grp->mObjectCount; i++) {
            LiveActor* actor = grp->getActor(i);
            if (MR::isLoopRail(actor)) {
                if (actor == this) {
                    _160 = actor;
                } else {
                    _164 = actor;
                }
            } else {
                _1AC++;
            }
        }
        mRail = _160;
        MR::followRailPoseOnGround(this, _160, 1.0f);
    }
}

bool PenguinSkater::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerSpinAttack(msg)) {
        bool isAttackable = false;
        if (isNerve(&NrvPenguinSkater::PenguinSkaterNrvAway::sInstance) || isNerve(&NrvPenguinSkater::PenguinSkaterNrvTurn::sInstance) ||
            isNerve(&NrvPenguinSkater::PenguinSkaterNrvSwitch::sInstance)) {
            isAttackable = true;
        }
        if (isAttackable) {
            MR::tryRumblePadStrong(this, WPAD_CHAN0);
            setNerve(&NrvPenguinSkater::PenguinSkaterNrvCaught::sInstance);
        }
    }

    return NPCActor::receiveMsgPlayerAttack(msg, pSender, pReceiver);
}

void PenguinSkater::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        bool isAttackable = false;
        if (isNerve(&NrvPenguinSkater::PenguinSkaterNrvAway::sInstance) || isNerve(&NrvPenguinSkater::PenguinSkaterNrvTurn::sInstance) ||
            isNerve(&NrvPenguinSkater::PenguinSkaterNrvSwitch::sInstance)) {
            isAttackable = true;
        }
        if (isAttackable) {
            MR::tryRumblePadStrong(this, WPAD_CHAN0);
            setNerve(&NrvPenguinSkater::PenguinSkaterNrvCaught::sInstance);
        }
    }
    NPCActor::attackSensor(pSender, pReceiver);
}

void PenguinSkater::moveRail(f32 speed, f32 p2) {
    MR::adjustmentRailCoordSpeed(mRail, speed, 0.15f);
    _1B0 = MR::moveCoordAndFollowTransAndCheckPassPointNo(mRail);
    MR::followRailPoseOnGround(this, mRail, p2);
}

void PenguinSkater::stopRail(f32 p1) {
    MR::adjustmentRailCoordSpeed(mRail, 0.0f, 0.2f);
    _1B0 = MR::moveCoordAndFollowTransAndCheckPassPointNo(mRail);
    MR::followRailPoseOnGround(this, mRail, p1);
}

bool PenguinSkater::inProvokeRangeIn(f32 dist) const {
    f32 absDist = __fabsf(dist);
    return (__fabsf((MR::getRailTotalLength(mRail) * 0.5f) - absDist) < sProvokeRangeIn);  // FIXME: float regswap
}

bool PenguinSkater::inProvokeRangeOut(f32 dist) const {
    f32 absDist = __fabsf(dist);
    return (__fabsf((MR::getRailTotalLength(mRail) * 0.5f) - absDist) < sProvokeRangeOut);  // FIXME: float regswap
}

bool PenguinSkater::inSwitchRange(f32 dist) const {
    f32 absDist = __fabsf(dist);
    if (absDist > sSwitchRange) {
        return false;
    }
    return _1B0 + 1;
}

void PenguinSkater::endSwitchRail() {
    f32 speed = MR::getRailCoordSpeed(mRail);

    LiveActor* actor = _160;
    mRail = _164;
    _160 = _164;
    _164 = actor;
    MR::setRailCoordSpeed(_160, speed);
    MR::moveCoordToNearestPos(mRail, mPosition);
}

void PenguinSkater::setCaughtStartMarioPose() {
    TPos3f baseMtx;
    baseMtx.set(MR::getPlayerDemoActor()->getBaseMtx());
    baseMtx.getQuat(_1B4);
    baseMtx.getTransInline(_1C4);
}

void PenguinSkater::blendBaseMatrixToMario(f32 blendRate) const {
    TMtx34f baseMtx;
    if (blendRate >= 1.0f) {
        baseMtx.set(getBaseMtx());
        MR::setPlayerBaseMtx(baseMtx);
    } else {
        baseMtx.identity();
        MR::blendMtx(MR::getPlayerBaseMtx(), getBaseMtx(), blendRate, baseMtx);
        MR::setPlayerBaseMtx(baseMtx);
    }
}

void PenguinSkater::exeReaction() {
    if (_D8) {
        MR::startSound(this, "SE_SM_PENGUIN_TRAMPLED", -1, -1);
        MR::startSound(this, "SE_SV_PENGUIN_S_TRAMPLED", -1, -1);
    }
    if (isPointingSe()) {
        MR::startSound(this, "SE_SV_PENGUIN_S_POINTED_2P", -1, -1);
        MR::startDPDHitSound();
    }
    if (_D9) {
        MR::startSound(this, "SE_SV_PENGUIN_S_SPIN_HIT", -1, -1);
    }
    if (_DB) {
        MR::startSound(this, "SE_SV_PENGUIN_S_STAR_PIECE", -1, -1);
    }
    if (MR::tryStartReactionAndPopNerve(this)) {
        return;
    }
}

void PenguinSkater::exeWait() {
    if (MR::tryStartReaction(this)) {
        pushNerve(&NrvPenguinSkater::PenguinSkaterNrvReaction::sInstance);
    } else if (MR::tryTalkNearPlayerAtEndAndStartTalkAction(this)) {
        MR::invalidateClipping(this);
        MR::tryStartDemo(this, "ペンギンスケート開始");
        MR::startMultiActorCameraTargetSelf(this, mCameraInfo, "開始", -1);
        MR::forwardNode(mMsgCtrl);
        TVec3f pos;
        MR::calcRailStartPointDirection(&pos, mRail);
        TVec3f* playerPos = MR::getPlayerPos();
        TVec3f v1 = *playerPos - mPosition;  // FIXME: operator- needs to not inline
        f32 dot = v1.dot(pos);
        if (dot > 0.0f) {
            MR::reverseRailDirection(_160);
            MR::reverseRailDirection(_164);
        }

        setNerve(&NrvPenguinSkater::PenguinSkaterNrvDemo::sInstance);
    }
}

void PenguinSkater::exeDemo() {
    if (MR::isFirstStep(this)) {
        MR::tryStartAction(this, "IceSwimStart");
    }
    if (MR::isBckOneTimeAndStopped(this)) {
        MR::tryStartAction(this, "IceSwim");
    }
    if (MR::isStep(this, sDemoSkateSeStartTiming)) {
        MR::startSound(this, "SE_SM_PENGUIN_SKATE_START", -1, -1);
    }
    if (MR::isGreaterEqualStep(this, sDemoSkateSeStartTiming)) {
        MR::startLevelSound(this, "SE_SM_LV_PENGUIN_SKATE", -1, -1, -1);
    }
    moveRail(sDemoSpeed, 0.05f);
    if (inProvokeRangeIn(calcLead())) {
        MR::endMultiActorCamera(this, mCameraInfo, "開始", false, -1);
        MR::endDemo(this, "ペンギンスケート開始");
        setNerve(&NrvPenguinSkater::PenguinSkaterNrvAway::sInstance);
    }
}

void PenguinSkater::exeAway() {
    if (MR::isFirstStep(this)) {
        MR::tryStartAction(this, "IceSwim");
    }
    MR::startLevelSound(this, "SE_SM_LV_PENGUIN_SKATE", -1, -1, -1);
    moveRail(sAwaySpeed, 0.05f);
    f32 lead = calcLead();
    if (lead < 0.0f) {
        setNerve(&NrvPenguinSkater::PenguinSkaterNrvTurn::sInstance);
    } else if (inSwitchRange(lead) && trySwitchRail()) {
        setNerve(&NrvPenguinSkater::PenguinSkaterNrvSwitch::sInstance);
    } else if (inProvokeRangeIn(lead)) {
        setNerve(&NrvPenguinSkater::PenguinSkaterNrvProvoke::sInstance);
    }
}

void PenguinSkater::exeTurn() {
    if (MR::isFirstStep(this)) {
        MR::tryStartAction(this, "IceSwimTurn");
    }
    if (MR::isBckOneTimeAndStopped(this)) {
        MR::tryStartAction(this, "IceSwim");
    }
    stopRail(1.0f);
    if (MR::isNearZero(MR::getRailCoordSpeed(mRail), 0.001f)) {
        MR::reverseRailDirection(_160);
        MR::reverseRailDirection(_164);
        setNerve(&NrvPenguinSkater::PenguinSkaterNrvAway::sInstance);
    }
}

void PenguinSkater::exeSwitch() {
    MR::startLevelSound(this, "SE_SM_LV_PENGUIN_SKATE", -1, -1, -1);
    moveRail(sSwitchSpeed, 0.05f);
    if (MR::isRailReachedGoal(mRail)) {
        endSwitchRail();
        setNerve(&NrvPenguinSkater::PenguinSkaterNrvAway::sInstance);
    }
}

void PenguinSkater::exeProvoke() {
    if (!MR::isNearZero(MR::getRailCoordSpeed(mRail), 0.001f)) {
        stopRail(1.0f);
    } else {
        MR::tryStartBck(this, "Congratulate1", static_cast< const char* >(0));
        if (MR::isNearPlayer(mMsgCtrl, 10000.0f)) {
            MR::tryTalkForce(mMsgCtrl);
            turnToPlayer();
        }
        f32 lead = calcLead();
        if (!inProvokeRangeIn(lead)) {
            if (lead < 0.0f) {
                MR::reverseRailDirection(_160);
                MR::reverseRailDirection(_164);
            }
            setNerve(&NrvPenguinSkater::PenguinSkaterNrvAway::sInstance);
            MR::startSound(this, "SE_SM_PENGUIN_SKATE_START", -1, -1);
        }
    }
}

void PenguinSkater::exeCaught() {
    if (MR::isFirstStep(this)) {
        MR::tryStartDemoMarioPuppetable(this, "捕まり");
        MR::startMultiActorCameraTargetPlayer(this, mCameraInfo, "終了", -1);
        MR::startAction(this, "Caught");
        MR::startBckPlayer("TossStart", static_cast< const char* >(0));
        MR::startSound(this, "SE_SM_PENGUIN_CAUGHT", -1, -1);
        MR::zeroVelocity(this);
        setCaughtStartMarioPose();
    }
    if (MR::isStep(this, 20)) {
        MR::startSound(this, "SE_SV_PENGUIN_S_CAUGHT", -1, -1);
        MR::startSystemSE("SE_SY_TOTAL_COMPLETE", -1, -1);
    }
    blendBaseMatrixToMario(MR::calcNerveRate(this, sMarioPoseBlendTime));
    if (MR::isActionEnd(this)) {
        setNerve(&NrvPenguinSkater::PenguinSkaterNrvFadeOut::sInstance);
        MR::startBckPlayer("TossWait", static_cast< const char* >(0));
    }
}

void PenguinSkater::exeFadeOut() {
    if (MR::isFirstStep(this)) {
        MtxPtr mtx = getBaseMtx();
        TPos3f pos;
        pos.set(mtx);
        MR::setPlayerBaseMtx(pos);
        MR::startAction(this, "CaughtWait");
        MR::closeWipeCircle(-1);
    }
    if (!MR::isWipeActive()) {
        setNerve(&NrvPenguinSkater::PenguinSkaterNrvFadeIn::sInstance);
    }
}

void PenguinSkater::exeFadeIn() {
    if (MR::isFirstStep(this)) {
        TPos3f pos;
        MR::findNamePos("マリオ移動後", pos);
        MR::setNPCActorPos(this, "ペンギン移動後");
        MR::setPlayerPosOnGroundAndWait("マリオ移動後");
        MR::endMultiActorCamera(this, mCameraInfo, "終了", false, -1);
        MR::endDemo(this, "捕まり");
        MR::startTalkingSequence(this);
        TalkMessageCtrl* ctrl = mMsgCtrl;
        MtxPtr mtx = getBaseMtx();
        MR::startNPCTalkCamera(ctrl, mtx, pos, 1.0f, 0);
        MR::forwardNode(mMsgCtrl);
        MR::startBckPlayer("Watch", static_cast< const char* >(0));
        MR::openWipeCircle(-1);
        mParam.setMoveTalkNoTurnAction("SitDown", "SitDownTalk");
    }
    MR::tryStartTurnAction(this);
    if (!MR::isWipeActive()) {
        setNerve(&NrvPenguinSkater::PenguinSkaterNrvGiveUp::sInstance);
    }
}

void PenguinSkater::exeGiveUp() {
    if (MR::isFirstStep(this)) {
        MR::endTalkingSequence(this);
    }
    if (MR::tryTalkForceAtEndAndStartTalkAction(this)) {
        MR::endNPCTalkCamera(false, -1);
        MR::forwardNode(mMsgCtrl);
        MR::onSwitchA(this);
        MR::validateClipping(this);
        setNerve(&NrvPenguinSkater::PenguinSkaterNrvSitDown::sInstance);
    }
}

void PenguinSkater::exeTalk() {
    if (!MR::tryStartReactionAndPushNerve(this, &NrvPenguinSkater::PenguinSkaterNrvReaction::sInstance)) {
        MR::tryTalkNearPlayerAndStartTalkAction(this);
    }
}

PenguinSkater::~PenguinSkater() {
}
