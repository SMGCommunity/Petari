#include "Game/Boss/TombSpiderEnvironment.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Map/PlanetMap.hpp"
#include "Game/MapObj/SpiderThread.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

TombSpiderEnvironment::TombSpiderEnvironment(LiveActor* pActor) : mActor(pActor), mPlanet(nullptr), mCocoon(nullptr) {
    MR::setEffectHostSRT(mActor, "Noctiluca", MR::getPlayerPos(), nullptr, nullptr);
    MR::createSceneObj(SceneObj_SpiderThread);
    MR::initSpiderThread(mActor->mPosition);

    mPlanet = new PlanetMap("戦場[トゥームスパイダー]", "TombSpiderPlanet");
    mPlanet->mPosition.set(mActor->mPosition);
    mPlanet->initWithoutIter();
    mPlanet->mLODCtrl->setDistanceToLow(25000.0f);
    MR::registerDemoSimpleCastAll(mPlanet);
    mPlanet->appear();

    mCocoon = new ModelObj("まゆ[トゥームスパイダー]", "TombSpiderCocoon", nullptr, -2, -2, -2, false);
    mCocoon->mPosition.set(mActor->mPosition);
    mCocoon->initWithoutIter();
    MR::invalidateClipping(mCocoon);
    MR::registerDemoSimpleCastAll(mCocoon);
    mCocoon->kill();
}
