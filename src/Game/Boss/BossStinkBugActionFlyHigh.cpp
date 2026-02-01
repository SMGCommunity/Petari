#include "Game/Boss/BossStinkBugActionFlyHigh.hpp"
#include "Game/Boss/BossStinkBug.hpp"
#include "Game/Boss/BossStinkBugActionBase.hpp"
#include "Game/Boss/BossStinkBugFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"

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
}  // namespace NrvBossStinkBugActionFlyHigh

BossStinkBugActionFlyHigh::BossStinkBugActionFlyHigh(BossStinkBug* pStinkBug)
    : BossStinkBugActionBase("ボスカメムシ高空戦", pStinkBug), _68(0), _6C(false), _70(0) {
    _38 = 20.0f;
    _3C = 1000.0f;
    _44 = -400.0f;
    _68 = calcKeepRollTime(60);
    initNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvFly::sInstance);
}

void BossStinkBugActionFlyHigh::appear() {
    MR::moveCoordToNearestPos(getHost(), getHost()->mPosition);

    if (MR::isRailGoingToEnd(getHost())) {
        MR::reverseRailDirection(getHost());
    }

    BossStinkBugActionBase::appear();
    setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvFlyDash::sInstance);
    MR::offBind(getHost());
    getHost()->validateCollisionFly();
    MR::startBck(getHost()->getBombLauncher(), "Open", nullptr);
    MR::tryStartAllAnim(getHost()->getWingModel(), "Fly");
}

void BossStinkBugActionFlyHigh::control() {
    if (!isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvFall::sInstance)) {
        MR::zeroVelocity(getHost());
    }
    updateSound();
    updateSoundFly();
}

bool BossStinkBugActionFlyHigh::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerHipDropFloor(msg) && isEnableHipDrop()) {
        getHost()->offBindLeg();
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormal();
        MR::emitEffectHit(getHost(), MR::getSensorPos(pSender), "Hit");

        if (BossStinkBugFunction::isHipDropableSensor(getHost(), pReceiver)) {
            if (isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvLand::sInstance) ||
                isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvGroundRun::sInstance)) {
                setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvDamage::sInstance);
            } else {
                setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvFall::sInstance);
            }

            MR::startSystemSE("SE_SY_VS_BOSS_LAST_HIT", -1, -1);
            return true;
        }
    }

    return false;
}

bool BossStinkBugActionFlyHigh::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (getHost()->isSensorBody(pReceiver)) {
        if ((isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvFly::sInstance) ||
             isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvRollSign::sInstance) ||
             isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvRollEnd::sInstance)) &&
            MR::isMsgFloorTouch(msg)) {
            setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvShakeOffSign::sInstance);
            return true;
        }
    } else if (isEnableAttack() && (MR::isMsgWallTouch(msg) || MR::isMsgCeilTouch(msg))) {
        if (MR::isFaceToTargetDegree(getHost(), MR::getSensorPos(pSender), getHost()->_EC, 45.0f)) {
            if (MR::sendMsgEnemyAttackStrongToDir(pSender, pReceiver, getHost()->_EC.multInLine(50.0f))) {
                if (isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvGroundRun::sInstance)) {
                    getHost()->validateCollisionHitOnly();
                    setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvToFly::sInstance);
                }
                return true;
            }
        }
    }

    return false;
}

bool BossStinkBugActionFlyHigh::isValidFollowId(s32 id) const {
    return isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvToGround::sInstance) ||
                   isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvLand::sInstance) ||
                   isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvGroundRun::sInstance) ||
                   isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvToFly::sInstance) ?
               id == 0 :
               id == 1;
}

void BossStinkBugActionFlyHigh::exeFlyDash() {
    if (updateFlyDash(15.0f)) {
        setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvFly::sInstance);
    }
}

