#include "Game/Boss/BossStinkBugActionFlyHigh.hpp"
#include "Game/Boss/BossStinkBug.hpp"
#include "Game/Boss/BossStinkBugActionBase.hpp"
#include "Game/Boss/BossStinkBugFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvBossStinkBugActionFlyHigh {
    NEW_NERVE(BossStinkBugActionFlyHighNrvFlyDash, BossStinkBugActionFlyHigh, FlyDash);
    NEW_NERVE(BossStinkBugActionFlyHighNrvFly, BossStinkBugActionFlyHigh, Fly);
    NEW_NERVE(BossStinkBugActionFlyHighNrvShakeOffSign, BossStinkBugActionFlyHigh, ShakeOffSign);
    NEW_NERVE(BossStinkBugActionFlyHighNrvShakeOff, BossStinkBugActionFlyHigh, ShakeOff);
    NEW_NERVE(BossStinkBugActionFlyHighNrvShakeOffWait, BossStinkBugActionFlyHigh, ShakeOffWait);
    NEW_NERVE(BossStinkBugActionFlyHighNrvShakeOffEnd, BossStinkBugActionFlyHigh, ShakeOffEnd);
    NEW_NERVE(BossStinkBugActionFlyHighNrvRollSign, BossStinkBugActionFlyHigh, RollSign);
    NEW_NERVE(BossStinkBugActionFlyHighNrvRoll, BossStinkBugActionFlyHigh, Roll);
    NEW_NERVE(BossStinkBugActionFlyHighNrvRollWait, BossStinkBugActionFlyHigh, RollWait);
    NEW_NERVE(BossStinkBugActionFlyHighNrvRollEnd, BossStinkBugActionFlyHigh, RollEnd);
    NEW_NERVE(BossStinkBugActionFlyHighNrvTurnSign, BossStinkBugActionFlyHigh, TurnSign);
    NEW_NERVE(BossStinkBugActionFlyHighNrvTurn, BossStinkBugActionFlyHigh, Turn);
    NEW_NERVE(BossStinkBugActionFlyHighNrvFall, BossStinkBugActionFlyHigh, Fall);
    NEW_NERVE(BossStinkBugActionFlyHighNrvToGround, BossStinkBugActionFlyHigh, ToGround);
    NEW_NERVE(BossStinkBugActionFlyHighNrvLand, BossStinkBugActionFlyHigh, Land);
    NEW_NERVE(BossStinkBugActionFlyHighNrvGroundRun, BossStinkBugActionFlyHigh, GroundRun);
    NEW_NERVE(BossStinkBugActionFlyHighNrvToFly, BossStinkBugActionFlyHigh, ToFly);
    NEW_NERVE(BossStinkBugActionFlyHighNrvDamage, BossStinkBugActionFlyHigh, Damage);
};  // namespace NrvBossStinkBugActionFlyHigh

BossStinkBugActionFlyHigh::BossStinkBugActionFlyHigh(BossStinkBug* pStinkBug)
    : BossStinkBugActionBase("ボスカメムシ高空戦", pStinkBug), _68(0), _6C(false), _70(0) {
    _38 = 20.0f;
    _3C = 1000.0f;
    _44 = -400.0f;
    _68 = calcKeepRollTime(60);
    initNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvFly));
}

void BossStinkBugActionFlyHigh::appear() {
    MR::moveCoordToNearestPos(getHost(), getHost()->mPosition);

    if (MR::isRailGoingToEnd(getHost())) {
        MR::reverseRailDirection(getHost());
    }

    BossStinkBugActionBase::appear();
    setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvFlyDash));
    MR::offBind(getHost());
    getHost()->validateCollisionFly();
    MR::startBck(getHost()->getBombLauncher(), "Open", nullptr);
    MR::tryStartAllAnim(getHost()->getWingModel(), "Fly");
}

void BossStinkBugActionFlyHigh::control() {
    if (!isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvFall))) {
        MR::zeroVelocity(getHost());
    }
    updateSound();
    updateSoundFly();
}

bool BossStinkBugActionFlyHigh::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerHipDropFloor(msg) && isEnableHipDrop()) {
        getHost()->offBindLeg();
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::shakeCameraNormal();
        MR::emitEffectHit(getHost(), MR::getSensorPos(pSender), "Hit");

        if (BossStinkBugFunction::isHipDropableSensor(getHost(), pReceiver)) {
            if (isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvLand)) ||
                isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvGroundRun))) {
                setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvDamage));
            } else {
                setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvFall));
            }

            MR::startSystemSE("SE_SY_VS_BOSS_LAST_HIT");
            return true;
        }
    }

    return false;
}

