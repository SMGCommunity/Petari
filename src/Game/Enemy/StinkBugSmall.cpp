#include "Game/Enemy/StinkBugSmall.hpp"
#include "Game/Enemy/StinkBugBase.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <JSystem/JMath/JMath.hpp>
#include <revolution/types.h>

StinkBugSmall::StinkBugSmall(const char* pName)
    : StinkBugBase(pName), mScaleController(nullptr), mBindStarPointer(nullptr), mStateBaseInterface(nullptr) {
}
StinkBugSmall::~StinkBugSmall() {
}

s32 StinkBugSmall::tryForceFall() {
    const LiveActor* pActor = this;
    if (!MR::isValidSwitchA(pActor) || !MR::isOnSwitchA(pActor)) {
        return 0;
    }
    if (LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDPDSwoon::sInstance)) {
        return 0;
    }
    if (LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvForceFall::sInstance)) {
        return 0;
    }
    LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvForceFall::sInstance);
    return 1;
}

s32 StinkBugSmall::tryDPDSwoon() {
    const LiveActor* pActor = this;
    if (LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance)) {
        return 0;
    }
    if (LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDPDSwoon::sInstance)) {
        return 0;
    }

    if (!mBindStarPointer->tryStartPointBind()) {
        return 0;
    }
    LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvDPDSwoon::sInstance);
    return 1;
}

s32 StinkBugSmall::isShakeChance() const {
    if (LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance)) {
        return 0;
    }
    if (!LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvShake::sInstance)) {
        return 0;
    }
    if (MR::isPlayerHipDropFalling()) {
        return 0;
    }
    return MR::isPlayerHipDropLand() == 0;
}

void StinkBugSmall::kill() {
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
    if (!MR::isValidSwitchDead(this)) {
        MR::appearStarPiece(this, this->mPosition, 3, 10.0f, 40.0f, 0);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
    }
    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
    LiveActor::kill();
}

bool StinkBugSmall::exeHipDropDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Death", 0);
        MR::startBrk(this, "Death");
        if (!unk196) {
            MR::invalidateCollisionParts(this);
        }
        MR::invalidateHitSensors(this);
        MR::invalidateClipping(this);
        MR::startSound(this, "SE_EM_STOMPED_S", -1, -1);
        MR::startSound(this, "SE_EV_STINKBUG_S_STOMPED", -1, -1);
    }
    this->mVelocity.zero();
    bool result = MR::isBckStopped(this);
    if (result) {
        return this->exeHipDropDown();
    } else {
        return result;
    }
    return result;
}

s32 StinkBugSmall::isNrvEnableStarPieceAttack() const {
    bool Inerves = LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvWait::sInstance) ||
                   LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvSearch::sInstance) ||
                   LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashSign::sInstance) ||
                   LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashSignEnd::sInstance) ||
                   LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance);
    if (Inerves) {
        return 1;
    }
    return 0;
}

bool StinkBugSmall::receiveOtherMsg(u32 msg, HitSensor* pReceiver, HitSensor* pSender) {
    if (LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance)) {
        return false;
    }
    if (MR::isPlayerElementModeInvincible() && (MR::isMsgFloorTouch(msg) || MR::isMsgWallTouch(msg) || MR::isMsgCeilTouch(msg))) {
        LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance);
        return true;
    }
    if (LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDPDSwoon::sInstance)) {
        return false;
    }
    if (LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvPanic::sInstance)) {
        return false;
    }
    if (LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvShakeStart::sInstance)) {
        return false;
    }
    {
        if (MR::isMsgFloorTouch(msg)) {
            if (MR::isOnPlayer(LiveActor::getSensor("body"))) {
                if (StinkBugSmall::isShakeChance()) {
                    MR::sendMsgEnemyAttackFlip(pReceiver, pSender);
                } else {
                    LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvPanic::sInstance);
                    return true;
                }
            }
        }
    }
    return false;
}

bool StinkBugSmall::receiveMsgPlayerAttack(u32 msg, HitSensor* pReceiver, HitSensor* pSender) {
    if (LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance)) {
        return false;
    }
    if (LiveActor::getSensor("body")) {
        return false;
    }
    if (MR::isMsgInvincibleAttack(msg) || MR::isMsgPlayerHipDropFloor(msg) || MR::isMsgPlayerHipDrop(msg)) {
        LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance);
        return true;
    } else if (!LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDPDSwoon::sInstance) && !MR::isMsgLockOnStarPieceShoot(msg)) {
        if (MR::isMsgStarPieceReflect(msg) && LiveActor::getSensor("body")) {
            if (StinkBugSmall::isNrvEnableStarPieceAttack()) {
                LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvSpinReaction::sInstance);
                return true;
            }
        }
        if (!LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvSpinReaction::sInstance) &&
            !LiveActor::isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDash::sInstance) && MR::isMsgPlayerSpinAttack(msg) &&
            LiveActor::getSensor("body")) {
            LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvSpinReaction::sInstance);
            return true;
        }
    }
    return false;
}

