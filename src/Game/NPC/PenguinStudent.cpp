#include "Game/NPC/PenguinStudent.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroupArray.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/NPC/Penguin.hpp"
#include "Game/NPC/TalkMessageFunc.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

namespace NrvPenguinStudent {
    NEW_NERVE(PenguinStudentNrvSwim, PenguinStudent, Swim);
    NEW_NERVE(PenguinStudentNrvLead, PenguinStudent, Lead);
    NEW_NERVE(PenguinStudentNrvSlow, PenguinStudent, Slow);
};  // namespace NrvPenguinStudent

PenguinStudent::PenguinStudent(const char* pName) : NPCActor(pName) {
    mRemovableTurtle = nullptr;
    mActor = nullptr;
    _15C = -1;
}

PenguinStudent::~PenguinStudent() {}

void PenguinStudent::init(const JMapInfoIter& rIter) {
    MR::initDefaultPosAndQuat(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &_15C);
    initModelManagerWithAnm("Penguin", nullptr, false);
    MR::connectToSceneNpc(this);
    MR::initLightCtrl(this);
    initHitSensor(1);
    MR::addHitSensorPush(this, "push", 8, 50.0f, TVec3f(0.0f, 50.0f, 0.0f));
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::initShadowVolumeSphere(this, 50.0f);
    NPCActor::initTalkCtrl(rIter, "PenguinStudent", TVec3f(0.0f, 150.0f, 0.0f), nullptr);
    NPCActorCaps caps = "";
    caps.mLodCtrl = 1;
    caps.mNerve = false;
    NPCActor::initialize(rIter, caps);
    initNerve(&NrvPenguinStudent::PenguinStudentNrvSwim::sInstance);
    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        MR::moveCoordToNearestPos(this, mPosition);
        mPosition.set< f32 >(MR::getRailPos(this));
    }

    MR::joinToGroupArray(this, rIter, nullptr, 32);
    MR::setGroupClipping(this, rIter, 8);
    MR::invalidateClipping(this);
    if (_15C == -1) {
        mRemovableTurtle = new RemovableTurtle(this, true);
        setNerve(&NrvPenguinStudent::PenguinStudentNrvSlow::sInstance);
    } else {
        s32 arg1 = 10;
        MR::getJMapInfoArg1NoInit(rIter, &arg1);
        _168 = 100.0f * arg1;
    }

    _128 = false;
    _110 = 0.3f;
    MR::useStageSwitchReadA(this, rIter);
    s32 arg7 = 0;
    MR::getJMapInfoArg7NoInit(rIter, &arg7);
    MR::startBrk(this, "ColorChange");
    MR::setBrkFrameAndStop(this, arg7);
    MR::onCalcShadow(this, nullptr);
    if (mMsgCtrl) {
        MR::registerBranchFunc(mMsgCtrl, TalkMessageFunc_Inline(this, &PenguinStudent::branchFunc));
    }
    makeActorAppeared();
}

void PenguinStudent::initAfterPlacement() {
    mActor = nullptr;
    LiveActorGroup* group = MR::getGroupFromArray(this);
    for (s32 i = 0; i < group->mObjectCount; i++) {
        LiveActor* actor = group->getActor(i);
        if (!actor->isNerve(&NrvPenguinStudent::PenguinStudentNrvSwim::sInstance)) {
            mActor = actor;
            return;
        }
    }
}

bool PenguinStudent::branchFunc(u32 msg) {
    return mActor->isNerve(&NrvPenguinStudent::PenguinStudentNrvLead::sInstance);
}

bool PenguinStudent::tryReleaseTurtle() {
    if (_15C == -1 && isNerve(&NrvPenguinStudent::PenguinStudentNrvLead::sInstance) && mRemovableTurtle->isPullRange() &&
        mRemovableTurtle->tryRemove()) {
        setNerve(&NrvPenguinStudent::PenguinStudentNrvSlow::sInstance);
        return true;
    }
    return false;
}

void PenguinStudent::exeSwim() {
    if (MR::isFirstStep(this)) {
        MR::tryStartAction(this, "Swim");
        MR::setBckFrameAtRandom(this);
    }
    f32 speed = ((200.0f * MR::getRailCoordSpeed(mActor)) / 7.0f);
    MR::setRailCoord(this, MR::getRailCoord(mActor) - _168);
    MR::moveCoordAndFollowTrans(this);
    MR::followRailPose(this, _114, 1.0f);
    TVec3f v19, v20, v21;
    MR::calcUpVec(&v20, this);
    MR::calcFrontVec(&v19, this);
    MR::rotAxisVecRad(v20, v19, &v21, 0.017453292f * (getNerveStep() % 360));
    TVec3f v14(v21);
    v14.x *= speed;
    v14.y *= speed;
    v14.z *= speed;
    mPosition.set< f32 >(mPosition + v14);
    if (mMsgCtrl) {
        MR::tryTalkNearPlayer(mMsgCtrl);
    }
}

void PenguinStudent::exeLead() {
    if (MR::isFirstStep(this)) {
        _11C = "SwimTurtle";
        _120 = "SwimTurtle";
        _10C = 7.0f;
    }
    MR::startLevelSound(this, "SE_SM_LV_PENGUIN_SWIM_JET", -1, -1, -1);
    MR::tryStartMoveTalkAction(this);
    if (MR::getRailCoordSpeed(this) <= 7.0f) {
        MR::tryTalkNearPlayer(mMsgCtrl);
    }
}

void PenguinStudent::exeSlow() {
    if (MR::isFirstStep(this)) {
        _11C = "Swim";
        _120 = "Swim";
        _10C = 5.0f;
    }

    MR::tryStartMoveTalkAction(this);
    if (MR::getRailCoordSpeed(this) <= 5.0f) {
        MR::tryTalkNearPlayer(mMsgCtrl);
    }

    if (!MR::isStageStatePowerStarAppeared() && MR::isOnSwitchA(this)) {
        if (mRemovableTurtle->tryAttach()) {
            setNerve(&NrvPenguinStudent::PenguinStudentNrvLead::sInstance);
        }
    }
}

void PenguinStudent::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        tryReleaseTurtle();
    }

    NPCActor::attackSensor(pSender, pReceiver);
}

bool PenguinStudent::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgSpinStormRange(msg) && tryReleaseTurtle()) {
        return true;
    }

    return false;
}
