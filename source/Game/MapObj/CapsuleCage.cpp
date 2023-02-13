#include "Game/MapObj/CapsuleCage.h"

CapsuleCage::CapsuleCage(const char *pName) : MapObjActor(pName) {
    mInfo = nullptr;
}

void CapsuleCage::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupProjmapMtx(false);
    info.setupNerve(&NrvCapsuleCage::CapsuleCageNrvWait::sInstance);
    initialize(rIter, info);
    MR::setBodySensorType(this, 88);

    if (!MR::initActorCamera(this, rIter, &mInfo)) {
        mInfo = nullptr;
    }
}

void CapsuleCage::exeStartCamera() {
    if (MR::isFirstStep(this)) {
        MR::startActorCameraTargetSelf(this, mInfo, -1);
    }

    if (MR::isStep(this, 50)) {
        setNerve(&NrvCapsuleCage::CapsuleCageNrvOpen::sInstance);
    }
}

void CapsuleCage::exeOpen() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "Move");
        MR::startSound(this, "SE_OJ_CAPSULE_CAGE_BREAK", -1, -1);
        MR::tryRumblePadWeak(this, 0);
        MR::shakeCameraWeak();
    }

    if (MR::isBckStopped(this)) {
        MR::tryRumblePadWeak(this, 0);
        MR::shakeCameraWeak();

        if (mInfo != nullptr) {
            setNerve(&NrvCapsuleCage::CapsuleCageNrvEndCamera::sInstance);
        }
        else {
            kill();
        }
    }
}

void CapsuleCage::kill() {
    if (mInfo != nullptr) {
        MR::endActorCamera(this, mInfo, false, -1);
        MR::endDemo(this, cDemoCameraName);
    }

    MapObjActor::kill();
}

void CapsuleCage::connectToScene(const MapObjActorInitInfo &rInfo) {
    MR::connectToSceneIndirectMapObj(this);
}

void CapsuleCage::initCaseUseSwitchB(const MapObjActorInitInfo &rInfo) {
    MR::FunctorV0M<CapsuleCage *, void (CapsuleCage::*)()> openFunc = MR::Functor_Inline<CapsuleCage>(this, &CapsuleCage::startOpen);
    MR::listenStageSwitchOnB(this, openFunc);
}

void CapsuleCage::startOpen() {
    MR::invalidateClipping(this);

    if (mInfo != nullptr) {
        MR::requestStartDemoWithoutCinemaFrame(this, cDemoCameraName, &NrvCapsuleCage::CapsuleCageNrvStartCamera::sInstance, nullptr);
    }
    else {
        setNerve(&NrvCapsuleCage::CapsuleCageNrvOpen::sInstance);
    }
}

void CapsuleCage::exeEndCamera() {
    if (MR::isStep(this, 0x3C)) {
        kill();
    }
}

namespace NrvCapsuleCage {
    INIT_NERVE(CapsuleCageNrvWait);
    INIT_NERVE(CapsuleCageNrvStartCamera);
    INIT_NERVE(CapsuleCageNrvOpen);
    INIT_NERVE(CapsuleCageNrvEndCamera);
};

void CapsuleCage::initCaseNoUseSwitchB(const MapObjActorInitInfo &) {

}

void CapsuleCage::initCaseNoUseSwitchA(const MapObjActorInitInfo &) {

}

void CapsuleCage::initCaseUseSwitchA(const MapObjActorInitInfo &) {

}

CapsuleCage::~CapsuleCage() {

}
