#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/BlackHole.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioHang.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Player/RushEndInfo.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include <cstring>

void MarioActorDefensiveMsg_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)0.5f;
    (void)0.001f;
    (void)15.0f;
    (void)30.0f;
    (void)10.0f;
    (void)20.0f;
    (void)1.1f;
    (void)100.0f;
}

bool MarioActor::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    bool carrying = getCarrySensor() != nullptr;
    if (mPlayerMode == PlayerMode_Teresa) {
        if (msg == ACTMES_ENEMY_ATTACK_HEATBEAM) {
            setPlayerMode(PlayerMode_Normal, true);
            return true;
        }

        return false;
    }

    if (mPlayerMode == PlayerMode_Ice && msg == ACTMES_ENEMY_ATTACK_FREEZE) {
        msg = ACTMES_ENEMY_ATTACK;
    }

    if (_934) {
        if (!mMario->isEnableAddDamage()) {
            return false;
        }

        if (cylinderHitCheck(pSender, nullptr)) {
            return MR::sendArbitraryMsg(msg, _924, pSender);
        }

        return false;
    }

    if (!isNerve(GET_NERVE(MarioActor, MarioActorNrvWait))) {
        return false;
    }

    if (msg == ACTMES_ENEMY_ATTACK_EXTRA_DAMAGE) {
        if (mPlayerMode == PlayerMode_Invincible) {
            return false;
        }

        decLife(0);
        return true;
    }

    if (!_FCC && !_FCD) {
        if (!cylinderHitCheck(pSender, nullptr)) {
            if (msg == ACTMES_ENEMY_ATTACK_FLIP_ROT && _3E5) {
                TVec3f direction(pReceiver->mPosition);
                direction -= pSender->mPosition;
                if (MR::isNearZero(direction)) {
                    getFrontVec(&direction);
                }

                direction.setLength(15.0f);
                return mMario->doFlipRot(direction);
            }

            return false;
        }

        if (!isDamaging()) {
            if (checkAndTryTrampleAttack(pSender, (pSender->mPosition - pReceiver->mPosition).length(), true)) {
                return false;
            }
        }
    }

    bool handled;
    bool result = tryAttackMsg(msg, pSender, &handled);
    if (handled) {
        if (result && carrying) {
            tryCounterJetAttack(pSender);
        }

        return result;
    }

    switch (msg) {
    case ACTMES_ENEMY_ATTACK_EXPLOSION: {
        if (MR::isExistMapCollisionExceptActor(pSender->mPosition, _2A0 - pSender->mPosition, pSender->mHost)) {
            return false;
        }

        break;
    }
    }

    TVec3f direction(pReceiver->mPosition);
    direction -= pSender->mPosition;
    direction.length();
    if (MR::isNearZero(direction)) {
        getFrontVec(&direction);
    }

    direction.setLength(30.0f);
    result = tryVectorAttackMsg(msg, direction);
    if (!result) {
        MR::sendArbitraryMsg(ACTMES_INVALID_HIT, pSender, getSensor("body"));
    } else {
        u8 autoBind;
        selectAutoBind(pSender->mHost->mName, &autoBind);
        if (autoBind == 2) {
            setPlayerMode(PlayerMode_Normal, true);
        }

        if (carrying) {
            tryCounterJetAttack(pSender);
        }

        if (msg == ACTMES_ENEMY_ATTACK_FLIP_MAXIMUM) {
            printHitMark(pSender);
        }
    }

    return result;
}

