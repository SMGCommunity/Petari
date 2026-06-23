#include "Game/NPC/PenguinSkater.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/NPCSupportRail.hpp"
#include "Game/NPC/TalkDirector.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <algorithm>

namespace {
    static const f32 sProvokeRangeIn = 1000.0f;
    static const f32 sProvokeRangeOut = 2000.0f;
    static const f32 sSwitchRange = 3000.0f;
    static const f32 sAwaySpeed = 14.0f;
    static const f32 sSwitchSpeed = 10.0f;
    static const f32 sDemoSpeed = 18.0f;
    static const f32 sAwayAccele = 0.15f;
    static const f32 sAwayBrake = 0.2f;
    static const f32 sBlendRatio = 0.05f;
    static const f32 sSameDirection = 0.55f;
    static const f32 sSamePosition = 10.0f;
    static const s32 sMarioPoseBlendTime = 5;
    static const s32 sDemoSkateSeStartTiming = 33;
};  // namespace

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

PenguinSkater::PenguinSkater(const char* pName)
    : NPCActor(pName), mCameraInfo(0), mCurrentRail(nullptr), mSwitchRail(nullptr), mRail(nullptr), mNumRails(0) {
    // TODO: doubtful this is written like this, fix later.
    mMarioQuat.set< f32 >(0.0f, 0.0f, 0.0f, 1.0f);
    mMarioPos.z = 0.0f;
    mMarioPos.y = 0.0f;
    mMarioPos.x = 0.0f;
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
    mParam.setMoveAction("Wait", "Turn");
    mParam.setTalkAction("Talk", "TalkTurn");
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
                    mCurrentRail = static_cast< NPCSupportRail* >(actor);
                } else {
                    mSwitchRail = static_cast< NPCSupportRail* >(actor);
                }
            } else {
                mSwitchRails[mNumRails++] = static_cast< NPCSupportRail* >(actor);
            }
        }
        mRail = mCurrentRail;
        MR::followRailPoseOnGround(this, mCurrentRail, 1.0f);
    }
}

bool PenguinSkater::isAttackable() const {
    return (isNerve(&NrvPenguinSkater::PenguinSkaterNrvAway::sInstance) || isNerve(&NrvPenguinSkater::PenguinSkaterNrvTurn::sInstance) ||
            isNerve(&NrvPenguinSkater::PenguinSkaterNrvSwitch::sInstance));
}

bool PenguinSkater::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerSpinAttack(msg)) {
        if (isAttackable()) {
            MR::tryRumblePadStrong(this, WPAD_CHAN0);
            setNerve(&NrvPenguinSkater::PenguinSkaterNrvCaught::sInstance);
        }
    }

    return NPCActor::receiveMsgPlayerAttack(msg, pSender, pReceiver);
}

void PenguinSkater::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (isAttackable()) {
            MR::tryRumblePadStrong(this, WPAD_CHAN0);
            setNerve(&NrvPenguinSkater::PenguinSkaterNrvCaught::sInstance);
        }
    }
    NPCActor::attackSensor(pSender, pReceiver);
}

f32 PenguinSkater::calcLead() const {
    TVec3f playerRailPos;
    f32 playerRailCoord = MR::calcRailPosNearestPlayer(&playerRailPos, mRail);
    f32 coord = MR::getRailCoord(mRail);
    f32 length = MR::getRailTotalLength(mRail);

    f32 half = length / 2;
    f32 diff = __fabsf(coord - playerRailCoord);
    bool ahead = coord > playerRailCoord;

    if (MR::isRailGoingToEnd(mRail)) {
        if ((ahead && diff > half) || (!ahead && diff < half)) {
            diff *= -1;
        }
    } else {
        if ((ahead && diff < half) || (!ahead && diff > half)) {
            diff *= -1;
        }
    }

    return diff;
}

void PenguinSkater::moveRail(f32 speed, f32 blendRate) {
    MR::adjustmentRailCoordSpeed(mRail, speed, ::sAwayAccele);
    mCrossPoint = MR::moveCoordAndFollowTransAndCheckPassPointNo(mRail);
    MR::followRailPoseOnGround(this, mRail, blendRate);
}

