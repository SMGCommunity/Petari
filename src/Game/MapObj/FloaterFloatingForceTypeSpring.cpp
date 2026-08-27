#include "Game/MapObj/FloaterFloatingForceTypeSpring.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/FloaterFunction.hpp"
#include "Game/Util.hpp"

void FloaterSpringForce_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)3.0f;
}

namespace {
    static const f32 sSpeedMin = 0.1f;
};  // namespace

namespace NrvFloaterFloatingForceTypeSpring {
    NEW_NERVE(HostTypeWait, FloaterFloatingForceTypeSpring, Wait);
    NEW_NERVE(HostTypeMove, FloaterFloatingForceTypeSpring, Move);
    NEW_NERVE(HostTypeMoveReturn, FloaterFloatingForceTypeSpring, MoveReturn);
};  // namespace NrvFloaterFloatingForceTypeSpring

FloaterFloatingForceTypeSpring::~FloaterFloatingForceTypeSpring() {
}

FloaterSpringForce::FloaterSpringForce(f32 param1, f32 param2, f32 param3) : mSpringValue() {
    _0 = param1;
    _4 = param2;
    _8 = param3;
    mSpringValue = new SpringValue(param1, 0.0f, param2, param3, 0.0f);
}

void FloaterSpringForce::addSpringVelocity(f32 vel) {
    mSpringValue->mVelocity += vel;
}

void FloaterSpringForce::setSpringBaseValue(f32 param1) {
    _0 = param1;

    mSpringValue->setParam(_0, 0.0f, _4, _8, 0.0f);
}

FloaterFloatingForceTypeSpring::FloaterFloatingForceTypeSpring(LiveActor* pHost, const char* pName)
    : FloaterFloatingForce(pHost, pName), mSpringForce(), mVelocity(gZeroVec), _48() {
}

void FloaterFloatingForceTypeSpring::init(const JMapInfoIter& rIter) {
    FloaterFloatingForce::init(rIter);

    f32 rotateSpeed = 0.0f;
    MR::getMapPartsArgRotateSpeed(&rotateSpeed, rIter);
    rotateSpeed *= 0.001f;

    f32 rotateAngle = 0.0f;
    MR::getMapPartsArgRotateAngle(&rotateAngle, rIter);
    rotateAngle *= 0.001f;

    s32 rotateAxis = 0;
    MR::getMapPartsArgRotateAxis(&rotateAxis, rIter);
    _48 = rotateAxis * 0.1f;

    mSpringForce = new FloaterSpringForce(-mMoveConditionType, rotateSpeed, rotateAngle);

    initNerve(&NrvFloaterFloatingForceTypeSpring::HostTypeWait::sInstance);
    MR::useStageSwitchReadA(mHost, rIter);
}

void FloaterFloatingForceTypeSpring::start() {
    FloaterFloatingForce::start();
    setNerve(&NrvFloaterFloatingForceTypeSpring::HostTypeWait::sInstance);
}

void FloaterFloatingForceTypeSpring::updateVelocity() {
    f32 sinkSpeed = mSpringForce->mSpringValue->mVelocity;
    bool isValidAndOnSwitchA = MR::isValidSwitchA(mHost) && MR::isOnSwitchA(mHost);

    if (isValidAndOnSwitchA) {
        sinkSpeed *= 3.0f;
    }

    mVelocity.set(_28 * sinkSpeed);

    if (MR::isNearZero(sinkSpeed)) {
        mVelocity.zero();
    }

    mSpringForce->mSpringValue->update();
}

void FloaterFloatingForceTypeSpring::updateHostVelocity(TVec3f* pDst) const {
    pDst->set(getCurrentVelocity());
}

void FloaterFloatingForceTypeSpring::soundMoveSE() {
    if (_18 == nullptr) {
        return;
    }

    if (getCurrentSinkSpeed() > ::sSpeedMin) {
        const char* seMoveDown = FloaterFunction::getSeMoveDown(_18);

        if (seMoveDown != nullptr) {
            MR::startLevelSound(mHost, seMoveDown);
        }
    } else if (getCurrentSinkSpeed() < -::sSpeedMin) {
        const char* seMoveUp = FloaterFunction::getSeMoveUp(_18);

        if (seMoveUp != nullptr) {
            MR::startLevelSound(mHost, seMoveUp);
        }
    }
}

f32 FloaterFloatingForceTypeSpring::getCurrentSinkSpeed() const {
    return mSpringForce->mSpringValue->mVelocity;
}

bool FloaterFloatingForceTypeSpring::tryOn() {
    bool isValidAndOnSwitchA = MR::isValidSwitchA(mHost) && MR::isOnSwitchA(mHost);

    if (isValidAndOnSwitchA) {
        return false;
    }

    if (!MR::isOnPlayer(mHost)) {
        return false;
    }

    setNerve(&NrvFloaterFloatingForceTypeSpring::HostTypeMove::sInstance);

    return true;
}

bool FloaterFloatingForceTypeSpring::tryReturn() {
    bool isValidAndOnSwitchA = MR::isValidSwitchA(mHost) && MR::isOnSwitchA(mHost);

    if (!isValidAndOnSwitchA && MR::isOnPlayer(mHost)) {
        return false;
    }

    setNerve(&NrvFloaterFloatingForceTypeSpring::HostTypeMoveReturn::sInstance);

    return true;
}

void FloaterFloatingForceTypeSpring::exeWait() {
    tryOn();
}

void FloaterFloatingForceTypeSpring::exeMove() {
    if (isFirstStep()) {
        mSpringForce->setSpringBaseValue(-mMoveConditionType);
        mSpringForce->addSpringVelocity(-_48);
    }

    updateVelocity();
    soundMoveSE();
    tryReturn();
}

void FloaterFloatingForceTypeSpring::exeMoveReturn() {
    if (isFirstStep()) {
        mSpringForce->setSpringBaseValue((mHost->mPosition - _1C).length());
    }

    updateVelocity();

    if (_18 != nullptr && getCurrentSinkSpeed() != ::sSpeedMin) {
        const char* seMoveUp = FloaterFunction::getSeMoveUp(_18);

        if (seMoveUp != nullptr) {
            MR::startLevelSound(mHost, seMoveUp);
        }
    }

    if (MR::isNearZero(_1C - mHost->mPosition)) {
        if (MR::isNearZero(getCurrentSinkSpeed())) {
            mHost->mPosition.set(_1C);
            setNerve(&NrvFloaterFloatingForceTypeSpring::HostTypeWait::sInstance);
            return;
        }
    }

    tryOn();
}
