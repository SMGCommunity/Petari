#include "Game/MapObj/CapsuleCage.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util.hpp"

namespace {
    const char* cDemoCameraName = "注目カメラ";
    static const s32 sStepForStartCamera = 50;
    static const s32 sStepForEndCamera = 60;
};  // namespace

namespace NrvCapsuleCage {
    NEW_NERVE(CapsuleCageNrvWait, CapsuleCage, Wait);
    NEW_NERVE(CapsuleCageNrvStartCamera, CapsuleCage, StartCamera);
    NEW_NERVE(CapsuleCageNrvOpen, CapsuleCage, Open);
    NEW_NERVE(CapsuleCageNrvEndCamera, CapsuleCage, EndCamera);
};  // namespace NrvCapsuleCage

CapsuleCage::CapsuleCage(const char* pName) : MapObjActor(pName), mInfo() {
}

void CapsuleCage::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupProjmapMtx(false);
    info.setupNerve(&NrvCapsuleCage::CapsuleCageNrvWait::sInstance);
    initialize(rIter, info);
    MR::setBodySensorType(this, ATYPE_KEY_SWITCH_AVOID);

    if (!MR::initActorCamera(this, rIter, &mInfo)) {
        mInfo = nullptr;
    }
}

void CapsuleCage::exeWait() {
}

void CapsuleCage::exeStartCamera() {
    if (MR::isFirstStep(this)) {
        MR::startActorCameraTargetSelf(this, mInfo, -1);
    }

    if (MR::isStep(this, ::sStepForStartCamera)) {
        setNerve(&NrvCapsuleCage::CapsuleCageNrvOpen::sInstance);
    }
}

void CapsuleCage::exeOpen() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "Move");
        MR::startSound(this, "SE_OJ_CAPSULE_CAGE_BREAK");
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        MR::shakeCameraWeak();
    }

    if (MR::isBckStopped(this)) {
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        MR::shakeCameraWeak();

        if (mInfo != nullptr) {
            setNerve(&NrvCapsuleCage::CapsuleCageNrvEndCamera::sInstance);
        } else {
            kill();
        }
    }
}

void CapsuleCage::kill() {
    if (mInfo != nullptr) {
        MR::endActorCamera(this, mInfo, false, -1);
        MR::endDemo(this, ::cDemoCameraName);
    }

    MapObjActor::kill();
}

void CapsuleCage::connectToScene(const MapObjActorInitInfo& rInfo) {
    MR::connectToSceneIndirectMapObj(this);
}

void CapsuleCage::initCaseUseSwitchB(const MapObjActorInitInfo& rInfo) {
    MR::listenStageSwitchOnB(this, MR::Functor_Inline(this, &CapsuleCage::startOpen));
}

void CapsuleCage::startOpen() {
    MR::invalidateClipping(this);

    if (mInfo != nullptr) {
        MR::requestStartDemoWithoutCinemaFrame(this, ::cDemoCameraName, &NrvCapsuleCage::CapsuleCageNrvStartCamera::sInstance, nullptr);
    } else {
        setNerve(&NrvCapsuleCage::CapsuleCageNrvOpen::sInstance);
    }
}

void CapsuleCage::exeEndCamera() {
    if (MR::isStep(this, ::sStepForEndCamera)) {
        kill();
    }
}

void CapsuleCage::initCaseNoUseSwitchB(const MapObjActorInitInfo&) {
}

void CapsuleCage::initCaseNoUseSwitchA(const MapObjActorInitInfo&) {
}

void CapsuleCage::initCaseUseSwitchA(const MapObjActorInitInfo&) {
}

CapsuleCage::~CapsuleCage() {
}
