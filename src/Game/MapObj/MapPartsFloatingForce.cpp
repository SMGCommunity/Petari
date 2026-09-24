#include "Game/MapObj/MapPartsFloatingForce.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/FloaterFunction.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/SpringValue.hpp"

namespace NrvMapPartsFloatingForce {
    NEW_NERVE(HostTypeWait, MapPartsFloatingForce, Wait);
    NEW_NERVE(HostTypeMove, MapPartsFloatingForce, Move);
    NEW_NERVE(HostTypeMoveSpring, MapPartsFloatingForce, MoveSpring);
    NEW_NERVE(HostTypeMoveReturn, MapPartsFloatingForce, MoveReturn);
}  // namespace NrvMapPartsFloatingForce

void MapPartsFloatingForce_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)3.0f;
}

SpringStep::SpringStep(f32 cond, f32 speed, f32 angle) : mSpringValue() {
    mCondition = cond;
    mSpeed = speed;
    mAngle = angle;
    mSpringValue = new SpringValue(cond, 0.0f, speed, angle, 0.0f);
}

void SpringStep::addSpringVelocity(f32 vel) {
    mSpringValue->mVelocity += vel;
}

void SpringStep::setSpringBaseValue(f32 val) {
    mCondition = val;
    mSpringValue->setParam(static_cast< f64 >(val), 0.0f, mSpeed, mAngle, 0.0f);
}

MapPartsFloatingForce::MapPartsFloatingForce(LiveActor* pActor)
    : MapPartsFunction(pActor, "浮力"), mSpringStep(), mObjectName(), _20(), mRotateAngle(), mRotateSpeed(), mRotateAccelType(), _30(gZeroVec),
      mUpDirection(0.0f, 1.0f, 0.0f), _48(), _4C() {
}

void MapPartsFloatingForce::init(const JMapInfoIter& rIter) {
    initNerve(GET_NERVE(MapPartsFloatingForce, HostTypeWait));
    MR::getMapPartsArgRotateAccelType(&mRotateAccelType, rIter);

    if (mRotateAccelType == 1) {
        initForSpringMotion(rIter);
    } else {
        initForNormalMotion(rIter);
    }

    MR::useStageSwitchReadA(mHost, rIter);
}

void MapPartsFloatingForce::setObjectName(const char* pName) {
    mObjectName = pName;
}

void MapPartsFloatingForce::initForNormalMotion(const JMapInfoIter& rIter) {
    s32 conditionType = 0;
    MR::getMapPartsArgMoveConditionType(&conditionType, rIter);
    _20 = conditionType;
    MR::getMapPartsArgRotateSpeed(&mRotateSpeed, rIter);
    MR::getMapPartsArgRotateAngle(&mRotateAngle, rIter);
}

void MapPartsFloatingForce::initForSpringMotion(const JMapInfoIter& rIter) {
    s32 conditionType = 0;
    MR::getMapPartsArgMoveConditionType(&conditionType, rIter);
    _20 = conditionType;
    f32 rotateSpeed = 0.0f;
    MR::getMapPartsArgRotateSpeed(&rotateSpeed, rIter);
    rotateSpeed *= 0.001f;
    f32 rotateAngle = 0.0f;
    MR::getMapPartsArgRotateAngle(&rotateAngle, rIter);
    rotateAngle *= 0.001f;
    s32 rotateAxis = 0;
    MR::getMapPartsArgRotateAxis(&rotateAxis, rIter);
    _5C = 0.1f * rotateAxis;
    mSpringStep = new SpringStep(-conditionType, rotateSpeed, rotateAngle);
}

void MapPartsFloatingForce::start() {
    _30.set(mHost->mPosition);
    TPos3f baseMtx;
    baseMtx.set(mHost->getBaseMtx());
    baseMtx.getYDir(mUpDirection);
    MR::normalize(&mUpDirection);
    if (mRotateAccelType != 1) {
        setNerve(GET_NERVE(MapPartsFloatingForce, HostTypeMove));
    }
}

