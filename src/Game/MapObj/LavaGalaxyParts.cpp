#include "Game/MapObj/LavaGalaxyParts.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"

void FORCE_SUB_NO_INLINE() {
    TVec3f a, b;
    a = a - b;
}

LavaGalaxyParts::~LavaGalaxyParts() {
}

LavaGalaxyParts::LavaGalaxyParts(const char* pName) : LiveActor(pName), mLodCtrl(), mMtxSetter() {
}

void LavaGalaxyParts::initAfterPlacement() {
    if (mMtxSetter != nullptr) {

        TVec3f groundVec;
        MR::calcMapGroundUpper(&groundVec, this);

        f32 yOffset = mPosition.distance(groundVec);
        if ((mPosition - groundVec).dot(mGravity) < 0.0f) {
            yOffset *= -1.0f;
        }

        TVec3f uselessVec(0.0f, yOffset, 0.0f);
        mMtxSetter->updateMtxUseBaseMtxWithLocalOffset(TVec3f(0.0f, yOffset, 0.0f));
        MR::offCalcGravity(this);
    }
}

void LavaGalaxyParts::makeActorAppeared() {
    LiveActor::makeActorAppeared();

    if (mLodCtrl != nullptr) {
        mLodCtrl->validate();
    }
}

void LavaGalaxyParts::makeActorDead() {
    if (mLodCtrl != nullptr) {
        mLodCtrl->invalidate();
    }

    LiveActor::makeActorDead();
}

void LavaGalaxyParts::control() {
    if (mLodCtrl != nullptr) {
        mLodCtrl->update();
    }
}

void LavaGalaxyParts::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    const char* objectName;
    MR::getObjectName(&objectName, rIter);

    // FIXME: this is an inline
    const char* pName = objectName;
    bool isLavaStep;
    if (MR::isEqualString("LavaStepA", pName)) {
        isLavaStep = true;
    } else if (MR::isEqualString("LavaStepB", pName)) {
        isLavaStep = true;
    } else if (MR::isEqualString("LavaStepC", pName)) {
        isLavaStep = true;
    } else {
        isLavaStep = false;
    }

    if (isLavaStep) {
        initModelManagerWithAnm(objectName, nullptr, true);

        mMtxSetter = MR::initDLMakerProjmapEffectMtxSetter(this);
        MR::newDifferedDLBuffer(this);
    } else {
        initModelManagerWithAnm(objectName, nullptr, false);
    }

    initHitSensor(1);
    MR::initCollisionParts(this, objectName, MR::addBodyMessageSensorMapObj(this), nullptr);

    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);

    f32 arg0 = -1.0f;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);

    // FIXME: this is an inline
    pName = objectName;
    bool isNonFloatingOrLavaStep;
    if (MR::isEqualString(pName, "LavaNonFloatingStepA")) {
        isNonFloatingOrLavaStep = true;
    } else if (MR::isEqualString(pName, "LavaStepA")) {
        isNonFloatingOrLavaStep = true;
    } else if (MR::isEqualString(pName, "LavaStepB")) {
        isNonFloatingOrLavaStep = true;
    } else if (MR::isEqualString(pName, "LavaStepC")) {
        isNonFloatingOrLavaStep = true;
    } else {
        isNonFloatingOrLavaStep = false;
    }

    if (isNonFloatingOrLavaStep) {
        mLodCtrl = MR::createLodCtrlPlanet(this, rIter, -1.0f, -1);
    }

    MR::connectToSceneMapObj(this);

    if (MR::isExistBtk(this, objectName)) {
        MR::startBtk(this, objectName);
    }

    if (MR::isExistBrk(this, objectName)) {
        MR::startBrk(this, objectName);
    }

    if (MR::isExistBpk(this, objectName)) {
        MR::startBpk(this, objectName);
    }

    if (MR::isExistBtp(this, objectName)) {
        MR::startBtp(this, objectName);
    }

    MR::onCalcGravity(this);

    makeActorAppeared();
}
