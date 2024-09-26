#include "Game/Map/LightDirector.hpp"
#include "Game/Map/LightFunction.hpp"
#include "Game/Util.hpp"

void LightDirector::init(const JMapInfoIter &rIter) {
    MR::connectToSceneMapObjMovement(this);
    LightFunction::loadAllLightWhite();
    mHolder = new LightDataHolder();
    mZoneHolder = new LightZoneDataHolder();
    mPointCtrl = new LightPointCtrl();
}

// LightDirector::loadLightPlayer

void LightDirector::loadLightCoin() const {
    LightFunction::loadLightInfoCoin(&mHolder->_8);
}

void LightDirector::movement() {
    mPointCtrl->update();
}

LightDirector::~LightDirector() {

}