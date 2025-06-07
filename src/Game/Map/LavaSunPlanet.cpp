#include "Game/Map/LavaSunPlanet.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/MapObj/MapPartsRotator.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "revolution/types.h"

LavaSunPlanet::LavaSunPlanet(const char *pName) : LiveActor(pName) {
    mRotator = nullptr;
    mObjName = nullptr;
    mLodCtrl = nullptr;
}

void LavaSunPlanet::control() {
    if (mRotator != nullptr) {
        mRotator->movement();
    }

    mLodCtrl->update();
}

void LavaSunPlanet::calcAndSetBaseMtx() {
    if (mRotator == nullptr)  {
        LiveActor::calcAndSetBaseMtx();
    }
    else {
        TPos3f mtx;
        mtx.setInline(mRotator->getRotateMtx());
        mtx.mMtx[0][3] = mPosition.x;
        mtx.mMtx[1][3] = mPosition.y;
        mtx.mMtx[2][3] = mPosition.z;
        MR::setBaseTRMtx(this, mtx);
    }
}

void LavaSunPlanet::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getObjectName(&mObjName, rIter);
    initModelManagerWithAnm(mObjName, nullptr, true);
    ProjmapEffectMtxSetter* planetEffect = MR::initDLMakerProjmapEffectMtxSetter(this);
    MR::newDifferedDLBuffer(this);
    MR::connectToScenePlanet(this);
    initHitSensor(1);
    MR::initCollisionParts(this, mObjName, MR::addBodyMessageSensorMapObj(this), nullptr);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    MR::setClippingFarMax(this);
    mLodCtrl = MR::createLodCtrlPlanet(this, rIter, -1.0f, -1);
    ProjmapEffectMtxSetter* lodEffect = MR::initDLMakerProjmapEffectMtxSetter(mLodCtrl->_10);

    if (MR::isExistAnim(this, mObjName)) {
        MR::startAllAnim(this, mObjName);
    }
    else if (!MR::isExistEffectTexMtx(this)) {
        calcAnim();
        MR::offCalcAnim(this);
    }

    if (MR::isEqualString(mObjName, "LavaRotatePlanet")) {
        mRotator = new MapPartsRotator(this);
        mRotator->init(rIter);
        mRotator->start();
        mLodCtrl->syncJointAnimation();
    }

    makeActorAppeared();
    planetEffect->updateMtxUseBaseMtx();
    lodEffect->updateMtxUseBaseMtx();
}

LavaSunPlanet::~LavaSunPlanet() {
    
}
