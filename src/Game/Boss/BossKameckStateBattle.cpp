#include "Game/Boss/BossKameckStateBattle.hpp"
#include "Game/Boss/BossKameckBattlePattarn.hpp"
#include "Game/Boss/BossKameckMoveRail.hpp"
#include "Game/Boss/BossKameck.hpp"
#include "Game/Enemy/KameckBeam.hpp"
#include "Game/Enemy/KameckBeamHolder.hpp"

BossKameckStateBattle::BossKameckStateBattle(BossKameck *pBoss) : mBossKameck(pBoss), ActorStateBase<BossKameck>("ボスカメック戦闘状態") {
    mMoveRail = nullptr;
    mBattlePattarn = nullptr;
    mBeam = nullptr;
    mBeamListener = nullptr;
    _20.setInline(0.0f);
    _2C = -1;
    _30 = 1;
    _34 = 0;
    _38 = 0;
    _3C = 0;
    _3D = 0;
}
 
void BossKameckStateBattle::init() {
    initNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvWait::sInstance);
}

void BossKameckStateBattle::appear() {
    mIsDead = false;
    startMove();
    MR::onCalcGravity(mBossKameck);
}

void BossKameckStateBattle::setMoveRail(BossKameckMoveRail *pMoveRail) {
    mMoveRail = pMoveRail;
}

void BossKameckStateBattle::setBattlePattarn(BossKameckBattlePattarn *pPattarn) {
    mBattlePattarn = pPattarn;
}

void BossKameckStateBattle::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (isEnableGuard() && MR::isSensorPlayer(a2)) {
        if (isNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvGuard::sInstance)) {
            if (MR::sendMsgEnemyAttackFlipRot(a2, a1)) {
                return;
            }
        }
        else if (MR::isSensorEnemyAttack(a1)) {
            if (!MR::sendMsgEnemyAttack(a2, a1)) {
                MR::sendMsgPush(a2, a1);
            }
        }
    }
}

bool BossKameckStateBattle::receiveMsgPlayerAttack(u32 msg, HitSensor *a1, HitSensor* a2) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    if (MR::isMsgJetTurtleAttack(msg)) {
        return requestDamage(a1, a2);
    }

    if (MR::isMsgPlayerSpinAttack(msg)) {
        return requestGuard(a1, a2);
    }

    return false;
}

bool BossKameckStateBattle::requestDamage(HitSensor *a1, HitSensor *a2) {
    if (isEnableDamage()) {
        if (mBeam != nullptr) {
            mBeam->kill();
            mBeam = nullptr;
        }

        mBossKameck->killAllBeam();
        MR::emitEffectHitBetweenSensors(mBossKameck, a1, a2, 0.0f, "HitMarkNormal");
        setNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvDamage::sInstance);
        return true;
    }

    return false;
}

bool BossKameckStateBattle::requestGuard(HitSensor *a1, HitSensor *a2) {
    if (isEnableGuard()) {
        if (mBeam != nullptr) {
            mBeam->kill();
            mBeam = nullptr;
        }

        setNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvGuard::sInstance);
        return true;
    }

    return false;
}

void BossKameckStateBattle::control() {
    mBossKameck->updatePose();
}

bool BossKameckStateBattle::tryAttackWait() {
    if (MR::isGreaterStep(this, 120) && MR::isNear(mBossKameck, _20, 100.0f)) {
        if (tryChargeBram()) {
            setNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvAttackWait::sInstance);
        }
        else {
            startMove();
        }

        return true;
    }

    return false;
}

bool BossKameckStateBattle::trySummonKameck() {
    if (mBattlePattarn->_10 == nullptr) {
        return false;
    }

    if (_38 <= 3) {
        return false;
    }

    if (mBossKameck->getLivingKameckNum() == 0) {
        _38 = 0;
        return true;
    } 

    return false;
}