bool BossStinkBugActionFlyHigh::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (getHost()->isSensorBody(pReceiver)) {
        if ((isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvFly)) ||
             isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvRollSign)) ||
             isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvRollEnd))) &&
            MR::isMsgFloorTouch(msg)) {
            setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvShakeOffSign));
            return true;
        }
    } else if (isEnableAttack() && (MR::isMsgWallTouch(msg) || MR::isMsgCeilTouch(msg))) {
        if (MR::isFaceToTargetDegree(getHost(), MR::getSensorPos(pSender), getHost()->_EC, 45.0f)) {
            if (MR::sendMsgEnemyAttackStrongToDir(pSender, pReceiver, getHost()->_EC * 50.0f)) {
                if (isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvGroundRun))) {
                    getHost()->validateCollisionHitOnly();
                    setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvToFly));
                }
                return true;
            }
        }
    }

    return false;
}

bool BossStinkBugActionFlyHigh::isValidFollowId(s32 id) const {
    return isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvToGround)) ||
                   isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvLand)) ||
                   isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvGroundRun)) ||
                   isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvToFly)) ?
               id == 0 :
               id == 1;
}

void BossStinkBugActionFlyHigh::exeFlyDash() {
    if (updateFlyDash(15.0f)) {
        setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvFly));
    }
}

void BossStinkBugActionFlyHigh::exeFly() {
    throwBomb(40);
    updateFly();
    if (_70 >= 2) {
        if (BossStinkBugFunction::isExistPlayerBack(getHost(), 1500.0f)) {
            setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvTurnSign));
            _70 = 0;
        }
    } else if (MR::isGreaterStep(this, _68)) {
        setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvRollSign));
        _70++;
    }
}

void BossStinkBugActionFlyHigh::exeShakeOffSign() {
    throwBomb(40);
    if (updateFlyShakeOffSign(120)) {
        setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvShakeOff));
    }
}

void BossStinkBugActionFlyHigh::exeShakeOff() {
    throwBomb(40);
    if (updateFlyShakeOff()) {
        setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvShakeOffWait));
    }
}

void BossStinkBugActionFlyHigh::exeShakeOffWait() {
    throwBomb(40);
    if (updateFlyShakeOffWait(30)) {
        setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvShakeOffEnd));
    }
}

void BossStinkBugActionFlyHigh::exeShakeOffEnd() {
    if (updateFlyShakeOffEnd()) {
        resetThrowBombTiming();
        setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvFly));
    }
}

void BossStinkBugActionFlyHigh::exeRollSign() {
    throwBomb(40);
    if (updateFlyRollSign(60)) {
        setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvRoll));
    }
}

void BossStinkBugActionFlyHigh::exeRoll() {
    throwBomb(40);
    if (updateFlyRoll()) {
        setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvRollWait));
    }
}

void BossStinkBugActionFlyHigh::exeRollWait() {
    throwBomb(40);
    if (updateFlyRollWait(_68)) {
        setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvRollEnd));
    }
}

void BossStinkBugActionFlyHigh::exeRollEnd() {
    if (updateFlyRollEnd()) {
        resetThrowBombTiming();
        setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvFly));
    }
}

void BossStinkBugActionFlyHigh::exeTurnSign() {
    if (updateFlyTurnSign()) {
        setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvTurn));
    }
}

void BossStinkBugActionFlyHigh::exeTurn() {
    if (updateFlyTurn()) {
        setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvFly));
    }
}

void BossStinkBugActionFlyHigh::exeFall() {
    if (updateFall()) {
        setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvDamage));
    }
}

void BossStinkBugActionFlyHigh::exeToGround() {
    if (updateToGround()) {
        setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvLand));
    }
}

void BossStinkBugActionFlyHigh::exeLand() {
    if (updateLand()) {
        setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvGroundRun));
    }
}

void BossStinkBugActionFlyHigh::exeGroundRun() {
    if (updateGroundRun(25.0f)) {
        setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvToFly));
    }
}

void BossStinkBugActionFlyHigh::exeToFly() {
    if (updateToFly()) {
        setNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvFly));
    }
}

void BossStinkBugActionFlyHigh::exeDamage() {
    if (updateFallDamage()) {
        kill();
    }
}

bool BossStinkBugActionFlyHigh::isEnableHipDrop() const {
    return isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvFly)) ||
           isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvRollSign)) ||
           isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvRoll)) ||
           isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvRollEnd)) ||
           isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvShakeOffSign)) ||
           isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvShakeOff)) ||
           isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvShakeOffEnd)) ||
           isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvTurnSign)) ||
           isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvToGround)) ||
           isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvLand)) ||
           isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvGroundRun)) ||
           isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvToFly)) ||
           isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvTurn));
}

bool BossStinkBugActionFlyHigh::isEnableAttack() const {
    if (isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvFall)) ||
        isNerve(GET_NERVE(BossStinkBugActionFlyHigh, BossStinkBugActionFlyHighNrvDamage))) {
        return false;
    } else {
        return true;
    }
}
