#include "Game/NPC/LuigiNPC.hpp"
#include "Game/Demo/AstroDemoFunction.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/NPC/TalkMessageFunc.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/Util/TalkUtil.hpp"

namespace {
    static const s32 sLiftUpSeTiming = 87;
    static const f32 sAfraidSeRange = 600.0f;
    static const f32 sTreeDist = 16.0f;
    static const f32 sShadowOffset = -25.0f;
    static const f32 sHitJump = 20.0f;
    static const f32 sHitMove = 4.0f;
};  // namespace

namespace NrvLuigiNPC {
    NEW_NERVE(LuigiNPCNrvWait, LuigiNPC, Wait);
    NEW_NERVE(LuigiNPCNrvReaction, LuigiNPC, Reaction);
    NEW_NERVE(LuigiNPCNrvTakeOutStar, LuigiNPC, TakeOutStar);
    NEW_NERVE(LuigiNPCNrvAfraidWait, LuigiNPC, AfraidWait);
    NEW_NERVE(LuigiNPCNrvAfraidSquat, LuigiNPC, AfraidSquat);
    NEW_NERVE(LuigiNPCNrvAfraidRise, LuigiNPC, AfraidRise);
    NEW_NERVE(LuigiNPCNrvArrestedWait, LuigiNPC, ArrestedWait);
    NEW_NERVE(LuigiNPCNrvArrestedJump, LuigiNPC, ArrestedJump);
    NEW_NERVE(LuigiNPCNrvOnTreeWait, LuigiNPC, OnTreeWait);
    NEW_NERVE(LuigiNPCNrvOnTreeTouch, LuigiNPC, OnTreeTouch);
    NEW_NERVE(LuigiNPCNrvOnTreeFall, LuigiNPC, OnTreeFall);
    NEW_NERVE(LuigiNPCNrvOnTreeLand, LuigiNPC, OnTreeLand);
};  // namespace NrvLuigiNPC

LuigiNPC::LuigiNPC(const char* pName) : NPCActor(pName), mTakeOutStar(), mType(Type_Invalid) {
}

void LuigiNPC::makeArchiveList(NameObjArchiveListCollector* pArchive, const JMapInfoIter& rIter) {
    pArchive->addArchive("LuigiNPC");
    pArchive->addArchive("LuigiNPCMiddle");
    pArchive->addArchive("LuigiNPCLow");
}

bool LuigiNPC::branchFunc(u32 msg) {
    return !MR::isNearPlayer(mMsgCtrl, -1.0f) || MR::inMessageArea(mMsgCtrl);
}

bool LuigiNPC::eventFunc(u32 msg) {
    if (msg == 0) {
        return mTakeOutStar->takeOut();
    }

    if (msg == 1) {
        MR::onGameEventFlagTalkedToLuigiAfterRescued();
    }

    return true;
}

void LuigiNPC::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) && isNerve(&NrvLuigiNPC::LuigiNPCNrvOnTreeWait::sInstance)) {
        setNerve(&NrvLuigiNPC::LuigiNPCNrvOnTreeTouch::sInstance);
    }

    NPCActor::attackSensor(pSender, pReceiver);
}

bool LuigiNPC::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStartPowerStarGet(msg)) {
        makeActorDead();

        return true;
    }

    return false;
}

bool LuigiNPC::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mType == Type_OnTree) {
        if (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg)) {
            MR::startSound(this, "SE_SM_NPC_TRAMPLED");
        }

        if (MR::isMsgStarPieceReflect(msg)) {
            MR::startSound(this, "SE_SM_STAR_PIECE_HIT");
        }

        if ((MR::isMsgPlayerSpinAttack(msg) || MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg) || MR::isMsgStarPieceReflect(msg)) &&
            (isNerve(&NrvLuigiNPC::LuigiNPCNrvOnTreeWait::sInstance) || isNerve(&NrvLuigiNPC::LuigiNPCNrvOnTreeTouch::sInstance))) {
            setNerve(&NrvLuigiNPC::LuigiNPCNrvOnTreeFall::sInstance);
        }
    }

    return NPCActor::receiveMsgPlayerAttack(msg, pSender, pReceiver);
}

