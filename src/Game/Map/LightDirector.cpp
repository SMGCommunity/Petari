#include "Game/Map/LightDirector.hpp"
#include "Game/LiveActor/ActorLightCtrl.hpp"
#include "Game/Map/LightDataHolder.hpp"
#include "Game/Map/LightFunction.hpp"
#include "Game/Map/LightPointCtrl.hpp"
#include "Game/Map/LightZoneDataHolder.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util/ObjUtil.hpp"

LightDirector::LightDirector() : NameObj("ライト指揮"), _C(), mDataHolder(), mZoneDataHolder(), mDefaultAreaLight(), _1C(), mPointCtrl(), mResourceHolder() {
}

void LightDirector::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    LightFunction::loadAllLightWhite();

    mDataHolder = new LightDataHolder();
    mZoneDataHolder = new LightZoneDataHolder();
    mPointCtrl = new LightPointCtrl();
}

void LightDirector::initData() {
    mResourceHolder = LightFunction::loadLightArchive();
    mDataHolder->initLightData();
    mZoneDataHolder->initZoneData();
    mDefaultAreaLight = mDataHolder->findAreaLight(mZoneDataHolder->getDefaultStageAreaLightName());
}

void LightDirector::loadLightPlayer() const {
    _1C->loadLight();
    mPointCtrl->loadPointLight();
}

void LightDirector::loadLightCoin() const {
    LightFunction::loadLightInfoCoin(&mDataHolder->_8);
}

void LightDirector::movement() {
    mPointCtrl->update();
}
