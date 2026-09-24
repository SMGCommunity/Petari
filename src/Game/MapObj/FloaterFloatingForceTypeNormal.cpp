#include "Game/MapObj/FloaterFloatingForceTypeNormal.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/FloaterFunction.hpp"
#include "Game/Util.hpp"

void FloaterFloatingForceTypeNormal_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)-1.0f;
    (void)0.05f;
    (void)0.0001f;
    (void)0.98f;
    (void)-50.0f;
    (void)50.0f;
    (void)10.0f;
    (void)0.8f;
    (void)0.1f;
    (void)-0.1f;
    (void)0.001f;
}

namespace {
    // static const f32 sSinkSpeedFric = _;
    // static const f32 sBrakeForce = _;
    // static const f32 sSinkSpeedMax = _;
    // static const f32 sKineticConst = _;
    // static const f32 sAttenCoefficient = _;
    // static const f32 sSpeedMin = _;
    // static const f32 sGroundOnSpeed = _;
    // static const s32 sGroundOnDownTime = _;
    // static const s32 sGroundOnReboundTime = _;
    // static const f32 sLandingMomentSpeed = _;
    // static const s32 sLandingMomentTime = _;
};  // namespace

namespace NrvFloaterFloatingForceTypeNormal {
    NEW_NERVE(HostTypeWait, FloaterFloatingForceTypeNormal, Wait);
    NEW_NERVE(HostTypeMoveFirst, FloaterFloatingForceTypeNormal, MoveFirst);
    NEW_NERVE(HostTypeMoveOffPlayer, FloaterFloatingForceTypeNormal, Move);
    NEW_NERVE(HostTypeMoveOnPlayer, FloaterFloatingForceTypeNormal, Move);
};  // namespace NrvFloaterFloatingForceTypeNormal

FloaterFloatingForceTypeNormal::FloaterFloatingForceTypeNormal(LiveActor* pActor, const char* pName)
    : FloaterFloatingForce(pActor, pName), mRotateAngle(), mRotateSpeed(), _40(), _44() {
}

void FloaterFloatingForceTypeNormal::init(const JMapInfoIter& rIter) {
    FloaterFloatingForce::init(rIter);
    MR::getMapPartsArgRotateSpeed(&mRotateSpeed, rIter);
    MR::getMapPartsArgRotateAngle(&mRotateAngle, rIter);
    MR::useStageSwitchReadA(mHost, rIter);
    MapPartsFunction::initNerve(GET_NERVE(FloaterFloatingForceTypeNormal, HostTypeWait));
}

void FloaterFloatingForceTypeNormal::start() {
    FloaterFloatingForce::start();
    setStateMove();
}

void FloaterFloatingForceTypeNormal::updateHostTrans(TVec3f* pDst) const {
    pDst->set(_1C - (_28 * _40));
}

void FloaterFloatingForceTypeNormal::updateVelocity(bool param1) {
    f32 f;

    if (param1 && (MR::isValidSwitchA(mHost) && MR::isOnSwitchA(mHost)) == false) {
        if (mMoveConditionType <= _40) {
            _44 = MR::converge< f32 >(_44, 0.0f, 0.05f);
        } else {
            _44 += mRotateSpeed * 0.0001f;
        }
    } else if (_40 <= 0.0f) {
        _44 = MR::converge< f32 >(_44, 0.0f, 0.05f);
    } else {
        _44 -= mRotateAngle * 0.0001f;
    }

    _44 *= 0.98f;
    _44 = MR::clamp(_44, -50.0f, 50.0f);

    f = _44;

    if (FloaterFunction::isExistImpactOnRide(_18)) {
        if (isNerve(GET_NERVE(FloaterFloatingForceTypeNormal, HostTypeMoveFirst))) {
            f = 10.0f;

            if (getStep() > 5) {
                f *= -1.0f;
            }
        }
    }

    if (FloaterFunction::isExistSpeedUpLanding(_18)) {
        bool isValidAndOnSwitchA = MR::isValidSwitchA(mHost) && MR::isOnSwitchA(mHost);

        if (!isValidAndOnSwitchA) {
            bool b = isNerve(GET_NERVE(FloaterFloatingForceTypeNormal, HostTypeMoveOnPlayer)) && getStep() < 7;

            if (b) {
                f += (7 - getStep()) * 0.8f;
            }
        }
    }

    mVelocity.set(-_28 * f);

    f32 maxDepth = mMoveConditionType;
    _40 += f;
    _40 = MR::clamp(_40, 0.0f, maxDepth);
}

void FloaterFloatingForceTypeNormal::soundMoveSE() {
    if (_18 == nullptr) {
        return;
    }

    if (_44 > 0.1f) {
        const char* seMoveDown = FloaterFunction::getSeMoveDown(_18);

        if (seMoveDown != nullptr) {
            MR::startLevelSound(mHost, seMoveDown);
        }
    } else if (_44 < -0.1f) {
        const char* seMoveUp = FloaterFunction::getSeMoveUp(_18);

        if (seMoveUp != nullptr) {
            MR::startLevelSound(mHost, seMoveUp);
        }
    }
}

void FloaterFloatingForceTypeNormal::setStateMove() {
    if (MR::isOnPlayer(MR::getBodySensor(mHost))) {
        MapPartsFunction::setNerve(GET_NERVE(FloaterFloatingForceTypeNormal, HostTypeMoveOnPlayer));
    } else {
        MapPartsFunction::setNerve(GET_NERVE(FloaterFloatingForceTypeNormal, HostTypeMoveOffPlayer));
    }
}

void FloaterFloatingForceTypeNormal::exeWait() {
    if (isFirstStep()) {
        _40 = 0.0f;
    }
}

void FloaterFloatingForceTypeNormal::exeMoveFirst() {
    if (isFirstStep()) {
        const char* seGroundOn = FloaterFunction::getSeGroundOn(_18);

        if (seGroundOn != nullptr) {
            MR::startSound(mHost, seGroundOn);
        }

        if (FloaterFunction::isExistImpactOnRide(_18)) {
            MR::shakeCameraWeak();
        }
    }

    updateVelocity(MR::isOnPlayer(MR::getBodySensor(mHost)));
    soundMoveSE();

    if (isStep(8)) {
        setStateMove();
    }
}

void FloaterFloatingForceTypeNormal::exeMove() {
    bool isOnPlayer = MR::isOnPlayer(MR::getBodySensor(mHost));

    if (isOnPlayer && MR::isNearZero(_40) && !FloaterFunction::isExistSpeedUpLanding(_18)) {
        MapPartsFunction::setNerve(GET_NERVE(FloaterFloatingForceTypeNormal, HostTypeMoveFirst));
    }

    updateVelocity(isOnPlayer);
    soundMoveSE();

    if (!isOnPlayer && isNerve(GET_NERVE(FloaterFloatingForceTypeNormal, HostTypeMoveOnPlayer)) && MapPartsFunction::getStep() >= 7 ||
        isOnPlayer && MapPartsFunction::isNerve(GET_NERVE(FloaterFloatingForceTypeNormal, HostTypeMoveOffPlayer))) {
        setStateMove();
    }
}
