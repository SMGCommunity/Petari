#include "Game/Boss/DinoPackunBattleVs2Lv1.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/Boss/DinoPackunBall.hpp"
#include "Game/Boss/DinoPackunFire.hpp"
#include "Game/Boss/DinoPackunStateAwake.hpp"
#include "Game/Boss/DinoPackunStateDamage.hpp"
#include "Game/Boss/DinoPackunStateFire.hpp"
#include "Game/Boss/DinoPackunTail.hpp"
#include "Game/Boss/DinoPackunTrackFire.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LiveActorUtil.hpp"

namespace {
    static TVec3f sShotMouthFireOffset = TVec3f(150.0f, 150.0f, 0.0f);
};

namespace NrvDinoPackunBattleVs2Lv1 {
    NEW_NERVE(DinoPackunBattleVs2Lv1NrvTurn, DinoPackunBattleVs2Lv1, Turn);
    NEW_NERVE_ONEND(DinoPackunBattleVs2Lv1NrvWalk, DinoPackunBattleVs2Lv1, Walk, Walk);
    NEW_NERVE(DinoPackunBattleVs2Lv1NrvFindCrazy, DinoPackunBattleVs2Lv1, FindCrazy);
    NEW_NERVE_ONEND(DinoPackunBattleVs2Lv1NrvCrazy, DinoPackunBattleVs2Lv1, Crazy, Crazy);
    NEW_NERVE_ONEND(DinoPackunBattleVs2Lv1NrvChase, DinoPackunBattleVs2Lv1, Chase, Chase);
    NEW_NERVE_ONEND(DinoPackunBattleVs2Lv1NrvAwake, DinoPackunBattleVs2Lv1, Awake, Awake);
    NEW_NERVE(DinoPackunBattleVs2Lv1NrvAttackHit, DinoPackunBattleVs2Lv1, AttackHit);
    NEW_NERVE(DinoPackunBattleVs2Lv1NrvDamage, DinoPackunBattleVs2Lv1, Damage);
};  // namespace NrvDinoPackunBattleVs2Lv1

DinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1(DinoPackun* pPackun) : DinoPackunAction("ラスト", pPackun) {
    mStateDamage = nullptr;
    mStateAwake = nullptr;
    mTrackFireHolder = nullptr;
    mFireHolder = nullptr;
    mStateFire = nullptr;
    _28.x = 0.0f;
    _28.y = 0.0f;
    _28.z = 0.0f;
    _34 = 0;
    _38 = 0;
    _39 = 0;
    _3A = 0;
    _3B = 0;
    _3C = 0;
    _3D = 0;
}

void DinoPackunBattleVs2Lv1::setTrackFire(DinoPackunTrackFireHolder* pHolder) {
    mTrackFireHolder = pHolder;
}

void DinoPackunBattleVs2Lv1::setFireBall(DinoPackunFireHolder* pHolder) {
    mFireHolder = pHolder;
}

void DinoPackunBattleVs2Lv1::setMiddleBattle() {
    mStateDamage->setDamageNormal();
}

void DinoPackunBattleVs2Lv1::setLastBattle() {
    mStateDamage->setDamageLast();
}

void DinoPackunBattleVs2Lv1::init() {
    initNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvAwake::sInstance);
    mStateDamage = new DinoPackunStateDamage(getHost());
    mStateDamage->setDamageLast();
    mStateDamage->init();
    mStateAwake = new DinoPackunStateAwake(getHost());
    mStateAwake->init();
    mStateFire = new DinoPackunStateFire(getHost());
    mStateFire->init();
}

void DinoPackunBattleVs2Lv1::appear() {
    if (MR::isDead(getHost())) {
        getHost()->makeActorAppeared();
    }

    mIsDead = false;
    if (_3C) {
        mStateFire->requestFireDirect();
    }

    if (_3D) {
        setNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvFindCrazy::sInstance);
    } else {
        setNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvAwake::sInstance);
    }
}