bool MarioActor::tryAttackMsg(u32 msg, const HitSensor* pSensor, bool* pHandled) {
    if (pHandled != nullptr) {
        *pHandled = true;
    }

    switch (msg) {
    case ACTMES_ENEMY_ATTACK_FLIP_VERYWEAK:
        if (getMovementStates()._1 && !isAnimationRun(nullptr)) {
            changeAnimation("ノーダメージ");
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
        if (pHandled != nullptr) {
            *pHandled = false;
        }

        return false;
    }
}

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

bool MarioActor::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    mMario->resetSleepTimer();
    if (pSender == _424) {
        return false;
    }

    if (pSender == getCarrySensor()) {
        return false;
    }

    mMario->setNotSafetyTimer();
    if (mMario->isStatusActive(MarioStatus_Stick)) {
        return false;
    }

    if (mMario->isStatusActive(MarioStatus_Crush)) {
        return false;
    }

    if (!isEnableMoveMario()) {
        return false;
    }

    if (_934) {
        return MR::sendMsgPush(_924, pSender);
    }

    TVec3f push;
    if (!cylinderHitCheck(pSender, &push)) {
        return false;
    }

    if (!MR::isNearZero(push)) {
        if (!isDamaging()) {
            if (checkAndTryTrampleAttack(pSender, (pSender->mPosition - pReceiver->mPosition).length(), true)) {
                return false;
            }

            if (mMario->mMovementStates._B && mMario->mMovementStates.jumping && tryHipDropAttack(pSender)) {
                return false;
            }
        }

        if (mMario->isSwimming() && getCarrySensor() != nullptr && (pSender->isType(ATYPE_JET_TURTLE) || pSender->isType(ATYPE_JET_TURTLE_SLOW))) {
            mMario->startPadVib(2);
            sendMsgToSensor(pSender, ACTMES_ATTACKDROP);
            return false;
        }

        if (strstr(pSender->mHost->mName, "ゴーストマリオ") != nullptr && (isJumping() || mMario->isHanging())) {
            return false;
        }

        if (mMario->isActiveTaskID(0x200)) {
            return false;
        }

        mMario->push(push * 1.1f);
        if (mMario->isStatusActive(MarioStatus_Foo)) {
            TVec3f direction(pReceiver->mPosition);
            direction -= pSender->mPosition;
            if (!MR::normalizeOrZero(&direction)) {
                mMario->_97C->hitWall(direction, nullptr);
            }
        }

        if (isJumping()) {
            if (mMario->_3BC > 1) {
                if (push.dot(mMario->mJumpVec) < 0.0f) {
                    MR::normalizeOrZero(&push);
                    mMario->cutVecElementFromJumpVec(push);
                }

                if (!mMario->isRising() && !MR::sendMsgPush(pSender, pReceiver)) {
                    if (getMovementStates()._B) {
                        mMario->startHipDropSlide(pSender);
                    } else {
                        mMario->startJumpDropSlide(pSender);
                    }
                }
            }
        }

        return true;
    }

    return false;
}

