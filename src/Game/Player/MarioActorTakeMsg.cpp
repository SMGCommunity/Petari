#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioSearchLight.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include <cstring>

void MarioActor::memorizeSensorThrow(HitSensor* pSensor) {
    _428[_468] = pSensor;
    _438[_468] = pSensor->mHost->mRotation - mRotation;

    _424 = nullptr;
    _468++;
    _490 = 0.0f;
    _EF6 = 30;

    mMario->startHandy();

    if (pSensor == _470) {
        _470 = nullptr;
    }

    if (pSensor == _46C) {
        _46C = nullptr;
    }
}

bool MarioActor::tryThrow() {
    if (mMario->getMovementStates().debugMode) {
        return false;
    }

    if (_468 == 0) {
        return false;
    }

    if (IsMarioSwimming()) {
        return false;
    }

    if (mMario->isStatusActive(MarioStatus_Bury)) {
        return false;
    }

    if (!isRequestThrow()) {
        return false;
    }

    HitSensor* pSensor = _428[_468 - 1];

    if (releaseThrowMemoSensor()) {
        mMario->mSwim->resetJet();
        mMarioAnim->changeThrowAnimation(pSensor);

        _F1C = 30;
        mVelocity.set2(0.0f);

        if (isJumping()) {
            _38C = 1;
            jumpHop();
        } else {
            _38C = 15;
        }

        offTakingFlag();

        return true;
    }

    return false;
}

void MarioActor::tryReleaseDirect(const HitSensor* pSensor) {
    u32 maxIdx = _468;
    for (s32 idx = 0; idx < maxIdx; idx++) {
        if (_428[idx] == pSensor) {
            _428[idx] = nullptr;
            _468--;

            continue;
        }

        if (idx == 0) {
            continue;
        }

        for (u32 innerIdx = 0; innerIdx < idx; innerIdx++) {
            if (_428[innerIdx] == nullptr) {
                _428[innerIdx] = _428[idx];
                _428[idx] = nullptr;

                break;
            }
        }
    }

    if (_474 == pSensor) {
        _474 = nullptr;
    }

    if (_468 == 0) {
        mMario->stopAnimationUpper(nullptr, nullptr);
        mMario->mSwim->resetJet();

        offTakingFlag();

        clearNullAnimation(0);
    }
}

void MarioActor::damageDropThrowMemoSensor() {
    tryReleaseWithMsg(ACTMES_DAMAGEDROP);
    updateSearchLight();
    tryReleaseBombTeresa();

    if (_424 != nullptr) {
        _424->receiveMessage(ACTMES_DAMAGEDROP, getSensor("body"));
        _424 = nullptr;
    }
}

void MarioActor::rushDropThrowMemoSensor() {
    tryReleaseWithMsg(ACTMES_RUSHDROP);
    updateSearchLight();

    if (_424 != nullptr) {
        _424->receiveMessage(ACTMES_DAMAGEDROP, getSensor("body"));
        _424 = nullptr;
    }
}

void MarioActor::trySetLockOnTarget(HitSensor* pSensor) {
    HitSensor* sensor = _468 == 0 ? nullptr : _428[0];

    if (sensor == pSensor) {
        return;
    }

    if (isJumping() && mMario->_430 == 5) {
        return;
    }

    TVec3f vec = pSensor->mPosition - mPosition;
    if (vec.length() > 500.0f) {
        return;
    }

    MR::vecKillElement(vec, getGravityVec(), &vec);

    if (MR::normalizeOrZero(&vec)) {
        return;
    }

    TVec3f front;
    getFrontVec(&front);
    MR::vecKillElement(front, getGravityVec(), &front);

    if (MR::normalizeOrZero(&front)) {
        return;
    }

    f32 diffAngle = MR::diffAngleAbsFast(vec, front) * _180_PI;
    if (_46C == nullptr || diffAngle < _478) {
        _46C = pSensor;
        _478 = diffAngle;
    }
}

void MarioActor::tryCoinPull() {
    for (u32 idx = 0; idx < ARRAY_SIZE(_4D0); idx++) {
        if (_4D0[idx] == nullptr) {
            continue;
        }

        if (MR::isDead(_4D0[idx]->mHost)) {
            _4D0[idx] = nullptr;
        } else {
            tryCoinPullOne(_4D0[idx]);
        }
    }
}