void LuigiNPC::init(const JMapInfoIter& rIter) {
    const char* objName;
    MR::getObjectName(&objName, rIter);

    NPCActorCaps caps(objName);
    caps.setDefault();
    caps.mObjectName = "LuigiNPC";
    caps.mWaitNerve = &NrvLuigiNPC::LuigiNPCNrvWait::sInstance;

    MR::getJMapInfoArg0NoInit(rIter, &mType);

    if (MR::isEqualString(objName, "LuigiEvent")) {
        mType += Type_Event;
    }

    NPCActor::initialize(rIter, caps);
    MR::registerBranchFunc(mMsgCtrl, TalkMessageFunc(this, &LuigiNPC::branchFunc));
    MR::registerEventFunc(mMsgCtrl, TalkMessageFunc(this, &LuigiNPC::eventFunc));
    setWaitAction();
    setNerve(&NrvLuigiNPC::LuigiNPCNrvWait::sInstance);

    switch (mType) {
    case Type_Afraid:
        setNerve(&NrvLuigiNPC::LuigiNPCNrvAfraidWait::sInstance);
        setAfraidAction();
        break;
    case Type_Normal:
        if (!MR::isOnGameEventFlagLuigiRescued() || MR::isLuigiDisappearFromAstroGalaxyOrHiding()) {
            makeActorDead();
        }

        AstroDemoFunction::tryRegisterDemoForLuigiAndKinopio(this, rIter);
        break;
    case Type_Event:
        break;
    case Type_Arrested:
        MR::useStageSwitchReadA(this, rIter);
        MR::useStageSwitchWriteB(this, rIter);
        setNerve(&NrvLuigiNPC::LuigiNPCNrvArrestedWait::sInstance);
        break;
    case Type_OnTree:
        MR::onCalcShadowDropPrivateGravity(this, nullptr);
        TVec3f offset(0.0f, 0.0f, ::sShadowOffset);
        MR::setShadowDropPositionAtJoint(this, nullptr, "Center", offset);
        setNerve(&NrvLuigiNPC::LuigiNPCNrvOnTreeWait::sInstance);
        break;
    default:
        break;
    }

    if (isDeclarePowerStarType()) {
        MR::declarePowerStar(this);
        mTakeOutStar = new TakeOutStar(this, "TakeOutStar", "TakeOutStar", &NrvLuigiNPC::LuigiNPCNrvTakeOutStar::sInstance);
    }

    bool stat = mType > Type_Afraid && mType < Type_Count;

    if (!stat) {
        return;
    }

    if (MR::isOnLuigiHidingCurrentStage()) {
        makeActorAppeared();

        if (MR::isValidSwitchB(this)) {
            MR::onSwitchB(this);
        }
    } else {
        makeActorDead();
    }
}

void LuigiNPC::initAfterPlacement() {
    NPCActor::initAfterPlacement();

    if (mType != Type_OnTree) {
        return;
    }

    TVec3f front;
    TVec3f result;
    Triangle poly;

    MR::calcFrontVec(&front, this);

    if (MR::getFirstPolyOnLineToMap(&result, &poly, mPosition, front * 500.0f)) {
        mPosition.set(result - (front * ::sTreeDist));
        MR::onCalcShadow(this, nullptr);
        MR::offBind(this);
    }
}

void LuigiNPC::control() {
    NPCActor::control();
}

bool LuigiNPC::trySetNerveAfraid() {
    if (MR::isNearPlayer(mMsgCtrl, -1.0f) && !MR::inMessageArea(mMsgCtrl) && !MR::isPlayerElementModeTeresa()) {
        return MR::trySetNerve(this, &NrvLuigiNPC::LuigiNPCNrvAfraidWait::sInstance);
    }

    if (MR::isNearPlayer(mMsgCtrl, -1.0f) && MR::isPlayerElementModeTeresa()) {
        return MR::trySetNerve(this, &NrvLuigiNPC::LuigiNPCNrvAfraidSquat::sInstance);
    }

    return MR::trySetNerve(this, &NrvLuigiNPC::LuigiNPCNrvAfraidWait::sInstance);
}

void LuigiNPC::setWaitAction() {
    const char* turn = "Turn";
    mParam._14 = "Wait";
    mParam._18 = turn;
    mParam._1C = "Talk";
    mParam._20 = turn;
    setDefaults();
    _12C = 1500.0f;
    _128 = true;
}

void LuigiNPC::setAfraidAction() {
    _128 = false;
    const char* afraidWait = "AfraidWait";
    const char* afraid = "Afraid";
    const char* afraidTalk = "AfraidTalk";
    mParam._14 = afraidWait;
    mParam._18 = afraid;
    mParam._1C = afraidTalk;
    mParam._20 = afraid;
}

bool LuigiNPC::trySetNerveArrested() {
    if (isNerve(&NrvLuigiNPC::LuigiNPCNrvArrestedWait::sInstance) && MR::isOnSwitchA(this)) {
        return MR::trySetNerve(this, &NrvLuigiNPC::LuigiNPCNrvArrestedJump::sInstance);
    }

    if (isNerve(&NrvLuigiNPC::LuigiNPCNrvArrestedJump::sInstance) && MR::isBckOneTimeAndStopped(this)) {
        return MR::trySetNerve(this, &NrvLuigiNPC::LuigiNPCNrvWait::sInstance);
    }

    return false;
}