bool MarioActor::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (_934) {
        if (msg == ACTMES_INHALE_BLACK_HOLE && _924->isType(ATYPE_MARIO_LAUNCHER_BIND)) {
            if (!_924->receiveMessage(ACTMES_RUSH_CANCEL, getSensor("body")) || _934) {
                RushEndInfo info(nullptr, 4, TVec3f(0.0f), false, 0);
                endRush(&info);
            }
        } else {
            return false;
        }
    }

    switch (msg) {
    case ACTMES_IS_BROKEN:
    case ACTMES_REQUEST_CLAP:
        break;
    case ACTMES_PUSH_FORCE:
        if (mMario->isStatusActive(MarioStatus_Hang)) {
            mMario->mHang->forceDrop();
        }

        return receiveMsgPush(pSender, pReceiver);
    case ACTMES_START_DEMO:
        mMario->invalidateRelativePosition();
        break;
    case ACTMES_INHALE_BLACK_HOLE:
        if (mMario->_1C._8) {
            return false;
        }

        mBlackHolePosition = pSender->mPosition;
        mBlackHole = static_cast< BlackHole* >(pSender->mHost);
        forceGameOverBlackHole();
        return true;
    case ACTMES_REFLECT_V:
        if (mMario->getCurrentStatus() != MarioStatus_Damage && MR::isSensorPlayer(pReceiver)) {
            mMario->mMovementStates._5 = false;
            mMario->mMovementStates._2B = true;
            mMario->_402 = 0;
            mMario->cutGravityElementFromJumpVec(true);
            return true;
        }

        break;
    case ACTMES_JUMP:
        if (mMario->getCurrentStatus() == MarioStatus_Damage) {
            break;
        }

        if (MR::isSensorPlayer(pReceiver)) {
            TVec3f velocity(pReceiver->mPosition);
            velocity -= pSender->mPosition;
            if (!isJumping()) {
                MR::vecKillElement(velocity, getGravityVector(), &velocity);
                velocity.setLength(mConst->getTable()->mMsgJumpGroundPowerXZ);
                velocity.add(-getGravityVector() * mConst->getTable()->mMsgJumpGroundPowerY);
            } else {
                velocity.setLength(mConst->getTable()->mMsgJumpAirPower);
            }

            mMario->tryForceFreeJump(velocity);
            mMario->mMovementStates._9 = false;
            mMario->mMovementStates._6 = false;
            return true;
        }

    case ACTMES_TOUCHJUMP:
        if (mMario->getCurrentStatus() != MarioStatus_Damage && MR::isSensorPlayer(pReceiver)) {
            if (!cylinderHitCheck(pSender, nullptr)) {
                return false;
            }

            TVec3f velocity(pReceiver->mPosition);
            velocity -= pSender->mPosition;
            if (!isJumping()) {
                MR::vecKillElement(velocity, getGravityVector(), &velocity);
                velocity.setLength(mConst->getTable()->mMsgJumpGroundPowerXZ);
                velocity.add(-getGravityVector() * mConst->getTable()->mMsgJumpGroundPowerY);
            } else {
                velocity.setLength(mConst->getTable()->mMsgJumpAirPower);
            }

            mMario->tryForcePowerJump(velocity, false);
            stopAnimation(nullptr);
            mMario->mMovementStates._9 = false;
            mMario->mMovementStates._6 = false;
            playSound("声壁反射", -1);
            return true;
        }

        break;
    case ACTMES_BILLIARDJUMP: {
        if (!isJumping()) {
            return receiveMsgPush(pSender, pReceiver);
        }

        if (!cylinderHitCheck(pSender, nullptr)) {
            return false;
        }

        TVec3f direction(pReceiver->mPosition);
        direction -= pSender->mPosition;
        MR::normalizeOrZero(&direction);
        f32 speed = mMario->cutVecElementFromJumpVec(direction);
        if (speed >= 0.0f) {
            return false;
        }

        f32 impulse = speed - mConst->getTable()->mBilliardJumpExtra;
        mMario->mJumpVec.add(-direction * impulse);
        mMario->_430 = 3;
        return true;
    }

    case ACTMES_AWAYJUMP:
        doTrampleJump(pSender);
        mMario->startPadVib(3);
        mMario->mJumpVec -= mMario->mFrontVec * 10.0f;
        changeAnimation("飛びすさりジャンプ");
        break;
    case ACTMES_REFLECT: {
        TVec3f normal;
        MR::calcDistance(pSender, pReceiver, &normal);
        mMario->reflectWallOnSpinning(normal, 5);
        if (mMario->mWalkSpeed > 0.5f) {
            mMario->mWalkSpeed = 0.5f;
        }

        return true;
    }

    case ACTMES_KICK:
        playSound("声ランニングキック", -1);
        changeAnimation("ランニングキック");
        return true;
    case ACTMES_PASS_RING:
        if (mMario->_97C == nullptr) {
            return false;
        }

        return mMario->_97C->passRing(pSender);
    case ACTMES_FOUNTAINJUMP: {
        mMario->touchWater();
        TVec3f horizontal;
        MR::vecKillElement(pReceiver->mPosition - pSender->mPosition, getGravityVec(), &horizontal);
        if (horizontal.length() >= pSender->mRadius) {
            return receiveMsgPush(pSender, pReceiver);
        }

        bool flowing = mMario->doFlow();
        if (flowing) {
            playSound("水弾かれ", -1);
            playSound("声噴水ジャンプ", -1);
        }

        return flowing;
    }

    case ACTMES_RUSHDROP:
        tryReleaseDirect(pSender);
        break;
    case ACTMES_SHAKE_OFF_BEE:
        _9F2 = 60;
        break;
    case ACTMES_PUDDLE_TOUCH_GROUND:
        _3A8 = 3;
        break;
    case ACTMES_BOMB_TERESA_TONGUE_TOUCH:
        if (_468 != 0) {
            return false;
        }

        if (isDamaging()) {
            return false;
        }

        if (!mPlayerMode || mPlayerMode == PlayerMode_Invincible) {
            _B94 = 2;
            changeAnimationUpper("ハンマー投げ回転中");
            return true;
        }

        return false;
    case ACTMES_WARP:
        return mMario->doObjWarp(pSender->mHost);
    }

    return false;
}