void MarioActor::tryCoinPullInRush() {
    for (u32 idx = 0; idx < _7DC; idx++) {
        if (_6DC[idx] == nullptr) {
            continue;
        }

        if (MR::isDead(_6DC[idx]->mHost)) {
            _6DC[idx] = nullptr;
        } else {
            tryTornadoPull(_6DC[idx]);
        }
    }
}

void MarioActor::flushCoinPull() {
    for (u32 idx = 0; idx < ARRAY_SIZE(_4D0); idx++) {
        if (_4D0[idx] == nullptr) {
            continue;
        }

        if (MR::isDead(_4D0[idx]->mHost)) {
            _4D0[idx] = nullptr;
        } else {
            tryGetItem(_4D0[idx]);
            _4D0[idx] = nullptr;
        }
    }
}

bool MarioActor::tryCoinPullOne(HitSensor* pSensor) NO_INLINE {
    tryPullTrans(&pSensor->mHost->mVelocity, pSensor->mPosition);

    return pSensor->receiveMessage(ACTMES_ITEM_PULL, getSensor("body"));
}

void MarioActor::tryPullTrans(TVec3f* velocity, const TVec3f& position) {
    TVec3f center(_2A0);
    TVec3f ground;
    getGroundPos(&ground);
    if (IsMarioSwimming()) {
        ground = center - _4B8 * 600.0f;
    } else if (mMario->mMovementStates._1) {
        TVec3f offset = ground - center;
        f32 height = MR::vecKillElement(offset, mMario->_368, &offset);
        ground = center + mMario->_368 * height;
    }
    TVec3f foot;
    f32 projection = MR::getFootPoint(center, ground, position, &foot);
    TVec3f inward = foot - position;
    f32 radius = inward.length();
    if (radius > 600.0f) {
        radius = 600.0f;
    }
    TVec3f speed;
    speed.y = 400000.0f / (10000.0f + radius * radius);
    f32 height = __fabsf(projection * (ground - center).length());
    if (height < speed.y) {
        speed.y = height;
    }
    if (projection < 0.0f) {
        speed.y = -speed.y;
    }
    if (!IsMarioSwimming()) {
        speed.y = 0.4f * speed.y;
    }
    TVec3f axis = ground - center;
    MR::normalizeOrZero(&axis);
    MR::normalizeOrZero(&inward);
    TVec3f tangent;
    tangent.cross(axis, inward);
    speed.z = (0.01f + 0.04f * (600.0f - radius) / 600.0f) * (2.0f * (PI * radius)) * getConst().getTable()->mCoinPullAngleSpeedRatio;
    TVec3f nextPosition = position + tangent * speed.z;
    TVec3f pull = foot - nextPosition;
    f32 nextRadius = pull.length();
    MR::normalizeOrZero(&pull);
    f32 pullSpeed = radius / 25.0f;
    if (_934) {
        pullSpeed += 50.0f;
    }
    pullSpeed *= getConst().getTable()->mCoinPullDistSpeedRatio;
    if (radius < pullSpeed) {
        pullSpeed = radius;
    }
    speed.x = pullSpeed + (nextRadius - radius);
    *velocity = pull * speed.x - axis * speed.y + tangent * speed.z;
}

bool MarioActor::releaseThrowMemoSensor() {
    // FIXME: missing class at 0x988 that has HitSensor* at 0xA4
    if (_468 != 0) {
        if (mMario->isStatusActive(MarioStatus_Wall) || mMario->isStatusActive(MarioStatus_SideStep)) {
            return false;
        }

        if (mMario->isSwimming() && mSearchLight->_A4 != nullptr) {
            _470 = mSearchLight->_A4;
        }

        s32 idx = _468 - 1;
        if (_470 != nullptr) {
            TVec3f diff = _470->mPosition - _2A0;

            if (mMario->isSwimming()) {
                TVec3f throwVec;
                getThrowVec(&throwVec);
                if (MR::diffAngleAbs(diff, throwVec) > PI / 3.0f) {
                    _470 = nullptr;
                }
            } else if (MR::diffAngleAbs(diff, mMario->mFrontVec) > PI / 3.0f) {
                _470 = nullptr;
            }
        }

        if (_470 == nullptr || !mMario->isSwimming()) {
            if (_F24 != nullptr) {
                if (!_428[idx]->receiveMessage(ACTMES_THROW, _F24)) {
                    return false;
                }
            } else if (!_428[idx]->receiveMessage(ACTMES_THROW, getSensor("body"))) {
                return false;
            }

            _474 = 0;
        } else {
            if (!_428[idx]->receiveMessage(ACTMES_THROW, _470)) {
                return false;
            }

            _474 = _470;
        }

        _468--;

        if (_468 == 0) {
            _470 = nullptr;
            _47C = 0;
            _428[0] = nullptr;
        }

        clearNullAnimation(0);

        _3AC = 60;

        return true;
    }

    return false;
}

