#include "Game/Boss/BossStinkBugActionGround.hpp"
#include "Game/Boss/BossStinkBug.hpp"
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

namespace {
    static const f32 sWalkSpeed = 5.0f;
    static const f32 sFindDegreeH = 45.0f;
    static const f32 sFindDegreeV = 15.0f;
    static const f32 sFindMinDistance = 500.0f;
    static const f32 sFindMaxDistance = 1900.0f;
    static const f32 sRunSpeed = 20.0f;
    static const f32 sTurnStartDistance = 2500.0f;
};  // namespace

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
    initNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvWalk));
}

void BossStinkBugActionGround::appear() {
    MR::moveCoordToNearestPos(getHost(), getHost()->mPosition);
    MR::offBind(getHost());
    getHost()->validateCollisionGround();
    MR::startBck(getHost()->getBombLauncher(), "Close");
    MR::tryStartAllAnim(getHost()->getWingModel(), "Ground");
    ActorStateBase::appear();
    setNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvStart));
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
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::shakeCameraNormal();
        MR::emitEffectHit(getHost(), MR::getSensorPos(pSender), "Hit");
        BossStinkBugFunction::invalidateAttack(getHost());
        MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_1");
        setNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvDamage));

        return true;
    }

    return false;
}

bool BossStinkBugActionGround::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (getHost()->isSensorBody(pReceiver)) {
        if (isEnableShakeStart() && MR::isMsgFloorTouch(msg)) {
            setNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvShakeOff));

            return true;
        }
    } else if (MR::isMsgWallTouch(msg) || MR::isMsgCeilTouch(msg)) {
        if (isNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvRun)) ||
            isNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvShakeOffRun))) {
            if (MR::isFaceToTargetDegree(getHost(), MR::getSensorPos(pSender), getHost()->_EC, 45.0f)) {
                TVec3f railPosFrontCoord;
                MR::calcRailPosFrontCoord(&railPosFrontCoord, getHost(), 2000.0f);

                if (MR::sendMsgEnemyAttackMaximumToDir(pSender, pReceiver, railPosFrontCoord - getHost()->mPosition)) {
                    getHost()->invalidateCollision();
                    setNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvAttackSuccess));

                    return true;
                }
            }
        }
    }

    return false;
}

void BossStinkBugActionGround::exeStart() {
    if (updateGroundRegainRail(::sWalkSpeed)) {
        setNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvWalk));
    }
}

void BossStinkBugActionGround::exeWalk() {
    updateGroundWalk(::sWalkSpeed);

    f32 findDistance = MR::calcDistanceToPlayer(getHost());

    if (::sFindMinDistance <= findDistance && findDistance <= ::sFindMaxDistance &&
        MR::isFaceToPlayerDegreeHV(getHost(), getHost()->_EC, ::sFindDegreeH, ::sFindDegreeV)) {
        setNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvFind));
    } else if (BossStinkBugFunction::isExistPlayerBack(getHost(), ::sTurnStartDistance)) {
        setNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvTurn));
    }
}

void BossStinkBugActionGround::exeDamage() {
    if (updateGroundDamage()) {
        kill();
    }
}

void BossStinkBugActionGround::exeFind() {
    if (updateGroundFind()) {
        setNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvRun));
    }
}

void BossStinkBugActionGround::exeShakeOff() {
    if (updateGroundShakeOff()) {
        setNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvShakeOffRun));
    }
}

void BossStinkBugActionGround::exeShakeOffRun() {
    if (updateGroundRun(::sRunSpeed)) {
        setNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvCoolDown));
    }
}

void BossStinkBugActionGround::exeRun() {
    if (updateGroundRun(::sRunSpeed)) {
        setNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvCoolDown));
    }
}

void BossStinkBugActionGround::endAttackSuccess() {
    getHost()->validateCollisionGround();
}

void BossStinkBugActionGround::exeAttackSuccess() {
    if (updateGroundAttackSuccess()) {
        setNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvWalk));
    }
}

void BossStinkBugActionGround::exeCoolDown() {
    if (updateGroundCoolDown()) {
        if (MR::isFaceToPlayerHorizontalDegree(getHost(), getHost()->_EC, 90.0f)) {
            setNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvWalk));
        } else {
            setNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvTurn));
        }
    }
}

void BossStinkBugActionGround::exeTurn() {
    if (updateGroundTurn()) {
        setNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvWalk));
    }
}

bool BossStinkBugActionGround::isEnableHipDrop() const {
    if (isNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvWalk)) ||
        isNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvFind)) ||
        isNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvShakeOff)) ||
        isNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvShakeOffRun)) ||
        isNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvRun)) ||
        isNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvTurn)) ||
        isNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvCoolDown))) {
        return true;
    }

    return false;
}

bool BossStinkBugActionGround::isEnableShakeStart() const {
    if (isNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvWalk)) ||
        isNerve(GET_NERVE(BossStinkBugGroundBattle, BossStinkBugActionGroundNrvRun))) {
        return true;
    }

    return false;
}
