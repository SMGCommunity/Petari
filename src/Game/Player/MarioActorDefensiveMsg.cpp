#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Util/ActorSensorUtil.hpp"

// bool MarioActor::receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

bool MarioActor::tryAttackMsg(u32 msg, const HitSensor* pSensor, bool* myBool) {
    if (myBool != nullptr) {
        *myBool = true;
    }

    switch (msg) {
    case ACTMES_ENEMY_ATTACK_FLIP_VERYWEAK:
        if (mMario->getMovementStates()._1 && !isAnimationRun(nullptr)) {
            changeAnimation("ノーダメージ", nullptr);
        }

        return true;

    case ACTMES_ENEMY_ATTACK_FIREBACK:
    case ACTMES_ENEMY_ATTACK_FIRERUN:
    case ACTMES_ENEMY_ATTACK_FIRERUN_STRONG:
        if (selectDamageFireColor(pSensor->mHost->mName)) {
            _1B4 = true;
        } else {
            _1B4 = false;
        }

        mMario->doFireObjHitWithInitialDamage();

        return true;

    case ACTMES_ENEMY_ATTACK_ELECTRIC:
    case ACTMES_ENEMY_ATTACK_HEATBEAM:
        return mMario->doParalyze();

    case ACTMES_ENEMY_ATTACK_FREEZE:
        return mMario->doFreeze();

    case ACTMES_ENEMY_ATTACK_CRUSH:
        return mMario->requestCrush();

    default:
        if (myBool != nullptr) {
            *myBool = false;
        }

        return false;
    }
}

// bool MarioActor::receiveMsgPush(HitSensor*, HitSensor*);

// bool MarioActor::receiveOtherMsg(u32, HitSensor*, HitSensor*);

bool MarioActor::tryVectorAttackMsg(u32 msg, const TVec3f& rVec) {
    TVec3f newVec;
    bool out = false;
    switch (msg) {
    case ACTMES_ENEMY_ATTACK_MAXIMUM:
        out = mMario->damageLarge(rVec);
        break;

    case ACTMES_ENEMY_ATTACK_EXPLOSION:
        out = mMario->damage(rVec);
        break;

    case ACTMES_ENEMY_ATTACK_FLIP_MAXIMUM:
        newVec = rVec;
        newVec.setLength(15.0f);
        out = mMario->doFlipLarge(newVec);
        break;

    case ACTMES_ENEMY_ATTACK_FLIP:
        newVec = rVec;
        newVec.setLength(10.0f);
        out = mMario->doFlipLarge(newVec);
        break;

    case ACTMES_ENEMY_ATTACK_FLIP_ROT:
        if (!isJumping()) {
            out = mMario->doFlipRot(rVec);
        }
        break;

    case ACTMES_ENEMY_ATTACK_FLIP_WEAK:
        if (!isJumping()) {
            out = mMario->doFlipWeak(rVec);
        }
        break;

    case ACTMES_ENEMY_ATTACK_FLIP_WEAK_JUMP:
        out = mMario->doFlipJump(rVec);
        break;

    case ACTMES_ENEMY_ATTACK_FLIP_JUMP:
        newVec = rVec;
        newVec.setLength(15.0f);
        out = mMario->doFlipBackRoll(newVec);
        break;

    case ACTMES_ENEMY_ATTACK_COUNTER_SPIN:
        if (!isJumping()) {
            out = mMario->doFlipRot(rVec);
        } else {
            mMario->stopWalk();
            newVec = rVec;
            newVec.setLength(20.0f);
            mMario->addReaction(newVec);
        }
        break;

    case ACTMES_ENEMY_ATTACK_COUNTER_HIPDROP:
        _988 = 2;
        trampleJump(mConst->getTable()->mTrampleNormal, mConst->getTable()->mTrampleLong);
        break;

    default:
        if (isJumping() || msg == ACTMES_ENEMY_ATTACK_STRONG) {
            out = mMario->damage(rVec);
        } else {
            out = mMario->faint(rVec);
        }
        break;
    }

    return out;
}

bool MarioActor::receiveMsgTaken(HitSensor* pReceiver, HitSensor* pSender) {
    if (_934) {
        return false;
    }

    if (mMario->isSwimming() && !mMario->isStatusActive(0x18)) {
        u32 isType = 0;
        if (pReceiver->isType(0x10)) {
            isType = 1;
        }

        if (!mMario->mSwim->startJet(isType)) {
            return false;
        }
    } else if (pReceiver != _424) {
        return false;
    } else if (_B92 == 0) {
        mMarioAnim->updateTakingAnimation(pReceiver);
    }

    memorizeSensorThrow(pReceiver);

    return true;
}

// void MarioActor::cylinderHitCheck(const HitSensor*, TVec3f*);

void MarioState::hitWall(const TVec3f&, HitSensor*) {
}

bool MarioState::passRing(const HitSensor*) {
    return false;
}