bool LuigiNPC::isDeclarePowerStarType() {
    return mType == Type_Afraid || mType >= Type_Event && mType < Type_Count;
}

void LuigiNPC::exeWait() {
    if (MR::isFirstStep(this)) {
        setWaitAction();
    }

    MR::tryTalkNearPlayerAndStartTalkAction(this);

    if (MR::tryStartReactionAndPushNerve(this, &NrvLuigiNPC::LuigiNPCNrvReaction::sInstance)) {
        return;
    }
}

void LuigiNPC::exeReaction() {
    if (_D8) {
        MR::startSound(this, "SE_SV_LUIGI_TRAMPLED");
        MR::startSound(this, "SE_SM_NPC_TRAMPLED");
    }

    if (NPCActor::isPointingSe()) {
        MR::startDPDHitSound();
        MR::startSound(this, "SE_SV_LUIGI_POINT");
    }

    if (_D9) {
        MR::startSound(this, "SE_SV_LUIGI_SPIN");
    }

    if (_DB) {
        MR::startSound(this, "SE_SV_LUIGI_STAR_PIECE_HIT");
    }

    if (MR::tryStartReactionAndPopNerve(this)) {
        return;
    }
}

void LuigiNPC::exeTakeOutStar() {
    if (MR::isFirstStep(this)) {
    }

    if (MR::isStep(this, ::sLiftUpSeTiming)) {
        MR::startSound(this, "SE_SV_LUIGI_LIFT_UP");
    }

    if (MR::isGreaterStep(this, ::sLiftUpSeTiming)) {
        MR::startLevelSound(mTakeOutStar->mStarModel, "SE_OJ_LV_POW_STAR_EXIST");
    }
}

void LuigiNPC::exeAfraidWait() {
    if (MR::isFirstStep(this)) {
        setAfraidAction();
    }

    if (!trySetNerveAfraid()) {
        MR::tryTalkNearPlayerAndStartTalkAction(this);
    }
}

void LuigiNPC::exeAfraidSquat() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "SquatStart");
    }

    if (MR::isBckOneTimeAndStopped(this)) {
        MR::startAction(this, "Squat");
    }

    if (trySetNerveAfraid()) {
        setNerve(&NrvLuigiNPC::LuigiNPCNrvAfraidRise::sInstance);
    } else {
        MR::tryTalkNearPlayer(mMsgCtrl);
    }
}

void LuigiNPC::exeAfraidRise() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "SquatEnd");
    }

    if (MR::isBckOneTimeAndStopped(this)) {
        setNerve(&NrvLuigiNPC::LuigiNPCNrvAfraidWait::sInstance);
    }
}

void LuigiNPC::exeArrestedWait() {
    if (!trySetNerveArrested()) {
        if (MR::tryTalkNearPlayer(mMsgCtrl)) {
            MR::tryStartAction(this, "ArrestedTalk");
        } else {
            MR::tryStartAction(this, "Arrested");
        }
    }
}

void LuigiNPC::exeArrestedJump() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Leave");
        MR::startSound(this, "SE_SV_LUIGI_ESCAPE");
    }

    if (trySetNerveArrested()) {
        return;
    }
}

void LuigiNPC::exeOnTreeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "TreeWait");
    }

    if (MR::calcDistanceToPlayer(mPosition) <= ::sAfraidSeRange) {
        MR::startLevelSound(this, "SE_SV_LV_LUIGI_AFRAID");
    }
}

void LuigiNPC::exeOnTreeTouch() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "TreeTrampled");
        MR::startSound(this, "SE_SV_LUIGI_AFRAID_TOUCH");
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvLuigiNPC::LuigiNPCNrvOnTreeWait::sInstance);
    }
}

void LuigiNPC::exeOnTreeFall() {
    if (MR::isFirstStep(this)) {
        TVec3f front;

        MR::onBind(this);
        MR::startAction(this, "TreeHit");
        MR::onCalcGravity(this);
        MR::calcFrontVec(&front, this);
        MR::addVelocityMoveToDirection(this, front, -::sHitMove);
        MR::addVelocityJump(this, ::sHitJump);
        TVec3f offset(0.0f, 0.0f, 0.0f);
        MR::setShadowDropPositionAtJoint(this, nullptr, "Center", offset);
        MR::startSound(this, "SE_SV_LUIGI_FALL");
    }

    MR::addVelocityToGravity(this, 1.0f);

    if (MR::isOnGround(this)) {
        mVelocity.zero();
        setNerve(&NrvLuigiNPC::LuigiNPCNrvOnTreeLand::sInstance);
    }
}

void LuigiNPC::exeOnTreeLand() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "TreeLand");
        MR::startSound(this, "SE_SM_LUIGI_LAND");
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvLuigiNPC::LuigiNPCNrvWait::sInstance);
    }
}
