#include "Game/Enemy/StinkBugSmall.hpp"
#include "Game/Enemy/StinkBugBase.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
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

namespace NrvStinkBugSmall {
    NEW_NERVE(StinkBugSmallNrvWait, StinkBugSmall, Wait);
    NEW_NERVE(StinkBugSmallNrvSearch, StinkBugSmall, Search);
    NEW_NERVE(StinkBugSmallNrvDashSign, StinkBugSmall, DashSign);
    NEW_NERVE(StinkBugSmallNrvDashSignEnd, StinkBugSmall, DashSignEnd);
    NEW_NERVE(StinkBugSmallNrvDash, StinkBugSmall, Dash);
    NEW_NERVE(StinkBugSmallNrvDashEnd, StinkBugSmall, DashEnd);
    NEW_NERVE(StinkBugSmallNrvBack, StinkBugSmall, Back);
    NEW_NERVE(StinkBugSmallNrvHipDropDown, StinkBugSmall, HipDropDown);
    NEW_NERVE(StinkBugSmallNrvAttack, StinkBugSmall, Attack);
    NEW_NERVE(StinkBugSmallNrvSpinReaction, StinkBugSmall, SpinReaction);
    NEW_NERVE(StinkBugSmallNrvShakeStart, StinkBugSmall, ShakeStart);
    NEW_NERVE(StinkBugSmallNrvShake, StinkBugSmall, Shake);
    NEW_NERVE(StinkBugSmallNrvPanic, StinkBugSmall, Panic);
    NEW_NERVE(StinkBugSmallNrvRecover, StinkBugSmall, Recover);
    NEW_NERVE(StinkBugSmallNrvDPDSwoon, StinkBugSmall, DPDSwoon);
    NEW_NERVE(StinkBugSmallNrvForceFall, StinkBugSmall, ForceFall);
}  // namespace NrvStinkBugSmall

StinkBugSmall::StinkBugSmall(const char* pName)
    : StinkBugBase(pName), mScaleController(nullptr), mBindStarPointer(nullptr), mStateBaseInterface(nullptr) {
}
StinkBugSmall::~StinkBugSmall() {
}

bool StinkBugSmall::tryForceFall() {
    if (!MR::isValidSwitchA(this) || !MR::isOnSwitchA(this)) {
        return false;
    }
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDPDSwoon::sInstance)) {
        return false;
    }
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvForceFall::sInstance)) {
        return false;
    }
    setNerve(&NrvStinkBugSmall::StinkBugSmallNrvForceFall::sInstance);
    return true;
}

bool StinkBugSmall::tryDPDSwoon() {
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance)) {
        return false;
    }
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDPDSwoon::sInstance)) {
        return false;
    }

    if (!mBindStarPointer->tryStartPointBind()) {
        return false;
    }
    setNerve(&NrvStinkBugSmall::StinkBugSmallNrvDPDSwoon::sInstance);
    return true;
}

bool StinkBugSmall::isShakeChance() const {
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance)) {
        return false;
    }
    if (!isNerve(&NrvStinkBugSmall::StinkBugSmallNrvShake::sInstance)) {
        return false;
    }
    if (MR::isPlayerHipDropFalling()) {
        return false;
    }
    return MR::isPlayerHipDropLand() == false;
}

void StinkBugSmall::kill() {
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
    if (!MR::isValidSwitchDead(this)) {
        MR::appearStarPiece(this, mPosition, 3, 10.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
    }
    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
    kill();
}

void StinkBugSmall::exeHipDropDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Death", nullptr);
        MR::startBrk(this, "Death");
        if (!_C4) {
            MR::invalidateCollisionParts(this);
        }
        MR::invalidateHitSensors(this);
        MR::invalidateClipping(this);
        MR::startSound(this, "SE_EM_STOMPED_S", -1, -1);
        MR::startSound(this, "SE_EV_STINKBUG_S_STOMPED", -1, -1);
    }
    mVelocity.zero();
    if (MR::isBckStopped(this)) {
        return kill();
    }
}

bool StinkBugSmall::isNrvEnableStarPieceAttack() const {
    return isNerve(&NrvStinkBugSmall::StinkBugSmallNrvWait::sInstance) || isNerve(&NrvStinkBugSmall::StinkBugSmallNrvSearch::sInstance) ||
           isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashSign::sInstance) || isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashSignEnd::sInstance) ||
           isNerve(&NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance);
}

bool StinkBugSmall::receiveOtherMsg(u32 msg, HitSensor* pSendder, HitSensor* pReceiver) {
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance)) {
        return false;
    }
    if (MR::isPlayerElementModeInvincible() && (MR::isMsgFloorTouch(msg) || MR::isMsgWallTouch(msg) || MR::isMsgCeilTouch(msg))) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance);
        return true;
    }
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDPDSwoon::sInstance)) {
        return false;
    }
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvPanic::sInstance)) {
        return false;
    }
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvShakeStart::sInstance)) {
        return false;
    }

    if (MR::isMsgFloorTouch(msg)) {
        if (MR::isOnPlayer(getSensor("body"))) {
            if (StinkBugSmall::isShakeChance()) {
                MR::sendMsgEnemyAttackFlip(pReceiver, pSendder);
            } else {
                setNerve(&NrvStinkBugSmall::StinkBugSmallNrvPanic::sInstance);
                return true;
            }
        }
    }
    return false;
}