void BossStinkBugActionFlyHigh::exeFly() {
    throwBomb(40);
    updateFly();
    if (_70 >= 2) {
        if (BossStinkBugFunction::isExistPlayerBack(getHost(), 1500.0f)) {
            setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvTurnSign::sInstance);
            _70 = 0;
        }
    } else if (MR::isGreaterStep(this, _68)) {
        setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvRollSign::sInstance);
        _70++;
    }
}

void BossStinkBugActionFlyHigh::exeShakeOffSign() {
    throwBomb(40);
    if (updateFlyShakeOffSign(120)) {
        setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvShakeOff::sInstance);
    }
}

void BossStinkBugActionFlyHigh::exeShakeOff() {
    throwBomb(40);
    if (updateFlyShakeOff()) {
        setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvShakeOffWait::sInstance);
    }
}

void BossStinkBugActionFlyHigh::exeShakeOffWait() {
    throwBomb(40);
    if (updateFlyShakeOffWait(30)) {
        setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvShakeOffEnd::sInstance);
    }
}

void BossStinkBugActionFlyHigh::exeShakeOffEnd() {
    if (updateFlyShakeOffEnd()) {
        resetThrowBombTiming();
        setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvFly::sInstance);
    }
}

void BossStinkBugActionFlyHigh::exeRollSign() {
    throwBomb(40);
    if (updateFlyRollSign(60)) {
        setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvRoll::sInstance);
    }
}

void BossStinkBugActionFlyHigh::exeRoll() {
    throwBomb(40);
    if (updateFlyRoll()) {
        setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvRollWait::sInstance);
    }
}

void BossStinkBugActionFlyHigh::exeRollWait() {
    throwBomb(40);
    if (updateFlyRollWait(_68)) {
        setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvRollEnd::sInstance);
    }
}

void BossStinkBugActionFlyHigh::exeRollEnd() {
    if (updateFlyRollEnd()) {
        resetThrowBombTiming();
        setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvFly::sInstance);
    }
}

void BossStinkBugActionFlyHigh::exeTurnSign() {
    if (updateFlyTurnSign()) {
        setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvTurn::sInstance);
    }
}

void BossStinkBugActionFlyHigh::exeTurn() {
    if (updateFlyTurn()) {
        setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvFly::sInstance);
    }
}

void BossStinkBugActionFlyHigh::exeFall() {
    if (updateFall()) {
        setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvDamage::sInstance);
    }
}

void BossStinkBugActionFlyHigh::exeToGround() {
    if (updateToGround()) {
        setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvLand::sInstance);
    }
}

void BossStinkBugActionFlyHigh::exeLand() {
    if (updateLand()) {
        setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvGroundRun::sInstance);
    }
}

void BossStinkBugActionFlyHigh::exeGroundRun() {
    if (updateGroundRun(25.0f)) {
        setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvToFly::sInstance);
    }
}

void BossStinkBugActionFlyHigh::exeToFly() {
    if (updateToFly()) {
        setNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvFly::sInstance);
    }
}

void BossStinkBugActionFlyHigh::exeDamage() {
    if (updateFallDamage()) {
        kill();
    }
}

bool BossStinkBugActionFlyHigh::isEnableHipDrop() const {
    return isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvFly::sInstance) ||
           isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvRollSign::sInstance) ||
           isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvRoll::sInstance) ||
           isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvRollEnd::sInstance) ||
           isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvShakeOffSign::sInstance) ||
           isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvShakeOff::sInstance) ||
           isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvShakeOffEnd::sInstance) ||
           isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvTurnSign::sInstance) ||
           isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvToGround::sInstance) ||
           isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvLand::sInstance) ||
           isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvGroundRun::sInstance) ||
           isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvToFly::sInstance) ||
           isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvTurn::sInstance);
}

bool BossStinkBugActionFlyHigh::isEnableAttack() const {
    if (isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvFall::sInstance) ||
        isNerve(&NrvBossStinkBugActionFlyHigh::BossStinkBugActionFlyHighNrvDamage::sInstance)) {
        return false;
    } else {
        return true;
    }
}
