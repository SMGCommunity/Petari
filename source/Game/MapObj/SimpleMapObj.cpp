#include "Game/MapObj/SimpleMapObj.h"

void SimpleMapObj::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    MapObjActorUtil::setupInitInfoTypical(&info, mObjectName);
    MapObjActorUtil::setupInitInfoColorChangeArg0(&info, rIter);
    MapObjActorUtil::setupInitInfoTextureChangeArg1(&info, rIter);
    MapObjActorUtil::setupInitInfoShadowLengthArg2(&info, rIter);
    initialize(rIter, info);
}

void SimpleEnvironmentObj::connectToScene(const MapObjActorInitInfo &rInfo) {
    if (rInfo.mConnectToScene) {
        if (rInfo._5C == 1) {
            MR::connectToSceneEnvironmentStrongLight(this);
            return;
        }
        else if (rInfo._5C != 1) {
            MR::connectToSceneEnvironment(this);
            return;
        }
    }
}

void SimpleMapObjFarMax::init(const JMapInfoIter &rIter) {
    SimpleMapObj::init(rIter);
    MR::setClippingFarMax(this);
}

void SimpleMapObjNoSilhouetted::connectToScene(const MapObjActorInitInfo &rInfo) {
    if (rInfo.mConnectToScene) {
        if (rInfo._5C == 1) {
            MR::connectToSceneNoSilhouettedMapObjStrongLight(this);
            return;
        }
        else if (rInfo._5C != 1) {
            MR::connectToSceneNoSilhouettedMapObj(this);
            return;
        }
    }
}

// SimpleTextureSwitchChangeObj::initCaseUseSwitchA
// GlaringLight::connectToScene

SimpleMirrorObj::SimpleMirrorObj(const char *pName, const char *a2, MtxPtr mtx) : MapObjActor(pName) {
    mMtx = mtx;
    MR::createMirrorCamera();
}

void SimpleMirrorObj::calcAndSetBaseMtx() {
    if (!mMtx) {
        MapObjActor::calcAndSetBaseMtx();
    }
    else {
        TPos3f reflectionPos;
        reflectionPos.set(mMtx);
        MR::setMirrorReflectionInfoFromMtxYUp(reflectionPos);
        f32 z = reflectionPos.mMtx[2][3];
        f32 y = reflectionPos.mMtx[1][3];
        f32 x = reflectionPos.mMtx[0][3];
        mPosition.set(x, y, z);
        MR::setBaseTRMtx(this, mMtx);
    }
}

void SimpleMirrorReflectionObj::init(const JMapInfoIter &rIter) {
    SimpleMapObj::init(rIter);
    MR::invalidateClipping(this);
}

void SimpleMirrorReflectionObj::connectToScene(const MapObjActorInitInfo &rInfo) {
    MR::connectToSceneMirrorMapObjNoMovement(this);
}

// @unnamed@SimpleMapObj_cpp@::getSeParam

SimpleSeesawObj::SimpleSeesawObj(const char *pName) : MapObjActor(pName) {
    _C4.identity();
}

// SimpleSeesawObj::init

void SimpleSeesawObj::control() {
    MapObjActor::control();
}

void SimpleMapObjPush::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (MR::isSensorPlayer(a2)) {
        MR::sendMsgPush(a2, a1);
    }
}

void SimpleEnvironmentObj::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupHioNode("環境");
    initialize(rIter, info);
}

// SimpleTextureSwitchChangeObj::changeTexture

void SimpleMirrorObj::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    bool hasMtx = false;

    if (!mMtx) {
        MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    }
    else {
        info.setupHioNode("地形オブジェ");
        info.setupConnectToScene();
        info.setupEffect(NULL);
        info.setupSound(4);
        info.setupGroupClipping(0x80);
        info.setupDefaultPos();
        hasMtx = true;
    }

    info.setupMirrorReflection(hasMtx);
    initialize(rIter, info);
    MR::invalidateClipping(this);
}

SimpleEnvironmentObj::~SimpleEnvironmentObj() {

}

// SimpleMapObjNoSilhouetted::~SimpleMapObjNoSilhouetted
// SimpleTextureSwitchChangeObj::~SimpleTextureSwitchChangeObj
// GlaringLight::~GlaringLight

SimpleMirrorObj::~SimpleMirrorObj() {

}

SimpleMirrorReflectionObj::~SimpleMirrorReflectionObj() {

}

SimpleSeesawObj::~SimpleSeesawObj() {

}

SimpleMapObjPush::~SimpleMapObjPush() {

}

// UFOKinokoUnderConstruction::~UFOKinokoUnderConstruction