bool BossKameckStateBattle::tryChargeBram() {
    if (mBossKameck->mActorList->isFull()) {
        return false;
    }

    TVec3f v6;
    v6.setInline(0.0f);
    MtxPtr jointMtx = MR::getJointMtx(mBossKameck, "PowerStarC");
    mBeam = MR::startFollowKameckBeam(mBattlePattarn->goNextPattarn(), jointMtx, 1.0f, v6, mBeamListener);

    if (mBeam != nullptr) {
        mBeam->setEventListener(mBossKameck->mBeamListener);

        if (mBossKameck->getLivingKameckNum() == 0) {
            _38++;
        }
        else {
            _38 = 0;
        }

        return true;
    }

    return false;
}

void BossKameckStateBattle::startMove() {
    if (_3D) {
        setNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvHideMoveStart::sInstance);
    }
    else {
        setNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvMove::sInstance);
    }
}

void BossKameckStateBattle::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mBossKameck, "Wait");
    }

    MR::attenuateVelocity(mBossKameck, 0.95999998f);

    if (MR::isGreaterStep(this, 60)) {
        startMove();
    }
}

/* epilogue is swapped but shrug */
void BossKameckStateBattle::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mBossKameck, "Wait");
        selectPosition();
    }

    f32 mag = (100.0f * PSVECMag(mBossKameck->mVelocity.toCVec()));
    MR::startLevelSound(mBossKameck, "SE_BM_LV_KAMECK_FLOAT", mag, -1, -1);

    s32 v2 = (_3C) ? 40 : 90;

    if (MR::isLessEqualStep(this, v2)) {
        if (MR::isNear(mBossKameck, _20, 100.0f) || MR::isNearPlayer(_20, 400.0f)) {
            selectPosition();
        }

        BossKameck* boss = mBossKameck;
        MR::addVelocityAwayFromTarget(boss, *MR::getPlayerPos(), 1.5f, 0.0f, 0.0f, 500.0f);
    }

    MR::turnDirectionToPlayerDegree(mBossKameck, &mBossKameck->_A0, 6.0f);
    MR::addVelocityMoveToTarget(mBossKameck, _20, 0.089999996f, 0.89999998f, 0.0f, 400.0f);
    MR::addVelocityKeepHeight(mBossKameck, _20, 0.0f, 0.5f, 50.0f);
    MR::attenuateVelocity(mBossKameck, 0.95999998f);
    tryAttackWait();
}

void BossKameckStateBattle::exeHideMoveStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mBossKameck, "Hide");
        MR::startSound(mBossKameck, "SE_BM_KAMECK_HIDE_SPIN", -1, -1);
    }

    MR::zeroVelocity(mBossKameck);
    MR::turnDirectionToPlayerDegree(mBossKameck, &mBossKameck->_A0, 6.0f);

    if (MR::isActionEnd(mBossKameck)) {
        MR::startSound(mBossKameck, "SE_BM_KAMECK_HIDE_SMOKE", -1, -1);
        setNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvHideMove::sInstance);
    }
}

void BossKameckStateBattle::exeHideMove() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mBossKameck, "Move");
        MR::hideModelAndOnCalcAnim(mBossKameck);
        selectPosition();
        _34 = 0;
    }

    MR::startLevelSound(mBossKameck, "SE_BM_LV_KAMECK_HIDE_MOVE", -1, -1, -1);

    f32 v2;

    if (_3C) {
        v2 = 20.0f;
    }
    else {
        v2 = 15.0f;
    }

    MR::addVelocityMoveToTarget(mBossKameck, _20, (0.1f * v2), v2, 0.0f, 400.0f);
    MR::addVelocityKeepHeight(mBossKameck, _20, 0.0f, 0.5f, 50.0f);
    MR::attenuateVelocity(mBossKameck, 0.89999998f);
    MR::turnDirectionToPlayerDegree(mBossKameck, &mBossKameck->_A0, 6.0f);

    if (MR::isNear(mBossKameck, _20, 100.0f)) {
        s32 v3 = _30;
        s32 v4 = _34;
        if (v4 <= v3) {
            setNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvHideMoveEnd::sInstance);
        }
        else {
            _34 = v3 + 1; 

            if (v4 == v3 + 1) {
                selectStoppablePosition();
            }
            else {
                selectPosition();
            }
        }
    }
}

