#include "Game/MapObj/SnowplowSwitch.hpp"

SnowplowSwitch::SnowplowSwitch(const char *pName) : LiveActor(pName) {
    _8C = 0;
}

bool SnowplowSwitch::receiveOtherMsg(u32 msg, HitSensor *, HitSensor *) {
    if (msg == 73) {
        return _8C == 0;
    }

    if (msg == 74 && _8C) {
        _8C = 1;
        MR::onSwitchA(this);
        return true;
    }

    return false;
}

SnowplowSwitch::~SnowplowSwitch() {

}

void SnowplowSwitch::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);
    initHitSensor(1);
    TVec3f sensorOffs;
    sensorOffs.x = 0.0f;
    sensorOffs.y = 0.0f;
    sensorOffs.z = 0.0f;
    MR::addHitSensorEnemy(this, "Body", 8, 70.0f, sensorOffs);
    MR::needStageSwitchWriteA(this, rIter);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }
    else {
        makeActorAppeared();
    }
}