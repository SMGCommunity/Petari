#include "Game/MapObj/ClipFieldSwitch.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/ClipAreaHolder.hpp"
#include "Game/Util.hpp"

ClipFieldSwitch::~ClipFieldSwitch() {
}

ClipFieldSwitch::ClipFieldSwitch(const char* pName) : LiveActor(pName) {
}

void ClipFieldSwitch::control() {
    mPosition = *MR::getPlayerPos();
    if (MR::isInClipArea(*MR::getPlayerPos(), 5.0f)) {
        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }

        if (MR::isValidSwitchB(this)) {
            MR::offSwitchB(this);
        }
    } else {
        if (MR::isValidSwitchA(this)) {
            MR::offSwitchA(this);
        }

        if (MR::isValidSwitchB(this)) {
            MR::onSwitchB(this);
        }
    }
}

void ClipFieldSwitch::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    MR::useStageSwitchWriteA(this, rIter);
    MR::useStageSwitchWriteB(this, rIter);
    initHitSensor(1);
    MR::addHitSensorEye(this, "body", 8, 1000.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::invalidateClipping(this);
    appear();
}
