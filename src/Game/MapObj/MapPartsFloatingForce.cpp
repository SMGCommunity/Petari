#include "Game/MapObj/MapPartsFloatingForce.hpp"
#include "Game/MapObj/FloaterFunction.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util.hpp"

SpringStep::SpringStep(f32 cond, f32 speed, f32 angle) {
    mSpringValue = 0;
    mCondition = cond;
    mSpeed = speed;
    mAngle = angle;
    mSpringValue = new SpringValue(cond, 0.0f, speed, angle, 0.0f);
}

void SpringStep::addSpringVelocity(f32 vel) {
    mSpringValue->_10 += vel;
}

void SpringStep::setSpringBaseValue(f32 val) {
    mCondition = val;
    mSpringValue->setParam((f64)val, 0.0f, mSpeed, mAngle, 0.0f);
}

MapPartsFloatingForce::MapPartsFloatingForce(LiveActor *pActor) : MapPartsFunction(pActor, "浮力") {
    mSpringStep = 0;
    mObjectName = 0;
    _20 = 0.0f;
    mRotateAngle = 0.0f;
    mRotateSpeed = 0.0f;
    mRotateAccelType = 0;
    _30.setZero();
    _3C = 0.0f;
    _40 = 1.0f;
    _44 = 0.0f;
    _48 = 0.0f;
    _5C = 0.0f;
}

void MapPartsFloatingForce::init(const JMapInfoIter &rIter) {
    initNerve(&NrvMapPartsFloatingForce::HostTypeWait::sInstance);
    MR::getMapPartsArgRotateAccelType(&mRotateAccelType, rIter);

    if (mRotateAccelType != 1) {
        initForNormalMotion(rIter);
    }
    else {
        initForSpringMotion(rIter);
    }

    MR::useStageSwitchReadA(mHost, rIter);
}

void MapPartsFloatingForce::setObjectName(const char *pName) {
    mObjectName = pName;
}

void MapPartsFloatingForce::initForNormalMotion(const JMapInfoIter &rIter) {
    s32 condition_type = 0;
    MR::getMapPartsArgMoveConditionType(&condition_type, rIter);
    _20 = condition_type;
    MR::getMapPartsArgRotateSpeed(&mRotateSpeed, rIter);
    MR::getMapPartsArgRotateAngle(&mRotateAngle, rIter);
}

void MapPartsFloatingForce::initForSpringMotion(const JMapInfoIter &rIter) {
    s32 condition_type = 0;
    MR::getMapPartsArgMoveConditionType(&condition_type, rIter);
    _20 = condition_type;
    f32 rotate_speed = 0.0f;
    MR::getMapPartsArgRotateSpeed(&rotate_speed, rIter);
    rotate_speed *= 0.001f;
    f32 rotate_angle = 0.0f;
    MR::getMapPartsArgRotateAngle(&rotate_angle, rIter);
    rotate_angle *= 0.001f;
    s32 rotate_axis = 0;
    MR::getMapPartsArgRotateAxis(&rotate_axis, rIter);
    _5C = 0.1f * rotate_axis;
    mSpringStep = new SpringStep(-condition_type, rotate_speed, rotate_angle);
}

void MapPartsFloatingForce::updateVelocity() {
    if (MR::isOnPlayer(MR::getBodySensor(mHost))) {
        if (_20 <= _48) {
            _4C = MR::converge<f32>(_4C, 0.0f, 0.050000001f);
        }
        else {
            _4C = _4C + (0.000099999997f * mRotateSpeed);
        }
    }
    else {
        if (_48 <= 0.0f) {
            _4C = MR::converge<f32>(_4C, 0.0f, 0.050000001f);
        }
        else {
            _4C = _4C - (0.000099999997f * mRotateAngle);
        }
    }

    _4C *= 0.98000002f;
    _4C = MR::clamp(_4C, -50.0f, 50.0f);
}

// void MapPartsFloatingForce::updateVelocitySpring()

bool MapPartsFloatingForce::tryOn() {
    bool isSwitchOn = false;

    if (MR::isValidSwitchA(mHost) && MR::isOnSwitchA(mHost)) {
        isSwitchOn = true;
    }

    if (isSwitchOn) {
        return false;
    }

    if (MR::isOnPlayer(MR::getBodySensor(mHost))) {
        const char* ground_sound = FloaterFunction::getSeGroundOn(mObjectName);
        if (ground_sound) {
            MR::startSound(mHost, ground_sound, -1, -1);
        }

        setNerve(&NrvMapPartsFloatingForce::HostTypeMoveSpring::sInstance);
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

    setNerve(&NrvMapPartsFloatingForce::HostTypeMoveReturn::sInstance);
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

// MapPartsFloatingForce::exeMove

void MapPartsFloatingForce::exeMoveSpring() {
    if (isFirstStep()) {
        mSpringStep->setSpringBaseValue(-_20);
        mSpringStep->addSpringVelocity(-_5C);
    }

    updateVelocitySpring();
    mHost->mVelocity.set(_50);
    tryReturn();
}

// MapPartsFloatingForce::exeMoveReturn

MapPartsFloatingForce::~MapPartsFloatingForce() {

}

namespace NrvMapPartsFloatingForce {
    INIT_NERVE(HostTypeWait);
    INIT_NERVE(HostTypeMove);
    INIT_NERVE(HostTypeMoveSpring);
    INIT_NERVE(HostTypeMoveReturn);

    void HostTypeMoveReturn::execute(Spine *pSpine) const {
        MapPartsFloatingForce* force = reinterpret_cast<MapPartsFloatingForce*>(pSpine->mExecutor);
        force->exeMoveReturn();
    }

    void HostTypeMoveSpring::execute(Spine *pSpine) const {
        MapPartsFloatingForce* force = reinterpret_cast<MapPartsFloatingForce*>(pSpine->mExecutor);
        force->exeMoveSpring();
    }

    void HostTypeMove::execute(Spine *pSpine) const {
        MapPartsFloatingForce* force = reinterpret_cast<MapPartsFloatingForce*>(pSpine->mExecutor);
        force->exeMove();
    }

    void HostTypeWait::execute(Spine *pSpine) const {
        MapPartsFloatingForce* force = reinterpret_cast<MapPartsFloatingForce*>(pSpine->mExecutor);
        force->exeWait();
    }
};