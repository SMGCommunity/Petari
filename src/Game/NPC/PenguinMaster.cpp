#include "Game/NPC/PenguinMaster.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

namespace NrvPenguinMaster {
    NEW_NERVE(PenguinMasterNrvWait, PenguinMaster, Wait);
    NEW_NERVE(PenguinMasterNrvReaction, PenguinMaster, Reaction);
}; // namespace NrvPenguinMaster

PenguinMaster::PenguinMaster(const char* pName)
    : NPCActor(pName) {
}

PenguinMaster::~PenguinMaster() {
}

void PenguinMaster::init(const JMapInfoIter& rIter) {
    f32          stack = 0.0f;
    NPCActorCaps caps = "PenguinMaster";
    caps.setDefault();
    caps.mUseHitSensors = false;
    caps._60 = 130.0f;
    caps.mMessageOffset.setPS2(TVec3f(0.0f, 430.0f, 0.0f));
    caps._80 = 180.0f;
    caps.mStarPointerOffs.x = 0.0f;
    caps.mStarPointerOffs.y = 200.0f;
    caps.mStarPointerOffs.z = 80.0f;
    caps.mWaitNerve = &NrvPenguinMaster::PenguinMasterNrvWait::sInstance;
    LiveActor::initHitSensor(2);
    MR::addHitSensorPush(this, "body", 8, 150.0f, TVec3f(0.0f, 100.0f, 0.0f));
    MR::addHitSensorPush(this, "head", 8, 150.0f, TVec3f(0.0f, 260.0f, 100.0f));
    initialize(rIter, caps);
    const char* talk = "Talk";
    mParam._14 = "Wait";
    mParam._1C = talk;
    mParam._18 = nullptr;
    mParam._20 = nullptr;
    mParam._0 = false;
    mParam._1 = false;
    _134 = "Trampled";
    _13C = "Reaction";
    _130 = "Reaction";
    _138 = "Pointing";
    if (mMsgCtrl) {
        MR::setDistanceToTalk(mMsgCtrl, 350.0f);
    }
}

bool PenguinMaster::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerSpinAttack(msg)) {
        _E3 = true;
        return true;
    } else {
        return NPCActor::receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }
}

void PenguinMaster::exeWait() {
    MR::tryTalkNearPlayerAndStartTalkAction(this);
    MR::tryStartReactionAndPushNerve(this, &NrvPenguinMaster::PenguinMasterNrvReaction::sInstance);
}

void PenguinMaster::exeReaction() {
    if (_D8) {
        MR::startSound(this, "SE_SV_PENGUIN_OLD_TRAMPLED", -1, -1);
        MR::startSound(this, "SE_SM_PENGUIN_TRAMPLED", -1, -1);
    }

    if (isPointingSe()) {
        MR::startSound(this, "SE_SV_PENGUIN_OLD_POINTED_2P", -1, -1);
        MR::startDPDHitSound();
    }

    if (_D9) {
        MR::startSound(this, "SE_SV_PENGUIN_OLD_SPIN_HIT", -1, -1);
    }

    if (_DB) {
        MR::startSound(this, "SE_SV_PENGUIN_OLD_STAR_PIECE", -1, -1);
    }

    if (!MR::tryStartReactionAndPopNerve(this)) {
        if (MR::tryStartReactionAndPopNerve(this)) {
            return;
        }
    }
}
