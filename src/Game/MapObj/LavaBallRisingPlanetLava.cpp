#include "Game/MapObj/LavaBallRisingPlanetLava.hpp"

LavaBallRisingPlanetLava::LavaBallRisingPlanetLava(const char *pName) : MapObjActor(pName) {

}

void LavaBallRisingPlanetLava::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(nullptr);
    info.setupSound(4);
    info.setupNerve(&NrvLavaBallRisingPlanetLava::LavaBallRisingPlanetLavaNrvWaitSmall::sInstance);
    initialize(rIter, info);
}

void LavaBallRisingPlanetLava::exeWait() {
    MR::startLevelSound(this, "SE_AT_LV_MAGMA_WIND_RISING", -1, -1, -1);

    if (!MR::isLessStep(this, 120)) {
        if (isNerve(&NrvLavaBallRisingPlanetLava::LavaBallRisingPlanetLavaNrvWaitSmall::sInstance)) {
            setNerve(&NrvLavaBallRisingPlanetLava::LavaBallRisingPlanetLavaNrvScaleUp::sInstance);
        }
        else {
            setNerve(&NrvLavaBallRisingPlanetLava::LavaBallRisingPlanetLavaNrvScaleDown::sInstance);
        }
    }
}

void LavaBallRisingPlanetLava::exeScaleUp() {
    MR::startLevelSound(this, "SE_AT_LV_MAGMA_WIND_RISING", -1, -1, -1);
    MR::startLevelSound(this, "SE_OJ_LV_LAVA_MOVE_RISING", -1, -1, -1);
    mScale.setAll<f32>(MR::calcNerveValue(this, 240, 1.0f, 1.1f));

    if (MR::isStep(this, 240)) {
        setNerve(&NrvLavaBallRisingPlanetLava::LavaBallRisingPlanetLavaNrvWaitBig::sInstance);
    }
}

void LavaBallRisingPlanetLava::exeScaleDown() {
    MR::startLevelSound(this, "SE_AT_LV_MAGMA_WIND_RISING", -1, -1, -1);
    MR::startLevelSound(this, "SE_OJ_LV_LAVA_MOVE_RISING", -1, -1, -1);
    mScale.setAll<f32>(MR::calcNerveValue(this, 240, 1.1f, 1.0f));

    if (MR::isStep(this, 240)) {
        setNerve(&NrvLavaBallRisingPlanetLava::LavaBallRisingPlanetLavaNrvWaitSmall::sInstance);
    }
}

LavaBallRisingPlanetLava::~LavaBallRisingPlanetLava() {

}

namespace NrvLavaBallRisingPlanetLava {
    INIT_NERVE(LavaBallRisingPlanetLavaNrvWaitSmall);
    INIT_NERVE(LavaBallRisingPlanetLavaNrvWaitBig);
    INIT_NERVE(LavaBallRisingPlanetLavaNrvScaleUp);
    INIT_NERVE(LavaBallRisingPlanetLavaNrvScaleDown);
};