bool MarioActor::receiveMsgTaken(HitSensor* pSender, HitSensor* pReceiver) {
    if (_934) {
        return false;
    }

    if (mMario->isSwimming() && !mMario->isStatusActive(MarioStatus_Foo)) {
        u32 type = 0;
        if (pSender->isType(ATYPE_JET_TURTLE_SLOW)) {
            type = 1;
        }

        if (!mMario->mSwim->startJet(type)) {
            return false;
        }
    } else if (pSender != _424) {
        return false;
    } else if (_B92 == 0) {
        mMarioAnim->updateTakingAnimation(pSender);
    }

    memorizeSensorThrow(pSender);

    return true;
}

bool MarioActor::cylinderHitCheck(const HitSensor* pSensor, TVec3f* pPush) {
    Mario* pMario = mMario;
    f32 radius = pSensor->mRadius;
    TVec3f delta = pSensor->mPosition - (_2A0 + pMario->_350 + pMario->_35C + mVelocity);
    TVec3f radial;
    f32 axial = MR::vecKillElement(delta, _4C4, &radial);
    f32 spherePenetration = 100.0f + radius - delta.length();
    if (spherePenetration < 0.0f) {
        return false;
    }

    if (axial > -radius) {
        f32 radialPenetration = radius + _4B0 - radial.length();
        f32 axialPenetration = radius + _4B4 - axial;
        if (radialPenetration > 0.0f && axialPenetration > 0.0f) {
            if (pPush != nullptr) {
                MR::normalizeOrZero(&radial);
                pPush->zero();
                if (spherePenetration < radialPenetration && spherePenetration < axialPenetration) {
                    MR::normalizeOrZero(&delta);
                    pPush->add(-delta * spherePenetration);
                } else if (radialPenetration < axialPenetration) {
                    pPush->add(-radial * radialPenetration);
                } else {
                    pPush->add(-_4C4 * axialPenetration);
                    pPush->add(-radial);
                }
            }

            return true;
        }
    }

    if (mMario->getMovementStates()._A && mMario->mMovementStates._1) {
        return false;
    }

    axial = MR::vecKillElement(delta, _4B8, &radial);
    if (axial < -radius) {
        return false;
    }

    f32 radialPenetration = radius + _4B0 - radial.length();
    f32 axialPenetration = radius + _4B4 - axial;
    if (radialPenetration > 0.0f && axialPenetration > 0.0f) {
        if (pPush != nullptr) {
            MR::normalizeOrZero(&radial);
            pPush->zero();
            if (spherePenetration < radialPenetration && spherePenetration < axialPenetration) {
                MR::normalizeOrZero(&delta);
                pPush->add(-delta * spherePenetration);
            } else if (radialPenetration < axialPenetration) {
                pPush->add(-radial * radialPenetration);
            } else {
                pPush->add(-_4B8 * axialPenetration);
                pPush->add(-radial);
            }
        }

        return true;
    }

    return false;
}

void MarioState::hitWall(const TVec3f&, HitSensor*) {
}

bool MarioState::passRing(const HitSensor*) {
    return false;
}
