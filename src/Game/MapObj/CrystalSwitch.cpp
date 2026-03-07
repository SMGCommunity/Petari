#include "Game/MapObj/CrystalSwitch.hpp"
#include "Game/LiveActor/HitSensor.hpp"

/* Note -- this file was compiled super early on in the game's life, so it was compiled under -O2 where the release build is -O4,p. Keep this in mind!
 */

namespace NrvCrystalSwitch {
    NEW_NERVE(CrystalSwitchNrvOff, CrystalSwitch, Off);
    NEW_NERVE(CrystalSwitchNrvSwitchDown, CrystalSwitch, SwitchDown);
    NEW_NERVE(CrystalSwitchNrvOn, CrystalSwitch, On);
    NEW_NERVE(CrystalSwitchNrvSwitchUp, CrystalSwitch, SwitchUp);
};  // namespace NrvCrystalSwitch

CrystalSwitch::CrystalSwitch(const char* pName) : LiveActor(pName) {
    _8C = 0;
    _90 = 300;
    mRotateSpeed = 0.0f;
    _98 = 0;
}

void CrystalSwitch::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("CrystalSwitch", nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    TVec3f sensor_offs;
    sensor_offs.x = 0.0f;
    sensor_offs.y = 100.0f;
    sensor_offs.z = 0.0f;
    MR::addHitSensorMapObj(this, "body", 16, 100.0f, sensor_offs);
    initSound(4, false);
    MR::needStageSwitchWriteA(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &_90);
    initNerve(&NrvCrystalSwitch::CrystalSwitchNrvOff::sInstance);
    initEffectKeeper(0, nullptr, false);
    MR::startBck(this, "Wait", nullptr);
    MR::startBpk(this, "Off");
    appear();
}

void CrystalSwitch::control() {
    calcRotSpeed();
    _98 = 0;
}

void CrystalSwitch::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        pReceiver->receiveMessage(41, pSender);
    }
}

bool CrystalSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    _98 = 1;
    return true;
}

bool CrystalSwitch::trySwitchDown() {
    if (_98) {
        setNerve(&NrvCrystalSwitch::CrystalSwitchNrvSwitchDown::sInstance);
        return true;
    }

    return false;
}

bool CrystalSwitch::tryOn() {
    if (getNerveStep() > 10) {
        setNerve(&NrvCrystalSwitch::CrystalSwitchNrvOn::sInstance);
        return true;
    }

    return false;
}

bool CrystalSwitch::tryOff() {
    if (mRotateSpeed < 8.0f) {
        setNerve(&NrvCrystalSwitch::CrystalSwitchNrvOff::sInstance);
        return true;
    }

    return false;
}

void CrystalSwitch::exeOff() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        MR::offSwitchA(this);
    }

    trySwitchDown();
}

void CrystalSwitch::exeSwitchDown() {
    if (MR::isFirstStep(this)) {
        MR::startBpk(this, "On");
        MR::invalidateClipping(this);
    }

    tryOn();
}

void CrystalSwitch::exeOn() {
    if (MR::isFirstStep(this)) {
        MR::onSwitchA(this);
        MR::shakeCameraNormal();
        MR::startSound(this, "SE_OJ_CRYSTAL_SWITCH_ON", -1, -1);
        mRotateSpeed = 10.0f;
        MR::setBckRate(this, 10.0f);
    }
}

void CrystalSwitch::exeSwitchUp() {
    if (MR::isFirstStep(this)) {
        MR::startBpk(this, "Off");
    }

    tryOff();
}

void CrystalSwitch::calcRotSpeed() {
    if (isNerve(&NrvCrystalSwitch::CrystalSwitchNrvOn::sInstance)) {
        if (mRotateSpeed < 10.0f) {
            mRotateSpeed += 1.5f;
        }

        if (mRotateSpeed > 10.0f) {
            mRotateSpeed = 10.0f;
        }
    } else {
        if (mRotateSpeed > 10.0f) {
            mRotateSpeed = 10.0f;
        }

        MR::setBckRate(this, mRotateSpeed);
        mRotateSpeed *= 0.99f;

        if (mRotateSpeed < 0.0f) {
            mRotateSpeed = 0.0f;
        }
    }
}

CrystalSwitch::~CrystalSwitch() {}