void PenguinSkater::stopRail(f32 blendRate) {
    MR::adjustmentRailCoordSpeed(mRail, 0.0f, ::sAwayBrake);
    mCrossPoint = MR::moveCoordAndFollowTransAndCheckPassPointNo(mRail);
    MR::followRailPoseOnGround(this, mRail, blendRate);
}

bool PenguinSkater::inProvokeRangeIn(f32 dist) const {
    f32 absDist = __fabsf(dist);
    return (__fabsf((MR::getRailTotalLength(mRail) / 2) - absDist) < ::sProvokeRangeIn);
}

bool PenguinSkater::inProvokeRangeOut(f32 dist) const {
    f32 absDist = __fabsf(dist);
    return (__fabsf((MR::getRailTotalLength(mRail) / 2) - absDist) < ::sProvokeRangeOut);
}

bool PenguinSkater::inSwitchRange(f32 dist) const {
    if (__fabsf(dist) > ::sSwitchRange) {
        return false;
    }
    return mCrossPoint != -1;
}

bool PenguinSkater::trySwitchRail() {
    TVec3f crossPos;
    TVec3f pos, dir;

    f32 speed = MR::getRailCoordSpeed(mRail);
    TVec3f railDir = MR::getRailDirection(mRail);

    MR::calcRailPointPos(&crossPos, mRail, mCrossPoint);

    for (s32 idx = 0; idx < mNumRails; idx++) {
        NPCSupportRail* rail = mSwitchRails[idx];

        MR::calcRailStartPointPos(&pos, rail);
        MR::calcRailStartPointDirection(&dir, rail);

        if ((crossPos - pos).length() < ::sSamePosition && railDir.dot(dir) > ::sSameDirection) {
            MR::setRailDirectionToEnd(rail);
            MR::setRailCoord(rail, 0.0f);
            MR::setRailCoordSpeed(rail, speed);
            mRail = rail;
            return true;
        }

        MR::calcRailEndPointPos(&pos, rail);
        MR::calcRailEndPointDirection(&dir, rail);

        if ((crossPos - pos).length() < ::sSamePosition && railDir.dot(dir) < -::sSameDirection) {
            MR::setRailDirectionToStart(rail);
            MR::setRailCoord(rail, MR::getRailTotalLength(rail));
            MR::setRailCoordSpeed(rail, speed);
            mRail = rail;
            return true;
        }
    }

    return false;
}

void PenguinSkater::endSwitchRail() {
    f32 speed = MR::getRailCoordSpeed(mRail);

    mRail = mSwitchRail;
    std::swap(mCurrentRail, mSwitchRail);

    MR::setRailCoordSpeed(mCurrentRail, speed);
    MR::moveCoordToNearestPos(mRail, mPosition);
}