void BossKameckStateBattle::exeHideMoveEnd() {
    if (MR::isFirstStep(this)) {
        MR::showModel(mBossKameck);
        MR::startAction(mBossKameck, "Appear");
        MR::startSound(mBossKameck, "SE_BM_KAMECK_APPEAR_SMOKE", -1, -1);
        MR::startSound(mBossKameck, "SE_BM_KAMECK_APPEAR_SPIN", -1, -1);
    }

    MR::zeroVelocity(mBossKameck);
    MR::turnDirectionToPlayerDegree(mBossKameck, &mBossKameck->_A0, 6.0f);

    if (MR::isActionEnd(mBossKameck)) {
        if (trySummonKameck()) {
            setNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvSummonKameckWait::sInstance);
        }
        else if (tryChargeBram()) {
            setNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvAttackWait::sInstance);
        }
        else {
            startMove();
        }
    }
}

void BossKameckStateBattle::exeSummonKameckWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mBossKameck, "SummonWait");
    }

    MR::zeroVelocity(mBossKameck);
    MR::turnDirectionToPlayerDegree(mBossKameck, &mBossKameck->_A0, 6.0f);

    if (MR::isActionEnd(mBossKameck)) {
        setNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvSummonKameck::sInstance);
    }
}

void BossKameckStateBattle::exeSummonKameck() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mBossKameck, "Summon");
        mBossKameck->appearKameck();
    }

    if (MR::isActionEnd(mBossKameck)) {
        setNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvWait::sInstance);
    }
}

void BossKameckStateBattle::exeAttackWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mBossKameck, "AttackWait");
    }

    MR::startLevelSound(mBossKameck, "SE_BM_LV_KAMECK_STAFF_TURN", -1, -1, -1);
    MR::turnDirectionToPlayerDegree(mBossKameck, &mBossKameck->_A0, 6.0f);
    MR::addVelocityMoveToTarget(mBossKameck, _20, 0.089999996f, 0.89999998f, 0.0f, 400.0f);
    MR::addVelocityKeepHeight(mBossKameck, _20, 0.0f, 0.2f, 50.0f);
    MR::attenuateVelocity(mBossKameck, 0.89999998f);

    if (MR::isGreaterStep(this, 60)) {
        setNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvAttack::sInstance);
    }
}

void BossKameckStateBattle::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mBossKameck, "Attack");
    }

    if (MR::isStep(this, 12)) {
        MR::startSound(mBossKameck, "SE_BM_KAMECK_STAFF_SWING", -1, -1);
        MR::startSound(mBossKameck, "SE_BV_KAMECK_STAFF_SWING", -1, -1);
    }

    if (MR::isStep(this, 23)) {
        mBeam->requestShootToPlayerGround(20.0f);
        mBossKameck->mActorList->addActor(mBeam);
        mBeam = nullptr;
        MR::startSound(mBossKameck, "SE_BM_KAMECK_STAFF_EFFECT", -1, -1);
    }

    MR::attenuateVelocity(mBossKameck, 0.95999998f);
    
    if (MR::isGreaterStep(this, 30)) {
        setNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvWait::sInstance);
    }
}

void BossKameckStateBattle::exeDamage() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(mBossKameck);

        if (_3C) {
            MR::stopStageBGM(30);
            MR::startAction(mBossKameck, "Down");
            MR::startSound(mBossKameck, "SE_BV_KAMECK_LAST_DAMAGE", -1, -1);
            MR::startSound(mBossKameck, "SE_BM_KAMECK_LAST_DAMAGE", -1, -1);
        }
        else {
            MR::startAction(mBossKameck, "Damage");
            MR::startSound(mBossKameck, "SE_BV_KAMECK_DAMAGE", -1, -1);
            MR::startSound(mBossKameck, "SE_BM_KAMECK_DAMAGE", -1, -1);
        }
    }

    if (MR::isActionEnd(mBossKameck)) {
        kill();
    }
}

