#include "Game/Player/MarioBlown.hpp"
#include "Game/Player/FireMarioBall.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Util/MathUtil.hpp"
#include "revolution/types.h"

bool Mario::blown(const TVec3f& rVec) {
    if (getCurrentStatus() == MarioStatus_Blown) {
        return false;
    }

    if (getCurrentStatus() == MarioStatus_Damage) {
        return false;
    }

    if (getCurrentStatus() == MarioStatus_13) {
        return false;
    }

    stopWalk();

    mBlown->vecKillActor240(rVec);

    mMovementStates._2C = true;
    mMovementStates._A = false;
    _430 = 3;

    return true;
}

MarioBlown::MarioBlown(MarioActor* pActor) : MarioState(pActor, MarioStatus_Blown), mTimer(), _14() {
    _18.zero();
    _24 = false;
    _25 = false;
}

bool MarioBlown::close() {
    if (!_24) {
        getPlayer()->stopJump();
    }

    stopAnimation("壁ヒット");

    if (_25) {
        stopAnimation("壁ヒット着地", "基本");
    }

    mActor->setBlendMtxTimer(6);
    getPlayer()->unlockGroundCheck(this);

    if (!_24) {
        getPlayer()->mMovementStates._36 = false;
    }

    return true;
}

bool MarioBlown::start() {
    mTimer = 0;
    _14 = 0;
    _24 = false;
    _25 = false;

    changeAnimation("壁ヒット", "基本");
    playSound("声壁体当たり");
    playSound("壁衝突");

    playEffectTrans("壁ヒット", getPlayer()->getWallPos());

    getPlayer()->mMovementStates._1 = false;
    getPlayer()->mMovementStates.jumping = true;
    getPlayer()->mMovementStates._B = false;

    _18 += -mActor->_240 * mActor->getConst().getTable()->mJumpHeightBlown;
    getPlayer()->setJumpVec(_18);
    addVelocity(_18);
    getPlayer()->lockGroundCheck(this, true);
    return true;
}

bool MarioBlown::update() {
    mTimer++;

    if (getPlayerMode() == PlayerMode_Teresa) {
        return false;
    }

    switch (_14) {
    case 0:
    case 2:
        addVelocity(_18);
        _18 += mActor->_240 * mActor->getConst().getTable()->mGravityBlown;
        if (mTimer > 120) {
            changeAnimation("中ダメージ空中", static_cast< const char* >(nullptr));
        }

        if (mTimer > 60) {
            TVec3f killed;
            if (MR::isNearZero(MR::vecKillElement(mActor->getLastMove(), mActor->_240, &killed))) {
                return false;
            }
        }

        if (getPlayer()->getMovementStates()._1 || MR::isNearZero(getPlayer()->mVerticalSpeed)) {
            getPlayer()->mMovementStates.jumping = false;
            getPlayer()->mMovementStates._D = true;

            if (getPlayer()->damagePolygonCheck(getPlayer()->getGroundPolygon())) {
                getPlayer()->_1C._16 = true;
                _24 = true;
                return false;
            }

            if (_14 != 2 || mTimer >= 3) {
                playSound("吹っ飛び倒れ");
                changeAnimation("壁ヒット着地", static_cast< const char* >(nullptr));
                playEffect("共通壁ヒット着地");
                MR::vecKillElement(_18, mActor->_240, &_18);
            }
            
            _14 = 1;
            mTimer = 0;
        }
        break;
    case 1:
        if (!getPlayer()->getMovementStates()._1 && !MR::isNearZero(getPlayer()->mVerticalSpeed)) {
            _25 = true;
            return false;
        }

        addVelocity(_18);
        _18.mult(0.95f);

        if (!isAnimationRun("壁ヒット着地")) {
            return false;
        }

        if (mTimer > 15 && checkTrgA()) {
            getPlayer()->stopJump();
            getPlayer()->tryJump();
            _24 = true;
            return false;
        }
        break;
    }

    f32 dot = vecKillActor240(_18);

    if (_18.length() > 10.0f) {
        _18.mult(0.5f);
    }

    _18 += mActor->_240 * MR::clamp(dot, 0.0f, 40.0f);
    getPlayer()->setJumpVec(_18);
    return true;
}