void PenguinSkater::setCaughtStartMarioPose() {
    TPos3f baseMtx;
    baseMtx.set(MR::getPlayerDemoActor()->getBaseMtx());
    baseMtx.getQuat(mMarioQuat);
    baseMtx.getTransInline(mMarioPos);
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
        MR::startSound(this, "SE_SM_PENGUIN_TRAMPLED");
        MR::startSound(this, "SE_SV_PENGUIN_S_TRAMPLED");
    }
    if (isPointingSe()) {
        MR::startSound(this, "SE_SV_PENGUIN_S_POINTED_2P");
        MR::startDPDHitSound();
    }
    if (_D9) {
        MR::startSound(this, "SE_SV_PENGUIN_S_SPIN_HIT");
    }
    if (_DB) {
        MR::startSound(this, "SE_SV_PENGUIN_S_STAR_PIECE");
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
        if ((*MR::getPlayerPos() - mPosition).dot(pos) > 0.0f) {
            MR::reverseRailDirection(mCurrentRail);
            MR::reverseRailDirection(mSwitchRail);
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
    if (MR::isStep(this, ::sDemoSkateSeStartTiming)) {
        MR::startSound(this, "SE_SM_PENGUIN_SKATE_START");
    }
    if (MR::isGreaterEqualStep(this, ::sDemoSkateSeStartTiming)) {
        MR::startLevelSound(this, "SE_SM_LV_PENGUIN_SKATE");
    }
    moveRail(::sDemoSpeed, ::sBlendRatio);
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
    MR::startLevelSound(this, "SE_SM_LV_PENGUIN_SKATE");
    moveRail(::sAwaySpeed, ::sBlendRatio);
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
    if (MR::isNearZero(MR::getRailCoordSpeed(mRail))) {
        MR::reverseRailDirection(mCurrentRail);
        MR::reverseRailDirection(mSwitchRail);
        setNerve(&NrvPenguinSkater::PenguinSkaterNrvAway::sInstance);
    }
}

void PenguinSkater::exeSwitch() {
    MR::startLevelSound(this, "SE_SM_LV_PENGUIN_SKATE");
    moveRail(::sSwitchSpeed, ::sBlendRatio);
    if (MR::isRailReachedGoal(mRail)) {
        endSwitchRail();
        setNerve(&NrvPenguinSkater::PenguinSkaterNrvAway::sInstance);
    }
}

void PenguinSkater::exeProvoke() {
    if (!MR::isNearZero(MR::getRailCoordSpeed(mRail))) {
        stopRail(1.0f);
    } else {
        MR::tryStartBck(this, "Congratulate1", static_cast< const char* >(nullptr));
        if (MR::isNearPlayer(mMsgCtrl, 10000.0f)) {
            MR::tryTalkForce(mMsgCtrl);
            turnToPlayer();
        }
        f32 lead = calcLead();
        if (!inProvokeRangeOut(lead)) {
            if (lead < 0.0f) {
                MR::reverseRailDirection(mCurrentRail);
                MR::reverseRailDirection(mSwitchRail);
            }
            setNerve(&NrvPenguinSkater::PenguinSkaterNrvAway::sInstance);
            MR::startSound(this, "SE_SM_PENGUIN_SKATE_START");
        }
    }
}

void PenguinSkater::exeCaught() {
    if (MR::isFirstStep(this)) {
        MR::tryStartDemoMarioPuppetable(this, "捕まり");
        MR::startMultiActorCameraTargetPlayer(this, mCameraInfo, "終了", -1);
        MR::startAction(this, "Caught");
        MR::startBckPlayer("TossStart", static_cast< const char* >(nullptr));
        MR::startSound(this, "SE_SM_PENGUIN_CAUGHT");
        MR::zeroVelocity(this);
        setCaughtStartMarioPose();
    }
    if (MR::isStep(this, 20)) {
        MR::startSound(this, "SE_SV_PENGUIN_S_CAUGHT");
        MR::startSystemSE("SE_SY_TOTAL_COMPLETE");
    }
    blendBaseMatrixToMario(MR::calcNerveRate(this, ::sMarioPoseBlendTime));
    if (MR::isActionEnd(this)) {
        setNerve(&NrvPenguinSkater::PenguinSkaterNrvFadeOut::sInstance);
        MR::startBckPlayer("TossWait", static_cast< const char* >(nullptr));
    }
}

void PenguinSkater::exeFadeOut() {
    if (MR::isFirstStep(this)) {
        TPos3f pos;
        pos.set(getBaseMtx());
        MR::setPlayerBaseMtx(pos);
        MR::startAction(this, "CaughtWait");
        MR::closeWipeCircle();
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
        MR::startNPCTalkCamera(getMsgCtrl(), getBaseMtx(), pos, 1.0f, 0);
        MR::forwardNode(getMsgCtrl());
        MR::startBckPlayer("Watch", static_cast< const char* >(nullptr));
        MR::openWipeCircle();
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

void PenguinSkater::exeSitDown() {
    MR::tryTalkNearPlayerAndStartTalkAction(this);
}

void PenguinSkater::exeTalk() {
    if (!MR::tryStartReactionAndPushNerve(this, &NrvPenguinSkater::PenguinSkaterNrvReaction::sInstance)) {
        MR::tryTalkNearPlayerAndStartTalkAction(this);
    }
}
