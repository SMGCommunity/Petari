#include "Game/MapObj/LavaBallRisingPlanetLava.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvLavaBallRisingPlanetLava {
    NEW_NERVE(LavaBallRisingPlanetLavaNrvWaitSmall, LavaBallRisingPlanetLava, Wait);
    NEW_NERVE(LavaBallRisingPlanetLavaNrvWaitBig, LavaBallRisingPlanetLava, Wait);
    NEW_NERVE(LavaBallRisingPlanetLavaNrvScaleUp, LavaBallRisingPlanetLava, ScaleUp);
    NEW_NERVE(LavaBallRisingPlanetLavaNrvScaleDown, LavaBallRisingPlanetLava, ScaleDown);
};  // namespace NrvLavaBallRisingPlanetLava

LavaBallRisingPlanetLava::LavaBallRisingPlanetLava(const char* pName) : MapObjActor(pName) {
}

void LavaBallRisingPlanetLava::init(const JMapInfoIter& rIter) {
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
    MR::startLevelSound(this, "SE_AT_LV_MAGMA_WIND_RISING");

    if (!MR::isLessStep(this, 120)) {
        if (isNerve(&NrvLavaBallRisingPlanetLava::LavaBallRisingPlanetLavaNrvWaitSmall::sInstance)) {
            setNerve(&NrvLavaBallRisingPlanetLava::LavaBallRisingPlanetLavaNrvScaleUp::sInstance);
        } else {
            setNerve(&NrvLavaBallRisingPlanetLava::LavaBallRisingPlanetLavaNrvScaleDown::sInstance);
        }
    }
}

void LavaBallRisingPlanetLava::exeScaleUp() {
    MR::startLevelSound(this, "SE_AT_LV_MAGMA_WIND_RISING");
    MR::startLevelSound(this, "SE_OJ_LV_LAVA_MOVE_RISING");
    mScale.setAll< f32 >(MR::calcNerveValue(this, 240, 1.0f, 1.1f));

    if (MR::isStep(this, 240)) {
        setNerve(&NrvLavaBallRisingPlanetLava::LavaBallRisingPlanetLavaNrvWaitBig::sInstance);
    }
}

void LavaBallRisingPlanetLava::exeScaleDown() {
    MR::startLevelSound(this, "SE_AT_LV_MAGMA_WIND_RISING");
    MR::startLevelSound(this, "SE_OJ_LV_LAVA_MOVE_RISING");
    mScale.setAll< f32 >(MR::calcNerveValue(this, 240, 1.1f, 1.0f));

    if (MR::isStep(this, 240)) {
        setNerve(&NrvLavaBallRisingPlanetLava::LavaBallRisingPlanetLavaNrvWaitSmall::sInstance);
    }
}

LavaBallRisingPlanetLava::~LavaBallRisingPlanetLava() {
}