void DinoPackunBattleVs2Lv1::control() {
    mStateFire->update();
}

void DinoPackunBattleVs2Lv1::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        bool v6 = false;

        if (isNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvChase::sInstance) ||
            isNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvWalk::sInstance) ||
            isNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvCrazy::sInstance)) {
            v6 = true;
        }

        if (v6) {
            if (sendBlowAttackMessage(pSender, pReceiver, mStateFire->isFire())) {
                if (_3D) {
                    return;
                }

                setNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvAttackHit::sInstance);
                return;
            }
        }

        bool v8 = false;
        if (isNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvTurn::sInstance) ||
            isNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvAwake::sInstance)) {
            v8 = true;
        }

        if (v8 && sendHitAttackMessage(pSender, pReceiver, mStateFire->isFire())) {
            if (!_3D) {
                setNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvAttackHit::sInstance);
            }
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    } else {
        bool v10 = false;
        if (isNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvChase::sInstance) ||
            isNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvWalk::sInstance) ||
            isNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvCrazy::sInstance)) {
            v10 = true;
        }

        if (v10) {
            MR::sendMsgEnemyAttack(pReceiver, pSender);
        }
    }
}

bool DinoPackunBattleVs2Lv1::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        getHost()->startHitReaction();
        return true;
    } else if (MR::isMsgPlayerSpinAttack(msg) && MR::sendMsgEnemyAttackFlipWeakJump(pSender, pReceiver)) {
        MR::emitEffectHitBetweenSensors(getHost(), pSender, pReceiver, 0.0f, "InvalidHitMark");
        return false;
    }

    return false;
}

bool DinoPackunBattleVs2Lv1::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvDamage::sInstance)) {
        if (mStateDamage->receiveOtherMsg(msg, pSender, pReceiver)) {
            mStateFire->requestCool();
            return true;
        }
    } else if (mStateDamage->isDamageMessage(msg)) {
        mTrackFireHolder->killAll();
        mFireHolder->killAll();
        setNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvDamage::sInstance);
        return true;
    }

    return false;
}

bool DinoPackunBattleVs2Lv1::tryAwake() {
    if (MR::isInSightConePlayer(getHost(), getHost()->_E8, 1050.0f, 60.0f)) {
        setNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvAwake::sInstance);
        return true;
    }

    return false;
}

DinoPackunBattleVs2Lv1::~DinoPackunBattleVs2Lv1() {
    return;
}

void DinoPackunBattleVs2Lv1::exeWalk() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), _3B ? "AngryWalk" : "Walk");
        _34 = _34 == 0;
    }

    f32 val = MR::calcNerveValue(this, 30, 1.0f, 1.5f);
    getHost()->mTail->_C = val;

    if (!tryAwake()) {
        s32 v3 = _3B ? 75 : 100;
        s32 v4 = _3B ? 15 : 20;
        f32 v5 = _3B ? 1.1f : 1.2f;

        s32 step = getNerveStep();
        s32 div = (step / v3);
        f32 v7;

        if (div % 2 == _34) {
            v7 = -1.0f;
        } else {
            v7 = 1.0f;
        }

        MR::rotateDirectionGravityDegree(getHost(), &getHost()->_E8, (v5 * v7));
        updateOnOffFireTail(v3, v4, 20);

        s32 v8 = _3A ? 500 : 610;
        if (updateWalk(v8, 1.0f, 50)) {
            if (!_34) {
                _10 = 1.0f;
            } else {
                _10 = -1.0f;
            }

            setNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvTurn::sInstance);
        }
    }
}

void DinoPackunBattleVs2Lv1::endWalk() {
    getHost()->mTail->_C = 1.0f;
}