void MarioActor::tryReleaseWithMsg(u32 msg) {
    u32 maxIdx = _468;
    if (maxIdx == 0) {
        if (isAnimationRun("ひろいクイック")) {
            mMario->stopAnimationUpperForce();
            clearNullAnimation(0);
            offTakingFlag();
        }

        return;
    }

    for (s32 idx = 0; idx < maxIdx; idx++) {
        if (!_428[idx]->receiveMessage(msg, getSensor("body"))) {
            if (idx == 0) {
                continue;
            }

            for (u32 innerIdx = 0; innerIdx < idx; innerIdx++) {
                if (_428[innerIdx] == nullptr) {
                    _428[innerIdx] = _428[idx];
                    _428[idx] = nullptr;

                    break;
                }
            }
        } else {
            _428[idx] = nullptr;
            _468--;
        }
    }

    _474 = nullptr;

    if (_468 == 0) {
        mMario->stopAnimationUpperForce();
        mMario->mSwim->resetJet();
    }

    if (_480) {
        mMario->stopAnimationUpperForce();
        offTakingFlag();
    }

    clearNullAnimation(0);
}

void MarioActor::tryTornadoPull(HitSensor* sensor) {
    if (!isActionOk("コイン引っ張り")) {
        return;
    }
    s32 type = sensor->mType;
    if (strcmp(sensor->mHost->mName, "カメックビーム用カメ") == 0) {
        type = 5555;
    }
    switch (type) {
    case ATYPE_COIN_RED:
        break;
    case ATYPE_COIN:
    case ATYPE_STAR_PIECE: {
        if (mPlayerMode == 7 || IsMarioSwimming()) {
            TVec3f center(_2A0);
            TVec3f ground;
            getGroundPos(&ground);
            if (IsMarioSwimming()) {
                ground = center - _4B8 * 600.0f;
            }
            center += mMario->mHeadVec * 200.0f;
            TVec3f position(sensor->mPosition);
            TVec3f foot;
            f32 projection = MR::getFootPoint(center, ground, position, &foot);
            if (projection < -0.1f) {
                return;
            }
            if (projection > 2.0f) {
                return;
            }
            TVec3f offset = foot - position;
            if (offset.length() > 600.0f) {
                return;
            }
            if (MR::isExistMapCollision(_2A0, sensor->mPosition - _2A0)) {
                return;
            }
            for (s32 i = 0; i < 128; i++) {
                if (_4D0[i] == sensor) {
                    return;
                }
            }
            if (sensor->receiveMessage(ACTMES_ITEM_PULL, getSensor("body"))) {
                for (s32 i = 0; i < 128; i++) {
                    if (_4D0[i] == nullptr) {
                        _4D0[i] = sensor;
                        return;
                    }
                }
            }
        }
        break;
    }
    case ATYPE_JET_TURTLE:
    case ATYPE_JET_TURTLE_SLOW:
        if (isActionOk("カメ持ち") && sensor->receiveMessage(ACTMES_IS_PULL_ENABLE, getSensor("body"))) {
            if ((sensor->mPosition - mPosition).length() < 120.0f) {
                tryGetItem(sensor);
                return;
            }
            tryCoinPullOne(sensor);
            _424 = sensor;
        }
        break;
    case 5555:
        if (isActionOk("カメ持ち") && sensor->receiveMessage(ACTMES_IS_PULL_ENABLE, getSensor("body"))) {
            tryCoinPullOne(sensor);
        }
        break;
    }
}

void MarioActor::tryReleaseBombTeresa() {
    if (_B94 == 0) {
        return;
    }

    _B94 = 0;

    mMario->stopAnimationUpper(nullptr, nullptr);

    if (!mMario->getMovementStates().jumping) {
        MarioStopWalk();
        _38C = 5;
        mMario->_420 = 25;
    }

    changeAnimation("ハンマー投げリリース", nullptr);
}
