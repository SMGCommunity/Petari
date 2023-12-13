#include "Game/MapObj/FloaterFloatingForceTypeNormal.hpp"
#include "Game/MapObj/FloaterFunction.hpp"
#include "Game/LiveActor/Spine.hpp"
#include "Game/Util/MapPartsUtil.hpp"

FloaterFloatingForceTypeNormal::FloaterFloatingForceTypeNormal(LiveActor *pActor, const char *pName) : FloaterFloatingForce(pActor, pName) {
    mRotateAngle = 0.0f;
    mRotateSpeed = 0.0f;
    _40 = 0.0f;
    _44 = 0.0f;
}

void FloaterFloatingForceTypeNormal::init(const JMapInfoIter &rIter) {
    FloaterFloatingForce::init(rIter);
    MR::getMapPartsArgRotateSpeed(&mRotateSpeed, rIter);
    MR::getMapPartsArgRotateAngle(&mRotateAngle, rIter);
    MR::useStageSwitchReadA(mHost, rIter);
    MapPartsFunction::initNerve(&NrvFloaterFloatingForceTypeNormal::HostTypeWait::sInstance);
}

void FloaterFloatingForceTypeNormal::start() {
    FloaterFloatingForce::start();
    setStateMove();
}

// FloaterFloatingForceTypeNormal::updateHostTrans
// FloaterFloatingForceTypeNormal::updateVelocity

void FloaterFloatingForceTypeNormal::soundMoveSE() {
    if (_18) {
        if (mVelocity.y > 0.1f) {
            const char* soundDown = FloaterFunction::getSeMoveDown(_18);
            if (soundDown) {
                MR::startLevelSound(mHost, soundDown, -1, -1, -1);
            }

            return;
        }

        if (mVelocity.y < -0.1f) {
            const char* soundUp = FloaterFunction::getSeMoveUp(_18);
            if (soundUp) {
                MR::startLevelSound(mHost, soundUp, -1, -1, -1);
            }
        }
    }
}

void FloaterFloatingForceTypeNormal::setStateMove() {
    if (MR::isOnPlayer(MR::getBodySensor(mHost))) {
        MapPartsFunction::setNerve(&NrvFloaterFloatingForceTypeNormal::HostTypeMoveOnPlayer::sInstance);
    }
    else {
        MapPartsFunction::setNerve(&NrvFloaterFloatingForceTypeNormal::HostTypeMoveOffPlayer::sInstance);
    }
}

void FloaterFloatingForceTypeNormal::exeMoveFirst() {
    if (isFirstStep()) {
        const char* soundOn = FloaterFunction::getSeGroundOn(_18);
        if (soundOn) {
            MR::startSound(mHost, soundOn, -1, -1);
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

    if (isOnPlayer && MR::isNearZero(_44, 0.001f) && !FloaterFunction::isExistSpeedUpLanding(_18)) {
        MapPartsFunction::setNerve(&NrvFloaterFloatingForceTypeNormal::HostTypeMoveFirst::sInstance);
    }

    updateVelocity(isOnPlayer);
    soundMoveSE();

    if (!isOnPlayer && isNerve(&NrvFloaterFloatingForceTypeNormal::HostTypeMoveOnPlayer::sInstance) && MapPartsFunction::getStep() >= 7 || isOnPlayer && MapPartsFunction::isNerve(&NrvFloaterFloatingForceTypeNormal::HostTypeMoveOffPlayer::sInstance)) {
        setStateMove();
    }
}

FloaterFloatingForceTypeNormal::~FloaterFloatingForceTypeNormal() {

}

namespace NrvFloaterFloatingForceTypeNormal {
    INIT_NERVE(HostTypeWait);
    INIT_NERVE(HostTypeMoveFirst);
    INIT_NERVE(HostTypeMoveOffPlayer);
    INIT_NERVE(HostTypeMoveOnPlayer);

    void HostTypeMoveOnPlayer::execute(Spine *pSpine) const {
        FloaterFloatingForceTypeNormal* floater = reinterpret_cast<FloaterFloatingForceTypeNormal*>(pSpine->mExecutor);
        floater->exeMove();
    }

    void HostTypeMoveOffPlayer::execute(Spine *pSpine) const {
        FloaterFloatingForceTypeNormal* floater = reinterpret_cast<FloaterFloatingForceTypeNormal*>(pSpine->mExecutor);
        floater->exeMove();
    }

    void HostTypeMoveFirst::execute(Spine *pSpine) const {
        FloaterFloatingForceTypeNormal* floater = reinterpret_cast<FloaterFloatingForceTypeNormal*>(pSpine->mExecutor);
        floater->exeMoveFirst();
    }

    void HostTypeWait::execute(Spine *pSpine) const {
        FloaterFloatingForceTypeNormal* floater = reinterpret_cast<FloaterFloatingForceTypeNormal*>(pSpine->mExecutor);
        
        if (floater->isFirstStep()) {
            floater->_40 = 0.0f;
        }
    }
};

const TVec3f& FloaterFloatingForceTypeNormal::getCurrentVelocity() const {
    return mVelocity;
}