void DinoPackunBattleVs2Lv1::exeTurn() {
    if (MR::isFirstStep(this)) {
        if (_10 > 0.0f) {
            MR::startAction(getHost(), "TurnRight");
        } else {
            MR::startAction(getHost(), "TurnLeft");
        }

        MR::startSound(getHost(), "SE_BV_D_PAKKUN_EGG_WALK", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);
    }

    emitFireTail();

    if (updateTurn(60, 3.0f)) {
        setNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvWalk::sInstance);
    } else if (MR::isInSightFanPlayer(getHost(), getHost()->_E8, 1050.0f, 40.0f, 80.0f)) {
        setNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvAwake::sInstance);
    }
}

void DinoPackunBattleVs2Lv1::exeFindCrazy() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "FindCrazy");
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_CHASE", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);
        mStateFire->requestFire();
    }

    MR::turnDirectionToPlayerDegree(getHost(), &getHost()->_E8, 3.0f);
    MR::calcNerveRate(this, 45);
    MR::addVelocityMoveToDirection(getHost(), getHost()->_E8, 0.0f);
    getHost()->updateNormalVelocity();
    if (MR::isActionEnd(getHost())) {
        setNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvCrazy::sInstance);
    }
}

void DinoPackunBattleVs2Lv1::exeCrazy() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), _39 ? "FireChase" : "Chase");
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_CHASE", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);
        mStateFire->requestFire();
    }

    if (MR::checkPassBckFrame(getHost(), 32.0f) || MR::checkPassBckFrame(getHost(), 132.0f)) {
        emitFireMouth();
    }

    f32 val = MR::calcNerveValue(this, 30, 1.0f, 1.5f);
    getHost()->mTail->_C = val;

    if (MR::isLessStep(this, 60) || MR::isInSightConePlayer(getHost(), getHost()->_E8, 2000.0f, 45.0f)) {
        MR::turnDirectionToPlayerDegree(getHost(), &getHost()->_E8, 1.0f);
    } else {
        s32 step = getNerveStep();
        s32 div = (step / 60);
        f32 v4;

        if (div % 2 != 0) {
            v4 = 1.0f;
        } else {
            v4 = -1.0f;
        }

        MR::rotateDirectionGravityDegree(getHost(), &getHost()->_E8, (2.0f * v4));
    }

    getHost()->updateFootPrintNerve(getNerveStep(), 46);
    updateOnOffFireTail(60, 10, 15);
    f32 rate = MR::calcNerveRate(this, 30);
    MR::addVelocityMoveToDirection(getHost(), getHost()->_E8, (1.7f * rate));
    getHost()->updateRunVelocity();

    if (MR::isGreaterStep(this, 240)) {
        if (MR::isInSightConePlayer(getHost(), getHost()->_E8, 1600.0f, 45.0f)) {
            setNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvFindCrazy::sInstance);
        }
    }
}

void DinoPackunBattleVs2Lv1::endCrazy() {
    getHost()->mTail->_C = 1.0f;
}

void DinoPackunBattleVs2Lv1::exeChase() {
    if (MR::isFirstStep(this)) {
        const char* action = (_39) ? "FireChase" : "Chase";
        MR::startAction(getHost(), action);
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_CHASE", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);
        mStateFire->requestFire();
    }

    if (MR::checkPassBckFrame(getHost(), 32.0f) || MR::checkPassBckFrame(getHost(), 132.0f)) {
        emitFireMouth();
    }

    f32 val = MR::calcNerveValue(this, 30, 1.0f, 1.5f);
    getHost()->mTail->_C = val;
    if (MR::isInSightConePlayer(getHost(), getHost()->_E8, 2000.0f, 60.0f)) {
        MR::turnDirectionToPlayerDegree(getHost(), &getHost()->_E8, 1.0f);
    } else {
        s32 step = getNerveStep();
        s32 div = (step / 60);
        f32 v4;

        if (div % 2 != 0) {
            v4 = 1.0f;
        } else {
            v4 = -1.0f;
        }

        MR::rotateDirectionGravityDegree(getHost(), &getHost()->_E8, (2.0f * v4));
    }

    getHost()->updateFootPrintNerve(getNerveStep(), 46);
    emitFireTail();
    f32 rate = MR::calcNerveRate(this, 45);
    MR::addVelocityMoveToDirection(getHost(), getHost()->_E8, (1.8f * rate));
    getHost()->updateRunVelocity();

    if (MR::isGreaterStep(this, 120)) {
        if (!MR::isInSightConePlayer(getHost(), getHost()->_E8, 2000.0f, 60.0f)) {
            setNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvWalk::sInstance);
        }
    }
}