bool StinkBugSmall::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance))
        return false;
    if (getSensor("body"))
        return false;
    if (MR::isMsgInvincibleAttack(msg) || MR::isMsgPlayerHipDropFloor(msg) || MR::isMsgPlayerHipDrop(msg)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance);
        return true;
    }
    if (!isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDPDSwoon::sInstance) && !MR::isMsgLockOnStarPieceShoot(msg)) {
        if (MR::isMsgStarPieceReflect(msg) && getSensor("body")) {
            if (StinkBugSmall::isNrvEnableStarPieceAttack()) {
                setNerve(&NrvStinkBugSmall::StinkBugSmallNrvSpinReaction::sInstance);
                return true;
            }
        }
        if (!isNerve(&NrvStinkBugSmall::StinkBugSmallNrvSpinReaction::sInstance) && !isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDash::sInstance) &&
            MR::isMsgPlayerSpinAttack(msg) && getSensor("body")) {
            setNerve(&NrvStinkBugSmall::StinkBugSmallNrvSpinReaction::sInstance);
            return true;
        }
    }
    return false;
}

void StinkBugSmall::exeWait() {
    if (MR::isFirstStep(this) && mScale.x != 0.0f) {
        MR::tryStartBck(this, "Search", nullptr);
    }
    fixInitPos();
    if (isPlayerInTerritory(400.0f, 600.0f, 200.0f, 200.0f)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashSign::sInstance);
    } else if (mScale.x != 0.0f) {
        if (MR::isGreaterStep(this, 1)) {
            setNerve(&NrvStinkBugSmall::StinkBugSmallNrvSearch::sInstance);
        }
    }
}
void StinkBugSmall::exeSearch() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Search", nullptr);
    }
    fixInitPos();
    if (tryTurnSearch(1.0f)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvWait::sInstance);
    } else if (isPlayerInTerritory(400.0f, 600.0f, 200.0f, 200.0f)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashSign::sInstance);
    }
}
void StinkBugSmall::exeDashSign() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "RushStart", nullptr);
        MR::startSound(this, "SE_EV_STINKBUG_S_FIND", -1, -1);
    }
    MR::startLevelSound(this, "SE_EM_STINKBUG_S_DASH_SIGN", -1, -1, -1);
    fixInitPos();
    tryTurnDashSign(3.0f);
    if (MR::isBckStopped(this)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashSignEnd::sInstance);
    }
}
void StinkBugSmall::exeDash() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Rush", nullptr);
        MR::validateHitSensors(this);
    }
    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_S_DASH", -1, -1, -1);
    if (MR::isNear(this, mPosition, mScale.y) && !MR::isBindedWall(this)) {
        return setDashVelocity(20.0f);
    }
    mVelocity.zero();
    setNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashEnd::sInstance);
    return MR::startSound(this, "SE_EM_STINKBUG_S_DASH_END", -1, -1);
}
void StinkBugSmall::exeDashEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "RushStop", nullptr);
    }
    mVelocity.zero();
    if (MR::isBckStopped(this)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance);
    }
}
void StinkBugSmall::exeBack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Back", nullptr);
        MR::invalidateHitSensor(this, "head");
    }
    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_S_BACK", -1, -1, -1);
    if (MR::isNear(this, mPosition, 10.0f)) {
        return setNerve(&NrvStinkBugSmall::StinkBugSmallNrvWait::sInstance);
    }
    TVec3f tvf;
    MR::normalize(&tvf);
    MR::turnVecToPlane(&tvf, tvf, mGravity);
    mVelocity.scale(5.0f);
}
void StinkBugSmall::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Attack", nullptr);
    }
    mVelocity.zero();
    if (MR::isBckStopped(this)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance);
    }
}
void StinkBugSmall::exeSpinReaction() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SpinAction", nullptr);
    }
    MR::startSound(this, "SE_EM_GUARD_S", -1, -1);
    mVelocity.zero();
    if (MR::isBckStopped(this)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance);
    }
}
void StinkBugSmall::exeShakeStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "repel", nullptr);
    }
    if (MR::isStep(this, 40)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvShake::sInstance);
    }
}
void StinkBugSmall::exePanic() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Loss", nullptr);
        MR::invalidateHitSensor(this, "head");
    }
    MR::startLevelSound(this, "SE_EV_LV_STINKBUG_S_PANIC", -1, -1, 15);
    MR::startLevelSound(this, "SE_EV_LV_STINKBUG_S_PANIC", -1, -1, -1);
    mVelocity.zero();
    if (MR::isOnPlayer(getSensor("body"))) {
        if (MR::isStep(this, 90)) {
            setNerve(&NrvStinkBugSmall::StinkBugSmallNrvShakeStart::sInstance);
        }
    } else {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvRecover::sInstance);
    }
}
void StinkBugSmall::exeRecover() {
    mVelocity.zero();
    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_S_PANID", -1, -1, -1);
    if (MR::isStep(this, 60)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance);
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
        mVelocity.zero();
        MR::calcGravity(this);
        MR::onBind(this);
    }
    JMAVECScaleAdd(mGravity, mVelocity, mVelocity, 2.0f);
}
