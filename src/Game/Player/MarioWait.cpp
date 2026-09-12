#include "Game/Player/MarioWait.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/MathUtil.hpp"

void MarioAnimator::controlWaitAnimation() {
    if (_78) {
        _78--;
        return;
    }

    TVec3f side;
    Mario* player = getPlayer();
    side.cross(getFrontVec(), player->_368);
    MR::normalizeOrZero(&side);
    if (MR::isNearZero(side)) {
        return;
    }

    TVec3f front;
    front.cross(getPlayer()->_368, side);
    MR::normalizeOrZero(&front);
    f32 sideSlope = side.dot(getPlayer()->getAirGravityVec());
    f32 frontSlope = front.dot(getPlayer()->getAirGravityVec());
    f32 weights[4];
    weights[3] = 0.0f;
    weights[2] = 0.0f;
    weights[1] = 0.0f;
    weights[0] = 0.0f;
    f32 sideBias = 0.0f;
    f32 frontBias = 0.0f;
    if (_16 == 1) {
        sideBias = 0.1f;
    } else if (_16 == 2) {
        frontBias = 0.1f;
    }

    f32 frontMagnitude = __fabsf(frontSlope);
    f32 sideMagnitude = __fabsf(sideSlope);
    if (frontBias + frontMagnitude < sideBias + sideMagnitude) {
        f32 angle = 1.5707964f - marioAcos(sideMagnitude);
        if (angle >= 0.7853982f) {
            angle = 0.7853982f;
        }
        weights[2] = angle / 0.7853982f;
        weights[3] = 1.0f - weights[2];
        if (sideSlope < 0.0f) {
            mXanimePlayer->changeTrackAnimation(2, "坂右ウエイト");
        } else {
            mXanimePlayer->changeTrackAnimation(2, "坂左ウエイト");
        }
        if (getPlayer()->_10._F) {
            forceSetBlendWeight(weights);
            getPlayer()->_10._F = false;
        } else {
            setBlendWeight(weights, mActor->getConst().getTable()->mSlopeAnimBlendRatio);
        }
        _16 = 1;
    } else {
        f32 angle = 1.5707964f - marioAcos(frontMagnitude);
        if (angle >= 0.7853982f) {
            angle = 0.7853982f;
        }
        weights[2] = angle / 0.7853982f;
        weights[3] = 1.0f - weights[2];
        if (frontSlope < 0.0f) {
            mXanimePlayer->changeTrackAnimation(2, "坂前ウエイト");
        } else {
            mXanimePlayer->changeTrackAnimation(2, "坂後ウエイト");
        }
        if (getPlayer()->_10._F) {
            forceSetBlendWeight(weights);
            getPlayer()->_10._F = false;
        } else {
            setBlendWeight(weights, mActor->getConst().getTable()->mSlopeAnimBlendRatio);
        }
        _16 = 2;
    }
    _78 = 4;
}

void MarioAnimator::stopWaitAnimation() {
    if (getPlayerMode() == PlayerMode_Invincible && getPlayer()->mWalkSpeed >= 1.5f) {
        return;
    }
    if (_78) {
        _78--;
        return;
    }
    if (isAnimationRun("基本")) {
        mXanimePlayer->changeTrackAnimation(2, "ラン");
    }
}