void DinoPackunBattleVs2Lv1::endChase() {
    getHost()->mTail->_C = 1.0f;
}

void DinoPackunBattleVs2Lv1::exeAttackHit() {
    if (MR::isFirstStep(this)) {
        mStateFire->requestFire();
    }

    if (updateAttackHit()) {
        setNerve(&NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvTurn::sInstance);
    }
}

void DinoPackunBattleVs2Lv1::updateOnOffFireTail(s32 a1, s32 a2, s32 a3) {
    s32 v8 = 2 * a1;
    s32 v9 = getNerveStep() % (2 * a1);

    if (v9 < a1 - a2) {
        mStateFire->requestFire();

    } else {
        if (v9 >= v8 - a3) {
            mStateFire->requestFireSign();
        } else {
            mStateFire->requestCool();
        }
    }

    if (mStateFire->isFire()) {
        emitFireTail();
    }
}

bool DinoPackunBattleVs2Lv1::emitFireMouth() {
    if (!_39) {
        return false;
    }

    DinoPackunFire* fire;
    LiveActorGroup* group = mFireHolder;

    if (group->getDeadActor() != nullptr) {
        fire = static_cast< DinoPackunFire* >(group->getDeadActor());
    } else {
        fire = nullptr;
    }

    if (fire == nullptr) {
        return false;
    }

    MtxPtr mouthMtx = MR::getJointMtx(getHost(), "DownMouth1");
    TPos3f v17;
    v17.setInline(mouthMtx);
    TVec3f v16;
    v17.mult(sShotMouthFireOffset, v16);
    TVec3f v15;
    v15.set< f32 >(v17(0, 0), v17(1, 0), v17(2, 0));
    MR::normalizeOrZero(&v15);
    TVec3f v12(v15);
    v12.x *= 50.0f;
    v12.y *= 50.0f;
    v12.z *= 50.0f;
    fire->appearShot(v16, v12);
    MR::addVelocityJump(fire, 30.0f);
    MR::addVelocity(fire, getHost()->mVelocity);
    return true;
}

bool DinoPackunBattleVs2Lv1::emitFireTail() {
    if (!_38) {
        return false;
    }

    DinoPackunTrackFire* fire;
    LiveActorGroup* group = mTrackFireHolder;

    if (group->getDeadActor() != nullptr) {
        fire = static_cast< DinoPackunTrackFire* >(group->getDeadActor());
    } else {
        fire = nullptr;
    }

    if (fire != nullptr) {
        TVec3f v7(getHost()->mBall->mPosition);
        if (PSVECDistance(&_28, &v7) >= 120.0f) {
            _28.setPS2(v7);
            fire->appearAndSetPos(v7);
            return true;
        }
    }

    return false;
}

void DinoPackunBattleVs2Lv1::exeDamage() {
    if (MR::updateActorState(this, mStateDamage)) {
        kill();
    }
}

void DinoPackunBattleVs2Lv1::exeAwake() {
    MR::updateActorStateAndNextNerve(this, mStateAwake, &NrvDinoPackunBattleVs2Lv1::DinoPackunBattleVs2Lv1NrvChase::sInstance);
}

void DinoPackunBattleVs2Lv1::endAwake() {
    mStateAwake->kill();
}
