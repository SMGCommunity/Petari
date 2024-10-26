#include "Game/MapObj/SimpleMapObj.hpp"

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

void SimpleTextureSwitchChangeObj::initCaseUseSwitchA(const MapObjActorInitInfo &rInfo) {
    MR::FunctorV0M<SimpleTextureSwitchChangeObj *, void (SimpleTextureSwitchChangeObj::*)()> func = MR::Functor<SimpleTextureSwitchChangeObj>(this, &SimpleTextureSwitchChangeObj::changeTexture);
    MR::listenStageSwitchOnA(this, func);
}

void GlaringLight::connectToScene(const MapObjActorInitInfo &rInfo) {
    if (rInfo.mConnectToScene) {
        MR::connectToScene(this, 0x22, 5, 0x22, -1);
    }
}

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

namespace {
    const SoundEffectDataEntry* getSeParam(const char *pName) {
        for (u32 i = 0; i < 8; i++) {
            if (MR::isEqualString(pName, sSeDataTable[i].objectName)) {
                return &sSeDataTable[i];
            }
        }

        return nullptr;
    }
};

SimpleSeesawObj::SimpleSeesawObj(const char *pName) : MapObjActor(pName) {
    _C4.identity();
}

void SimpleSeesawObj::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    const SoundEffectDataEntry* entry = getSeParam(mObjectName);
    const char* sound_name = entry != nullptr ? entry->soundName : nullptr;
    entry = getSeParam(mObjectName);
    MapObjActorUtil::setupInitInfoSeesaw(&info, rIter, sound_name, entry != nullptr ? entry->_8 : 0.0f);
    initialize(rIter, info);
}

void SimpleSeesawObj::control() {
    MapObjActor::control();
}

void SimpleMapObjPush::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (MR::isSensorPlayer(a2)) {
        MR::sendMsgPush(a2, a1);
    }
}

void UFOKinokoUnderConstruction::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupFarClipping(-1.0f);
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    MapObjActorUtil::setupInitInfoColorChangeArg0(&info, rIter);
    MapObjActorUtil::setupInitInfoTextureChangeArg1(&info, rIter);
    MapObjActorUtil::setupInitInfoShadowLengthArg2(&info, rIter);

    if (MR::isEqualString("UFOKinokoUnderConstruction", mObjectName)) {

        if (MR::isUFOKinokoUnderConstruction()) {
            info.mModelName = "UFOKinokoUnderConstruction";
        }
        else {
            info.mModelName = "UFOKinokoLandingAstro";
        }
    }

    initialize(rIter, info);
    if (MR::isUFOKinokoBeforeConstruction()) {
        makeActorDead();
    }
}

namespace MR {
    void makeArchiveListUFOKinokoUnderConstruction(NameObjArchiveListCollector *pArchiveList, const JMapInfoIter &rIter) {
        const char* archive;

        if (MR::isUFOKinokoBeforeConstruction()) {
            archive = "UFOKinokoUnderConstruction";
        }
        else {
            archive = "UFOKinokoLandingAstro";
        }

        pArchiveList->addArchive(archive);
    }
};

void SimpleEnvironmentObj::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupHioNode("環境");
    initialize(rIter, info);
}

void SimpleTextureSwitchChangeObj::changeTexture() {
    MR::startBtk(this, "TexChange");
    MR::setBtkFrameAndStop(this, getChangedTextureNo());
}

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
        info.setupEffect(nullptr);
        info.setupSound(4);
        info.setupGroupClipping(0x80);
        info.setupDefaultPos();
        hasMtx = true;
    }

    info.setupMirrorReflection(hasMtx);
    initialize(rIter, info);
    MR::invalidateClipping(this);
}

s32 SimpleTextureSwitchChangeObj::getChangedTextureNo() const {
    return 1;
}

SimpleEnvironmentObj::~SimpleEnvironmentObj() {

}

SimpleMapObjNoSilhouetted::~SimpleMapObjNoSilhouetted() {

}

SimpleTextureSwitchChangeObj::~SimpleTextureSwitchChangeObj() {

}

GlaringLight::~GlaringLight() {

}

SimpleMirrorObj::~SimpleMirrorObj() {

}

SimpleMirrorReflectionObj::~SimpleMirrorReflectionObj() {

}

SimpleSeesawObj::~SimpleSeesawObj() {

}

SimpleMapObjPush::~SimpleMapObjPush() {

}

UFOKinokoUnderConstruction::~UFOKinokoUnderConstruction() {

}