bool MarioWait::checkStart() {
    if (mActor->_EA4) {
        _16 = 0;
        return false;
    }
    if (getPlayer()->isStatusActive(MarioStatus_Wait)) {
        return false;
    }
    bool damaged = mActor->_482 || mActor->_481;
    if (damaged) {
        _16 = 0;
        return false;
    }

    bool cancel = false;
    switch (getPlayer()->_960) {
    case 27:
    case 28:
        cancel = true;
        break;
    }
    if (getStickP() != 0.0f || checkLvlA() || checkLvlZ() || mActor->isRequestRush()) {
        cancel = true;
    } else if (getPlayer()->mMovementStates.jumping && getPlayerMode() != PlayerMode_Teresa) {
        cancel = true;
    } else if (getPlayer()->isStatusActive(MarioStatus_Swim)) {
        cancel = true;
    } else if (getPlayer()->isStatusActive(MarioStatus_Hang)) {
        cancel = true;
    } else if (getPlayer()->isStatusActive(MarioStatus_Sukekiyo)) {
        cancel = true;
    } else if (getPlayer()->isStatusActive(MarioStatus_Bury)) {
        cancel = true;
    } else if (getPlayerMode() == PlayerMode_Hopper || isStatusActiveID(MarioStatus_Foo)) {
        cancel = true;
    } else if (getPlayer()->isStatusActive(MarioStatus_FpView)) {
        cancel = true;
    } else if (getPlayer()->isStatusActive(MarioStatus_Magic)) {
        cancel = true;
    } else if (getPlayer()->isStatusActive(MarioStatus_Talk)) {
        cancel = true;
    }
    if (mActor->mBeeWallWalk) {
        cancel = true;
    }
    if (getPlayer()->_1C._5) {
        cancel = true;
    }
    if (getPlayer()->mMovementStates._23) {
        cancel = true;
    }
    if (getPlayer()->mMovementStates._A) {
        cancel = true;
    }
    if (getPlayer()->mDrawStates.mIsUnderwater) {
        cancel = true;
    }
    if (!MR::isNearZero(getPlayer()->_184)) {
        cancel = true;
    }
    if (MR::isNearZero(getPlayer()->mWalkSpeed)) {
        _16++;
    } else {
        cancel = true;
    }
    if (cancel) {
        _16 = 0;
        return false;
    }
    if (_16 == 1800) {
        if (MR::getAreaObj("NonSleepCube", getTrans())) {
            _12 = 1;
            return true;
        }
        _12 = 0;
        return true;
    }
    return false;
}

bool MarioWait::start() {
    switch (_12) {
    case 0:
        changeAnimation("特殊ウエイト1A", static_cast< const char* >(nullptr));
        break;
    case 1:
        changeAnimation("戦闘ウエイト", static_cast< const char* >(nullptr));
        break;
    }
    _16 = 0;
    _14 = 0;
    return true;
}

bool MarioWait::update() {
    if (getStickP() != 0.0f || checkLvlA() || checkLvlZ() || mActor->isRequestJump2P()) {
        stopAnimation(nullptr, static_cast< const char* >(nullptr));
        getPlayer()->mainMove();
        return false;
    }

    bool cancel = false;
    if (mActor->isRequestRush()) {
        cancel = true;
    }
    if (getPlayer()->isStatusActive(MarioStatus_FpView)) {
        cancel = true;
    }
    if (getPlayer()->_1C._5) {
        cancel = true;
    }
    if (getPlayerMode() != PlayerMode_Teresa && !getPlayer()->mMovementStates._1) {
        cancel = true;
    }
    if (!MR::isNearZero(getPlayer()->_184)) {
        cancel = true;
    }
    if (cancel) {
        stopAnimation(nullptr, static_cast< const char* >(nullptr));
        return false;
    }

    _16++;
    switch (_12) {
    case 0:
        switch (_14) {
        case 0:
            if (!mActor->_468) {
                u32 duration = 1800;
                if (isPlayerModeTeresa()) {
                    duration = 210;
                }
                if (_16 == duration) {
                    _16 = 0;
                    _14++;
                    changeAnimation("特殊ウエイト1B", static_cast< const char* >(nullptr));
                }
            }
            break;
        case 1:
            break;
        }
        break;
    case 1:
        break;
    }
    return true;
}

bool MarioWait::close() {
    stopSound("声あくび", 0);
    stopSound("声いびき１", 0);
    stopSound("声いびき２", 0);
    _16 = 0;
    return true;
}

bool Mario::isBlendWaitGround() const {
    return calcAngleD(_368) > 8.0f;
}

void Mario::checkSpecialWaitAnimation() {
    if (mWait->checkStart()) {
        changeStatus(mWait);
    }
}

void Mario::resetSleepTimer() {
    mWait->_16 = 0;
    if (isStatusActive(MarioStatus_Wait)) {
        closeStatus(mWait);
    }
}

MarioWait::MarioWait(MarioActor* actor) : MarioState(actor, MarioStatus_Wait) {
    _16 = 0;
    _12 = 0;
    _14 = 0;
}

void MarioState::hitPoly(u8, const TVec3f&, HitSensor*) {
}

bool MarioState::keep() {
    return true;
}

bool MarioState::notice() {
    return false;
}

void MarioState::init() {
}
