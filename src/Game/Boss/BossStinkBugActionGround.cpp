#include "Game/Boss/BossStinkBugActionGround.hpp"
#include "Game/Boss/BossStinkBug.hpp"
#include "Game/Boss/BossStinkBugActionBase.hpp"
#include "Game/Boss/BossStinkBugFunction.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

namespace NrvBossStinkBugGroundBattle {
    NEW_NERVE(BossStinkBugActionGroundNrvStart, BossStinkBugActionGround, Start);
    NEW_NERVE(BossStinkBugActionGroundNrvWalk, BossStinkBugActionGround, Walk);
    NEW_NERVE(BossStinkBugActionGroundNrvDamage, BossStinkBugActionGround, Damage);
    NEW_NERVE(BossStinkBugActionGroundNrvFind, BossStinkBugActionGround, Find);
    NEW_NERVE(BossStinkBugActionGroundNrvShakeOff, BossStinkBugActionGround, ShakeOff);
    NEW_NERVE(BossStinkBugActionGroundNrvShakeOffRun, BossStinkBugActionGround, ShakeOffRun);
    NEW_NERVE(BossStinkBugActionGroundNrvRun, BossStinkBugActionGround, Run);
    NEW_NERVE_ONEND(BossStinkBugActionGroundNrvAttackSuccess, BossStinkBugActionGround, AttackSuccess, AttackSuccess);
    NEW_NERVE(BossStinkBugActionGroundNrvCoolDown, BossStinkBugActionGround, CoolDown);
    NEW_NERVE(BossStinkBugActionGroundNrvTurn, BossStinkBugActionGround, Turn);
};  // namespace NrvBossStinkBugGroundBattle

BossStinkBugActionGround::BossStinkBugActionGround(BossStinkBug* pStinkBug) : BossStinkBugActionBase("ボスカメムシ地上戦", pStinkBug) {
    initNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvWalk::sInstance);
}

void BossStinkBugActionGround::appear() {
    MR::moveCoordToNearestPos(getHost(), getHost()->mPosition);
    MR::offBind(getHost());
    getHost()->validateCollisionGround();
    MR::startBck(getHost()->getBombLauncher(), "Close", nullptr);
    MR::tryStartAllAnim(getHost()->getWingModel(), "Ground");
    ActorStateBase::appear();
    setNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvStart::sInstance);
    getHost()->onBindLeg();
}

void BossStinkBugActionGround::control() {
    getHost()->updatePose();
    MR::updateEffectFloorCodeLineToMap(getHost(), 1000.0f, -200.0f);
    updateSound();
}

bool BossStinkBugActionGround::isValidFollowId(s32 id) const {
    return id == 0;
}

void BossStinkBugActionGround::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
}

bool BossStinkBugActionGround::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerHipDropFloor(msg) && isEnableHipDrop()) {
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormal();
        MR::emitEffectHit(getHost(), MR::getSensorPos(pSender), "Hit");
        BossStinkBugFunction::invalidateAttack(getHost());
        MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_1", -1, -1);
        setNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvDamage::sInstance);
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionGround::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (getHost()->isSensorBody(pReceiver)) {
        if (isEnableShakeStart() && MR::isMsgFloorTouch(msg)) {
            setNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvShakeOff::sInstance);
            return true;
        }
    } else if (MR::isMsgWallTouch(msg) || MR::isMsgCeilTouch(msg)) {
        if (isNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvRun::sInstance) ||
            isNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvShakeOffRun::sInstance)) {
            if (MR::isFaceToTargetDegree(getHost(), MR::getSensorPos(pSender), getHost()->_EC, 45.0f)) {
                TVec3f railPosFrontCoord;
                MR::calcRailPosFrontCoord(&railPosFrontCoord, getHost(), 2000.0f);
                if (MR::sendMsgEnemyAttackMaximumToDir(pSender, pReceiver, railPosFrontCoord - getHost()->mPosition)) {
                    getHost()->invalidateCollision();
                    setNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvAttackSuccess::sInstance);
                    return true;
                }
            }
        }
    }
    return false;
}

void BossStinkBugActionGround::exeStart() {
    if (updateGroundRegainRail(5.0f)) {
        setNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvWalk::sInstance);
    }
}

void BossStinkBugActionGround::exeWalk() {
    updateGroundWalk(5.0f);
    f32 distToPlayer = MR::calcDistanceToPlayer(getHost());
    if (500.0f <= distToPlayer && distToPlayer <= 1900.0f && MR::isFaceToPlayerDegreeHV(getHost(), getHost()->_EC, 45.0f, 15.0f)) {
        setNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvFind::sInstance);
    } else if (BossStinkBugFunction::isExistPlayerBack(getHost(), 2500.0f)) {
        setNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvTurn::sInstance);
    }
}

void BossStinkBugActionGround::exeDamage() {
    if (updateGroundDamage()) {
        kill();
    }
}

void BossStinkBugActionGround::exeFind() {
    if (updateGroundFind()) {
        setNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvRun::sInstance);
    }
}

void BossStinkBugActionGround::exeShakeOff() {
    if (updateGroundShakeOff()) {
        setNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvShakeOffRun::sInstance);
    }
}

void BossStinkBugActionGround::exeShakeOffRun() {
    if (updateGroundRun(20.0f)) {
        setNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvCoolDown::sInstance);
    }
}

void BossStinkBugActionGround::exeRun() {
    if (updateGroundRun(20.0f)) {
        setNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvCoolDown::sInstance);
    }
}

void BossStinkBugActionGround::endAttackSuccess() {
    getHost()->validateCollisionGround();
}

void BossStinkBugActionGround::exeAttackSuccess() {
    if (updateGroundAttackSuccess()) {
        setNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvWalk::sInstance);
    }
}

void BossStinkBugActionGround::exeCoolDown() {
    if (updateGroundCoolDown()) {
        if (MR::isFaceToPlayerHorizontalDegree(getHost(), getHost()->_EC, 90.0f)) {
            setNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvWalk::sInstance);
        } else {
            setNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvTurn::sInstance);
        }
    }
}

void BossStinkBugActionGround::exeTurn() {
    if (updateGroundTurn()) {
        setNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvWalk::sInstance);
    }
}

bool BossStinkBugActionGround::isEnableHipDrop() const {
    if (isNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvWalk::sInstance) ||
        isNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvFind::sInstance) ||
        isNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvShakeOff::sInstance) ||
        isNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvShakeOffRun::sInstance) ||
        isNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvRun::sInstance) ||
        isNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvTurn::sInstance) ||
        isNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvCoolDown::sInstance)) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionGround::isEnableShakeStart() const {
    if (isNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvWalk::sInstance) ||
        isNerve(&NrvBossStinkBugGroundBattle::BossStinkBugActionGroundNrvRun::sInstance)) {
        return true;
    } else {
        return false;
    }
}