void MapPartsFloatingForce::updateVelocity() {
    if (MR::isOnPlayer(MR::getBodySensor(mHost))) {
        if (_20 <= _48) {
            _4C = MR::converge< f32 >(_4C, 0.0f, 0.05f);
        } else {
            _4C = _4C + (0.0001f * mRotateSpeed);
        }
    } else {
        if (_48 <= 0.0f) {
            _4C = MR::converge< f32 >(_4C, 0.0f, 0.05f);
        } else {
            _4C = _4C - (0.0001f * mRotateAngle);
        }
    }

    _4C *= 0.98f;
    _4C = MR::clamp(_4C, -50.0f, 50.0f);
}

void MapPartsFloatingForce::updateVelocitySpring() {
    f32 velocity = mSpringStep->mSpringValue->mVelocity;
    bool isSwitchOn = false;
    if (MR::isValidSwitchA(mHost) && MR::isOnSwitchA(mHost)) {
        isSwitchOn = true;
    }

    if (isSwitchOn) {
        velocity *= 3.0f;
    }

    _50.set(mUpDirection * velocity);
    if (MR::isNearZero(_50)) {
        _50.zero();
    }

    mSpringStep->mSpringValue->update();
}

bool MapPartsFloatingForce::tryOn() {
    bool isSwitchOn = false;

    if (MR::isValidSwitchA(mHost) && MR::isOnSwitchA(mHost)) {
        isSwitchOn = true;
    }

    if (isSwitchOn) {
        return false;
    }

    if (MR::isOnPlayer(MR::getBodySensor(mHost))) {
        const char* pGroundSound = FloaterFunction::getSeGroundOn(mObjectName);
        if (pGroundSound != nullptr) {
            MR::startSound(mHost, pGroundSound);
        }

        setNerve(GET_NERVE(MapPartsFloatingForce, HostTypeMoveSpring));
        return true;
    }

    return false;
}

bool MapPartsFloatingForce::tryReturn() {
    bool isSwitchOn = false;

    if (MR::isValidSwitchA(mHost) && MR::isOnSwitchA(mHost)) {
        isSwitchOn = true;
    }

    if (!isSwitchOn) {
        if (MR::isOnPlayer(MR::getBodySensor(mHost))) {
            return false;
        }
    }

    setNerve(GET_NERVE(MapPartsFloatingForce, HostTypeMoveReturn));
    return true;
}

void MapPartsFloatingForce::exeWait() {
    if (isFirstStep()) {
        _48 = 0.0f;
    }

    if (mRotateAccelType == 1) {
        tryOn();
    }
}

void MapPartsFloatingForce::exeMove() {
    updateVelocity();
    _50.set(-mUpDirection * _4C);
    _48 += _4C;
    _48 = MR::clamp(_48, 0.0f, _20);
    TVec3f* const pPosition = &mHost->mPosition;
    pPosition->set(_30 - mUpDirection * _48);
    if (mObjectName != nullptr) {
        if (_4C > 0.1f) {
            const char* pSound = FloaterFunction::getSeMoveDown(mObjectName);
            if (pSound != nullptr) {
                MR::startLevelSound(mHost, pSound);
            }
        } else if (_4C < -0.1f) {
            const char* pSound = FloaterFunction::getSeMoveUp(mObjectName);
            if (pSound != nullptr) {
                MR::startLevelSound(mHost, pSound);
            }
        }
    }
}

void MapPartsFloatingForce::exeMoveSpring() {
    if (isFirstStep()) {
        mSpringStep->setSpringBaseValue(-_20);
        mSpringStep->addSpringVelocity(-_5C);
    }

    updateVelocitySpring();
    mHost->mVelocity.set(_50);
    tryReturn();
}

void MapPartsFloatingForce::exeMoveReturn() {
    if (isFirstStep()) {
        mSpringStep->setSpringBaseValue((mHost->mPosition - _30).length());
    }

    updateVelocitySpring();
    mHost->mVelocity.set(_50);
    if (mObjectName != nullptr && 0.1f != _4C) {
        const char* pSound = FloaterFunction::getSeMoveUp(mObjectName);
        if (pSound != nullptr) {
            MR::startLevelSound(mHost, pSound);
        }
    }

    if (MR::isNearZero(_30 - mHost->mPosition) && MR::isNearZero(_50)) {
        mHost->mPosition.set(_30);
        setNerve(GET_NERVE(MapPartsFloatingForce, HostTypeWait));
    } else {
        tryOn();
    }
}

MapPartsFloatingForce::~MapPartsFloatingForce() {
}
