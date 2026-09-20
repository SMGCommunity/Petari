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
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <revolution/types.h>

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
};  // namespace NrvBossStinkBugActionFlyLow

BossStinkBugActionFlyLow::BossStinkBugActionFlyLow(BossStinkBug* pStinkBug) : BossStinkBugActionBase("ボスカメムシ低空戦", pStinkBug), _64(0.0f) {
    _38 = 14.0f;
    _3C = 900.0f;
    _44 = -400.0f;
    initNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvFly));
}

void BossStinkBugActionFlyLow::appear() {
    MR::moveCoordToNearestPos(getHost(), getHost()->mPosition);

    if (MR::isRailGoingToEnd(getHost())) {
        MR::reverseRailDirection(getHost());
    }

    _64 = 0.0f;
    _3C = 900.0f;
    BossStinkBugActionBase::appear();
    setNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvDash));
    MR::offBind(getHost());
    getHost()->validateCollisionFly();
    MR::startBck(getHost()->getBombLauncher(), "Open");
    MR::tryStartAllAnim(getHost()->getWingModel(), "Fly");
}

void BossStinkBugActionFlyLow::control() {
    if (!isNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvFall))) {
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
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::shakeCameraNormal();
        MR::emitEffectHit(getHost(), MR::getSensorPos(pSender), "Hit");

        if (BossStinkBugFunction::isHipDropableSensor(getHost(), pReceiver)) {
            MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_2");
            setNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvFall));
            return true;
        }
    }

    return false;
}

bool BossStinkBugActionFlyLow::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (getHost()->isSensorBody(pReceiver)) {
        if (isNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvFly)) && MR::isMsgFloorTouch(msg)) {
            setNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvShakeOffSign));
            return true;
        }
    } else if (isEnableAttack() && (MR::isMsgWallTouch(msg) || MR::isMsgCeilTouch(msg))) {
    }
    return false;
}

void BossStinkBugActionFlyLow::exeFly() {
    _64 += 0.05235988f;
    MR::repeatDegree(&_64);
    _3C = 900.0f + MR::sin(_64) * 100.0f;
    throwBomb(60);
    updateFly();
    if (BossStinkBugFunction::isExistPlayerBack(getHost(), 2000.0f)) {
        setNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvTurnSign));
    }
}

void BossStinkBugActionFlyLow::exeShakeOffSign() {
    if (updateFlyShakeOffSign(120)) {
        setNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvShakeOff));
    }
}

void BossStinkBugActionFlyLow::exeShakeOff() {
    if (updateFlyShakeOff()) {
        setNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvShakeOffWait));
    }
}

void BossStinkBugActionFlyLow::exeShakeOffWait() {
    if (updateFlyShakeOffWait(30)) {
        setNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvShakeOffEnd));
    }
}

void BossStinkBugActionFlyLow::exeShakeOffEnd() {
    if (updateFlyShakeOffEnd()) {
        setNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvFly));
    }
}

void BossStinkBugActionFlyLow::exeFall() {
    if (updateFall()) {
        setNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvDamage));
    }
}

void BossStinkBugActionFlyLow::exeDamage() {
    if (updateFallDamage()) {
        kill();
    }
}

void BossStinkBugActionFlyLow::exeDash() {
    if (updateFlyDash(15.0f)) {
        setNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvFly));
    }
}

void BossStinkBugActionFlyLow::exeTurnSign() {
    if (updateFlyTurnSign()) {
        setNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvTurn));
    }
}

void BossStinkBugActionFlyLow::exeTurn() {
    if (updateFlyTurn()) {
        setNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvFly));
    }
}

bool BossStinkBugActionFlyLow::isEnableHipDrop() const {
    return isNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvFly)) ||
           isNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvShakeOffSign)) ||
           isNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvShakeOff)) ||
           isNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvShakeOffEnd)) ||
           isNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvTurnSign)) ||
           isNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvTurn));
}

bool BossStinkBugActionFlyLow::isEnableAttack() const {
    if (isNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvFall)) ||
        isNerve(GET_NERVE(BossStinkBugActionFlyLow, BossStinkBugActionFlyLowNrvDamage))) {
        return false;
    } else {
        return true;
    }
}
