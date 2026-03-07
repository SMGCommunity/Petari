#include "Game/Map/OceanHomeMapCtrl.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Map/PlanetMap.hpp"
#include "Game/Map/WaterAreaHolder.hpp"
#include "Game/Map/WaterInfo.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/LiveActorUtil.hpp"

// getOceanHomeMapCtrl__30@unnamed@OceanHomeMapCtrl_cpp@Fv (from the debug symbol map)
// Not entirely sure if the createSceneObj would've been here or not. There's no evidence for or against it being here.
inline OceanHomeMapCtrl* getOceanHomeMapCtrl() {
    MR::createSceneObj(SceneObj_OceanHomeMapCtrl);
    return (OceanHomeMapCtrl*)MR::getSceneObjHolder()->getObj(SceneObj_OceanHomeMapCtrl);
}

OceanHomeMapCtrl::OceanHomeMapCtrl() : NameObj("オーシャンホーム地形制御") {
    mOceanHomePlanet = nullptr;
    mOceanRingPlanet = nullptr;
    _14 = 0;
    mOceanRingPlanetLowInWater = nullptr;
    _1C = 0;
    _20 = 0;
}

void OceanHomeMapCtrl::entryMapRing(PlanetMap* pPlanet) {
    mOceanRingPlanet = pPlanet;
    mOceanRingPlanetLowInWater = MR::createModelObjPlanetLow("オーシャンリング（水中用Low）", "OceanRingPlanetLowInWater", nullptr);
    MR::copyTransRotateScale(mOceanRingPlanet, mOceanRingPlanetLowInWater);
    MR::invalidateClipping(mOceanRingPlanetLowInWater);
    MR::tryStartAllAnim(mOceanRingPlanetLowInWater, "OceanRingPlanetLowInWater");
    mOceanRingPlanetLowInWater->kill();
}

void OceanHomeMapFunction::tryEntryOceanHomeMap(PlanetMap* pPlanet) {
    if (!strcmp(pPlanet->mName, "海洋ホーム惑星")) {
        getOceanHomeMapCtrl()->mOceanHomePlanet = pPlanet;
    } else if (!strcmp(pPlanet->mName, "オーシャンリング惑星")) {
        getOceanHomeMapCtrl()->entryMapRing(pPlanet);
    }
}

void OceanHomeMapCtrl::init(const JMapInfoIter&) {
    MR::connectToSceneMapObjMovement(this);
}

void OceanHomeMapCtrl::movement() {
    if (MR::isClipped(mOceanRingPlanet))
        return;

    if (WaterAreaFunction::getCameraWaterInfo()->mOceanBowl) {
        if (MR::isDead(mOceanRingPlanetLowInWater)) {
            mOceanRingPlanetLowInWater->makeActorAppeared();
        } else if (!MR::isHiddenModel(mOceanRingPlanet)){
            MR::hideModel(mOceanRingPlanet);
            mOceanRingPlanet->mLODCtrl->invalidate();
        }
    } else if (!mOceanRingPlanet->mLODCtrl->isShowLowModel() && MR::isHiddenModel(mOceanRingPlanet)) {
        MR::showModel(mOceanRingPlanet);
        mOceanRingPlanet->mLODCtrl->validate();
    } else {
        if (!MR::isDead(mOceanRingPlanetLowInWater)) {
            mOceanRingPlanetLowInWater->makeActorDead();
        }
    }
}

OceanHomeMapCtrl::~OceanHomeMapCtrl() {}