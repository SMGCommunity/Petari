#include "Game/Boss/BossKameckStateBattle.hpp"
#include "Game/Boss/BossKameckBattlePattarn.hpp"
#include "Game/Boss/BossKameck.hpp"
#include "Game/Enemy/KameckBeam.hpp"
#include "Game/Enemy/KameckBeamHolder.hpp"

BossKameckStateBattle::BossKameckStateBattle(BossKameck *pBoss) : BossKameckStateHolder(pBoss, "ボスカメック戦闘状態") {
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

BossKameckStateBattle::~BossKameckStateBattle() {

}