void BossKameckStateBattle::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mBossKameck, "Recover");
        MR::startSound(mBossKameck, "SE_BM_KAMECK_RECOVER", -1, -1);
    }

    if (MR::isGreaterStep(this, 60)) {
        MR::turnDirectionToPlayerDegree(mBossKameck, &mBossKameck->_A0, 6.0f);
        MR::addVelocityMoveToTarget(mBossKameck, _20, 0.089999996f, 0.89999998f, 0.0f, 400.0f);
        MR::addVelocityKeepHeight(mBossKameck, _20, 0.0f, 0.5f, 50.0f);
        MR::attenuateVelocity(mBossKameck, 0.95999998f);
    }

    if (MR::isGreaterStep(this, 60)) {
        if (MR::isNear(mBossKameck, _20, 100.0f)) {
            MR::validateHitSensors(mBossKameck);
            startMove();
        }
    }
}

void BossKameckStateBattle::exeGuard() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mBossKameck, "Guard");
        MR::turnDirectionToTarget(mBossKameck, &mBossKameck->_A0, *MR::getPlayerPos(), 0.30000001f);
    }

    MR::attenuateVelocity(mBossKameck, 0.95999998f);

    if (MR::isActionEnd(mBossKameck)) {
        startMove();
    }
}

void BossKameckStateBattle::selectStoppablePosition() {
    _2C = mMoveRail->calcRandomStoppableIndex(_2C);
    mMoveRail->calcMovePosition(&_20, _2C);
}

void BossKameckStateBattle::selectPosition() {
    _2C = mMoveRail->calcRandomIndex(_2C);
    mMoveRail->calcMovePosition(&_20, _2C);
}

bool BossKameckStateBattle::isEnableDamage() const {
    bool ret = false;

    if (isNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvWait::sInstance)
    || isNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvMove::sInstance)
    || isNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvHideMoveStart::sInstance)
    || isNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvHideMoveEnd::sInstance)
    || isNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvSummonKameckWait::sInstance)
    || isNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvSummonKameck::sInstance)
    || isNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvAttackWait::sInstance)
    || isNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvAttack::sInstance)) {
        return true;
    }

    return ret;
}

bool BossKameckStateBattle::isEnableGuard() const {
    bool ret = false;

    if (isNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvWait::sInstance)
    || isNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvMove::sInstance)
    || isNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvHideMoveStart::sInstance)
    || isNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvHideMoveEnd::sInstance)
    || isNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvSummonKameckWait::sInstance)
    || isNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvSummonKameck::sInstance)
    || isNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvAttackWait::sInstance)
    || isNerve(&NrvBossKameckStateBattle::BossKameckStateBattleNrvAttack::sInstance)) {
        return true;
    }

    return ret;
}

BossKameckStateBattle::~BossKameckStateBattle() {

}

namespace NrvBossKameckStateBattle {
    INIT_NERVE(BossKameckStateBattleNrvWait);
    INIT_NERVE(BossKameckStateBattleNrvMove);
    INIT_NERVE(BossKameckStateBattleNrvHideMoveStart);
    INIT_NERVE(BossKameckStateBattleNrvHideMove);
    INIT_NERVE(BossKameckStateBattleNrvHideMoveEnd);
    INIT_NERVE(BossKameckStateBattleNrvSummonKameckWait);
    INIT_NERVE(BossKameckStateBattleNrvSummonKameck);
    INIT_NERVE(BossKameckStateBattleNrvAttackWait);
    INIT_NERVE(BossKameckStateBattleNrvAttack);
    INIT_NERVE(BossKameckStateBattleNrvDamage);
    INIT_NERVE(BossKameckStateBattleNrvRecover);
    INIT_NERVE(BossKameckStateBattleNrvGuard);
};
