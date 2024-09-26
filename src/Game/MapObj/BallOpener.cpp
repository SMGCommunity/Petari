#include "Game/MapObj/BallOpener.hpp"
#include "Game/Util/GeometryBindUtil.hpp"

BallOpener::~BallOpener() {
    
}

BallOpener::BallOpener(const char *pName) : LiveActor(pName) {
    mSensor = nullptr;
    _A0.set(0, 0, 0);
    _AC.set(0, 0, 0);
    _B8.set(0, 0, 0);
    _C4 = 0;
    _8C = new BindCone(mPosition, TVec3f(0.0f, 1.0f, 0.0f), 400.0f, 200.0f);
}

void BallOpener::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("BallOpener", nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addHitSensor(this, "body", 121, 8, 200.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initEffectKeeper(1, nullptr, false);
    initSound(4, false);
    MR::initCollisionParts(this, "BallOpener", getSensor(nullptr), nullptr);
    _8C->setPosition(mPosition);
    initNerve(&NrvBallOpener::BallOpenerNrvWait::sInstance);
    MR::useStageSwitchWriteA(this, rIter);
    appear();
}

void BallOpener::initAfterPlacement() {
    TVec3f grav;
    MR::calcGravityVector(this, &_94, nullptr, 0);
    _8C->setDirection(_94);
}

void BallOpener::control() {

}

// BallOpener::receiveOtherMsg

void BallOpener::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
    }

    if (mSensor != nullptr) {
        bindHole();
        setNerve(&NrvBallOpener::BallOpenerNrvSetCenter::sInstance);
    }
}

void BallOpener::exeSetCenter() {
    if (MR::isFirstStep(this)) {
        _C4 = 0;
        MR::invalidateClipping(this);
    }

    bindHole();

    if (_B8.squared() < 0.000099999997f) {
        _C4++;
    }
    else { 
        _C4 = 0; 
    }

    if (_C4 == 1) {
        MR::shakeCameraWeak();
    }

    MR::startLevelSound(this, "SE_OJ_LV_BALL_OPN_SETTING", -1, -1, -1);
    if (_C4 > 20 || MR::isGreaterStep(this, 180)) {
        setNerve(&NrvBallOpener::BallOpenerNrvOpen::sInstance);
    }
}

void BallOpener::exeOpen() {
    if (MR::isFirstStep(this)) {
        mSensor->receiveMessage(0xAE, getSensor("body"));
    }

    if (MR::isStep(this, 45)) {
        MR::zeroVelocity(mSensor->mActor);
        mSensor->receiveMessage(0xB3, getSensor("body"));
        mSensor = nullptr;
        MR::startSound(this, "SE_OJ_BALL_OPN_OPEN", -1, -1);
        MR::startSystemSE("SE_SY_TAMAKORO_GOAL", -1, -1);

        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }

        setNerve(&NrvBallOpener::BallOpenerNrvWait::sInstance);
    }
}

// BallOpener::bindHole

namespace NrvBallOpener {
    INIT_NERVE(BallOpenerNrvWait);
    INIT_NERVE(BallOpenerNrvSetCenter);
    INIT_NERVE(BallOpenerNrvOpen);
};