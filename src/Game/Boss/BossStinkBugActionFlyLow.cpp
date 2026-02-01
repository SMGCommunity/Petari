#include "Game/Boss/BossStinkBugActionFlyLow.hpp"
#include "Game/Boss/BossStinkBug.hpp"
#include "Game/Boss/BossStinkBugActionBase.hpp"
#include "Game/Boss/BossStinkBugFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include "revolution/types.h"

namespace NrvBossStinkBugActionFlyLow {
    NEW_NERVE(BossStinkBugActionFlyLowNrvFly, BossStinkBugActionFlyLow, Fly);
    NEW_NERVE(BossStinkBugActionFlyLowNrvShakeOffSign, BossStinkBugActionFlyLow, ShakeOffSign);
    NEW_NERVE(BossStinkBugActionFlyLowNrvShakeOff, BossStinkBugActionFlyLow, ShakeOff);
    NEW_NERVE(BossStinkBugActionFlyLowNrvShakeOffWait, BossStinkBugActionFlyLow, ShakeOffWait);
    NEW_NERVE(BossStinkBugActionFlyLowNrvShakeOffEnd, BossStinkBugActionFlyLow, ShakeOffEnd);
    NEW_NERVE(BossStinkBugActionFlyLowNrvFall, BossStinkBugActionFlyLow, Fall);
    NEW_NERVE(BossStinkBugActionFlyLowNrvDamage, BossStinkBugActionFlyLow, Damage);
    NEW_NERVE(BossStinkBugActionFlyLowNrvDash, BossStinkBugActionFlyLow, Dash);
    NEW_NERVE(BossStinkBugActionFlyLowNrvTurnSign, BossStinkBugActionFlyLow, TurnSign);
    NEW_NERVE(BossStinkBugActionFlyLowNrvTurn, BossStinkBugActionFlyLow, Turn);
}  // namespace NrvBossStinkBugActionFlyLow

BossStinkBugActionFlyLow::BossStinkBugActionFlyLow(BossStinkBug* pStinkBug) : BossStinkBugActionBase("ボスカメムシ低空戦", pStinkBug), _64(0.0f) {
    _38 = 14.0f;
    _3C = 900.0f;
    _44 = -400.0f;
    initNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvFly::sInstance);
}

void BossStinkBugActionFlyLow::appear() {
    MR::moveCoordToNearestPos(getHost(), getHost()->mPosition);

    if (MR::isRailGoingToEnd(getHost())) {
        MR::reverseRailDirection(getHost());
    }

    _64 = 0.0f;
    _3C = 900.0f;
    BossStinkBugActionBase::appear();
    setNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvDash::sInstance);
    MR::offBind(getHost());
    getHost()->validateCollisionFly();
    MR::startBck(getHost()->getBombLauncher(), "Open", nullptr);
    MR::tryStartAllAnim(getHost()->getWingModel(), "Fly");
}

void BossStinkBugActionFlyLow::control() {
    if (!isNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvFall::sInstance)) {
        MR::zeroVelocity(getHost());
    }
    updateSound();
    updateSoundFly();
}

bool BossStinkBugActionFlyLow::isValidFollowId(s32 id) const {
    return id == 1;
}

bool BossStinkBugActionFlyLow::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerHipDropFloor(msg) && isEnableHipDrop()) {
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormal();
        MR::emitEffectHit(getHost(), MR::getSensorPos(pSender), "Hit");

        if (BossStinkBugFunction::isHipDropableSensor(getHost(), pReceiver)) {
            MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_2", -1, -1);
            setNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvFall::sInstance);
            return true;
        }
    }

    return false;
}

bool BossStinkBugActionFlyLow::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (getHost()->isSensorBody(pReceiver)) {
        if (isNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvFly::sInstance) && MR::isMsgFloorTouch(msg)) {
            setNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvShakeOffSign::sInstance);
            return true;
        }
    } else if (isEnableAttack() && (MR::isMsgWallTouch(msg) || MR::isMsgCeilTouch(msg))) {
    }
    return false;
}

void BossStinkBugActionFlyLow::exeFly() {
    _64 += 0.05235988f;
    MR::repeatDegree(&_64);
    _3C = 900.0f + JMath::sSinCosTable.sinLapRad(_64) * 100.0f;
    throwBomb(60);
    updateFly();
    if (BossStinkBugFunction::isExistPlayerBack(getHost(), 2000.0f)) {
        setNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvTurnSign::sInstance);
    }
}

void BossStinkBugActionFlyLow::exeShakeOffSign() {
    if (updateFlyShakeOffSign(120)) {
        setNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvShakeOff::sInstance);
    }
}

void BossStinkBugActionFlyLow::exeShakeOff() {
    if (updateFlyShakeOff()) {
        setNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvShakeOffWait::sInstance);
    }
}

void BossStinkBugActionFlyLow::exeShakeOffWait() {
    if (updateFlyShakeOffWait(30)) {
        setNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvShakeOffEnd::sInstance);
    }
}

void BossStinkBugActionFlyLow::exeShakeOffEnd() {
    if (updateFlyShakeOffEnd()) {
        setNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvFly::sInstance);
    }
}

void BossStinkBugActionFlyLow::exeFall() {
    if (updateFall()) {
        setNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvDamage::sInstance);
    }
}

void BossStinkBugActionFlyLow::exeDamage() {
    if (updateFallDamage()) {
        kill();
    }
}

void BossStinkBugActionFlyLow::exeDash() {
    if (updateFlyDash(15.0f)) {
        setNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvFly::sInstance);
    }
}

void BossStinkBugActionFlyLow::exeTurnSign() {
    if (updateFlyTurnSign()) {
        setNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvTurn::sInstance);
    }
}

void BossStinkBugActionFlyLow::exeTurn() {
    if (updateFlyTurn()) {
        setNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvFly::sInstance);
    }
}

bool BossStinkBugActionFlyLow::isEnableHipDrop() const {
    return isNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvFly::sInstance) ||
           isNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvShakeOffSign::sInstance) ||
           isNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvShakeOff::sInstance) ||
           isNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvShakeOffEnd::sInstance) ||
           isNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvTurnSign::sInstance) ||
           isNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvTurn::sInstance);
}

bool BossStinkBugActionFlyLow::isEnableAttack() const {
    if (isNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvFall::sInstance) ||
        isNerve(&NrvBossStinkBugActionFlyLow::BossStinkBugActionFlyLowNrvDamage::sInstance)) {
        return false;
    } else {
        return true;
    }
}