void StinkBugSmall::exeWait() {
    if (MR::isFirstStep(this) && this->mScale.x != 0.0f) {
        MR::tryStartBck(this, "Search", 0);
    }
    StinkBugBase::fixInitPos();
    if (StinkBugBase::isPlayerInTerritory(400.0, 600.0, 200.0, 200.0)) {
        LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashSign::sInstance);
    } else if (this->mScale.x != 0.0f) {
        if (MR::isGreaterStep(this, 1)) {
            LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvSearch::sInstance);
        }
    }
}
void StinkBugSmall::exeSearch() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Search", 0);
    }
    StinkBugBase::fixInitPos();
    if (StinkBugBase::tryTurnSearch(1.0)) {
        LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvWait::sInstance);
    } else if (StinkBugBase::isPlayerInTerritory(400.0, 600.0, 200.0, 200.0)) {
        LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashSign::sInstance);
    }
}
void StinkBugSmall::exeDashSign() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "RushStart", 0);
        MR::startSound(this, "SE_EV_STINKBUG_S_FIND", -1, -1);
    }
    MR::startLevelSound(this, "SE_EM_STINKBUG_S_DASH_SIGN", -1, -1, -1);
    StinkBugBase::fixInitPos();
    StinkBugBase::tryTurnDashSign(3.0f);
    if (MR::isBckStopped(this)) {
        LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashSignEnd::sInstance);
    }
}
void StinkBugSmall::exeDash() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Rush", 0);
        MR::validateHitSensors(this);
    }
    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_S_DASH", -1, -1, -1);
    if (MR::isNear(this, this->mPosition, this->mScale.y) && !MR::isBindedWall(this)) {
        return StinkBugBase::setDashVelocity(20.0f);
    }
    this->mVelocity.zero();
    LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashEnd::sInstance);
    return MR::startSound(this, "SE_EM_STINKBUG_S_DASH_END", -1, -1);
}
void StinkBugSmall::exeDashEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "RushStop", 0);
    }
    this->mVelocity.zero();
    if (MR::isBckStopped(this)) {
        LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance);
    }
}
void StinkBugSmall::exeBack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Back", 0);
        MR::invalidateHitSensor(this, "head");
    }
    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_S_BACK", -1, -1, -1);
    if (MR::isNear(this, this->mPosition, 10.0f)) {
        return LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvWait::sInstance);
    }
    TVec3f tvf;
    MR::normalize(&tvf);
    MR::turnVecToPlane(&tvf, tvf, this->mGravity);
    this->mVelocity.scale(5.0);
}
void StinkBugSmall::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Attack", 0);
    }
    this->mVelocity.zero();
    if (MR::isBckStopped(this)) {
        LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance);
    }
}
void StinkBugSmall::exeSpinReaction() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SpinAction", 0);
    }
    MR::startSound(this, "SE_EM_GUARD_S", -1, -1);
    this->mVelocity.zero();
    if (MR::isBckStopped(this)) {
        LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance);
    }
}
void StinkBugSmall::exeShakeStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "repel", 0);
    }
    if (MR::isStep(this, 40)) {
        LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvShake::sInstance);
    }
}
void StinkBugSmall::exePanic() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Loss", 0);
        MR::invalidateHitSensor(this, "head");
    }
    MR::startLevelSound(this, "SE_EV_LV_STINKBUG_S_PANIC", -1, -1, 15);
    MR::startLevelSound(this, "SE_EV_LV_STINKBUG_S_PANIC", -1, -1, -1);
    this->mVelocity.zero();
    if (MR::isOnPlayer(LiveActor::getSensor("body"))) {
        if (MR::isStep(this, 90)) {
            LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvShakeStart::sInstance);
        }
    } else {
        LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvRecover::sInstance);
    }
}
void StinkBugSmall::exeRecover() {
    this->mVelocity.zero();
    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_S_PANID", -1, -1, -1);
    if (MR::isStep(this, 60)) {
        LiveActor::setNerve(&NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance);
    }
}
void StinkBugSmall::exeDPDSwoon() {
    if (MR::isFirstStep(this)) {
        MR::deleteEffectAll(this);
    }
    if (MR::isEffectValid(this, "RushSmoke")) {
        MR::deleteEffect(this, "RushSmoke");
    }
    MR::updateActorStateAndNextNerve(this, mStateBaseInterface, &NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance);
}
void StinkBugSmall::exeForceFall() {
    if (MR::isFirstStep(this)) {
        this->mVelocity.zero();
        MR::calcGravity(this);
        MR::onBind(this);
    }
    JMAVECScaleAdd(this->mGravity, this->mVelocity, this->mVelocity, 2.0f);
}
