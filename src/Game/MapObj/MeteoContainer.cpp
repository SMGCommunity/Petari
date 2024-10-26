#include "Game/MapObj/MeteoContainer.hpp"

MeteoContainer::MeteoContainer(const char *pName) : LiveActor(pName) {

}

void MeteoContainer::init(const JMapInfoIter &rIter) {
    const char* objName = nullptr;

    if (!MR::getObjectName(&objName, rIter)) {
        objName = "MeteoContainer";
    }

    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm(objName, nullptr, false);
    MR::connectToSceneEnemy(this);
    initHitSensor(1);
    TVec3f sensorOffs;
    sensorOffs.x = 0.0f;
    sensorOffs.y = 150.0f;
    sensorOffs.z = 0.0f;
    MR::addHitSensorMapObj(this, "body", 0x10, 150.0f, sensorOffs);
    initSound(4, false);
    initNerve(&NrvMeteoContainer::MeteoContainerNrvWait::sInstance);
    MR::initShadowVolumeSphere(this, 150.0f);
    MR::onCalcShadowOneTime(this, nullptr);
    MR::needStageSwitchWriteDead(this, rIter);
    initEffectKeeper(1, nullptr, false);
    appear();
}

void MeteoContainer::appear() {
    LiveActor::appear();
    setNerve(&NrvMeteoContainer::MeteoContainerNrvWait::sInstance);
}

void MeteoContainer::kill() {
    MR::startSound(this, "SE_OJ_MTO_CONTAINER_BREAK", -1, -1);
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
        MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
    }

    LiveActor::kill();
}

void MeteoContainer::control() {

}

void MeteoContainer::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (MR::isSensorPlayer(a2)) {
        a2->receiveMessage(41, a1);
    }
}

bool MeteoContainer::receiveMsgPush(HitSensor *a1, HitSensor *a2) {
    return MR::isSensorPlayer(a1);
}

bool MeteoContainer::receiveOtherMsg(u32 msg, HitSensor *, HitSensor *) {
    if (msg == 49 && isNerve(&NrvMeteoContainer::MeteoContainerNrvWait::sInstance)) {
        setNerve(&NrvMeteoContainer::MeteoContainerNrvDestroy::sInstance);
        return true;
    }

    return false;
}

void MeteoContainer::exeDestroy() {
    if (MR::isStep(this, 2)) {
        MR::stopScene(10);
    }

    if (MR::isStep(this, 3)) {
        MR::shakeCameraNormal();
        kill();
    }
}


MeteoContainer::~MeteoContainer() {

}

namespace NrvMeteoContainer {
    INIT_NERVE(MeteoContainerNrvWait);
    INIT_NERVE(MeteoContainerNrvDestroy);
};
