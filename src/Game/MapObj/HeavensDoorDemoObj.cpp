#include "Game/MapObj/HeavensDoorDemoObj.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/StageEffectDataTable.hpp"
#include "Game/Util/Functor.hpp"

HeavensDoorDemoObj::HeavensDoorDemoObj(const char* pName) : MapObjActor(pName) {
    _C4 = false;
}

HeavensDoorDemoObj::~HeavensDoorDemoObj() {}

void HeavensDoorDemoObj::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    if (MapObjActor::isObjectName("HeavensDoorInsideCage") || MapObjActor::isObjectName("HeavensDoorAppearStepA")) {
        info.setupProjmapMtx(false);
    }
    if (MapObjActor::isObjectName("HeavensDoorAppearStepA")) {
        info.setupNerve(&NrvHeavensDoorDemoObj::HeavensDoorDemoObjNrvAppear::sInstance);
    } else {
        info.setupNerve(&NrvHeavensDoorDemoObj::HeavensDoorDemoObjNrvWait::sInstance);
    }
    MapObjActorUtil::setupInitInfoTypical(&info, mObjectName);
    MapObjActor::initialize(rIter, info);
    if (MapObjActor::isObjectName("HeavensDoorInsidePlanetPartsA")) {
        MR::registerDemoSimpleCastAll(this);
    }
    if (MR::isDemoCast(this, nullptr)) {
        MR::tryRegisterDemoActionNerve(this, &NrvHeavensDoorDemoObj::HeavensDoorDemoObjNrvVanish::sInstance, nullptr);
        if (MapObjActor::isObjectName("HeavensDoorInsideCage")) {
            MR::tryRegisterDemoActionFunctor(this, MR::Functor_Inline(this, &HeavensDoorDemoObj::startInsideCageDemo), "消滅");
        }
    }
}

void HeavensDoorDemoObj::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "Appear");
        MR::StageEffect::tryStageEffectStart(this, mObjectName);
        MR::StageEffect::shakeCameraMoving(this, mObjectName);
        if (MapObjActor::isObjectName("HeavensDoorAppearStepA")) {
            MR::startStageBGM("MBGM_GALAXY_24", false);
            MR::startSubBGM("BGM_APPEAR_HD_TOWER", false);
        }
    }
    MR::StageEffect::tryStageEffectMoving(this, mObjectName);
    if (MR::isBtkStopped(this)) {
        MR::StageEffect::tryStageEffectStop(this, mObjectName);
        MR::StageEffect::stopShakingCameraMoving(this, mObjectName);
        setNerve(&NrvHeavensDoorDemoObj::HeavensDoorDemoObjNrvWait::sInstance);
    }
}

void HeavensDoorDemoObj::exeVanish() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "Move");
        if (MapObjActor::isObjectName("HeavensDoorInsideCage")) {
            MR::emitEffect(this, "Break");
        }
        MR::StageEffect::tryStageEffectStart(this, mObjectName);
        if (MapObjActor::isObjectName("HeavensDoorInsidePlanetPartsA")) {
            MR::stopStageBGM(75);
        }
    }
    MR::StageEffect::tryStageEffectMoving(this, mObjectName);
    if (MR::isBtkStopped(this)) {
        if (MapObjActor::isObjectName("HeavensDoorInsidePlanetPartsA")) {
            MR::startStageBGM("STM_STAR_EXIST_2", false);
        }
        MR::StageEffect::tryStageEffectStop(this, mObjectName);
        MR::StageEffect::stopShakingCameraMoving(this, mObjectName);
        kill();
    }
}

void HeavensDoorDemoObj::kill() {
    _C4 = false;
    MapObjActor::kill();
}

void HeavensDoorDemoObj::control() {
    if (_C4) {
        MR::tryRumblePadWeak(this, 0);
    }
}

void HeavensDoorDemoObj::connectToScene(const MapObjActorInitInfo& info) {
    if (MapObjActor::isObjectName("HeavensDoorInsideCage")) {
        MR::connectToSceneIndirectMapObj(this);
    } else {
        MapObjActor::connectToScene(info);
    }
}

void HeavensDoorDemoObj::startInsideCageDemo() {
    MR::StageEffect::shakeCameraMoving(this, mObjectName);
    _C4 = true;
}

namespace NrvHeavensDoorDemoObj {
    INIT_NERVE(HeavensDoorDemoObjNrvAppear);
    INIT_NERVE(HeavensDoorDemoObjNrvWait);
    INIT_NERVE(HeavensDoorDemoObjNrvVanish);

    void HeavensDoorDemoObjNrvAppear::execute(Spine* pSpine) const {
        HeavensDoorDemoObj* door = reinterpret_cast< HeavensDoorDemoObj* >(pSpine->mExecutor);
        door->exeAppear();
    }

    void HeavensDoorDemoObjNrvWait::execute(Spine* pSpine) const {
        HeavensDoorDemoObj* door = reinterpret_cast< HeavensDoorDemoObj* >(pSpine->mExecutor);
        door->exeWait();
    }

    void HeavensDoorDemoObjNrvVanish::execute(Spine* pSpine) const {
        HeavensDoorDemoObj* door = reinterpret_cast< HeavensDoorDemoObj* >(pSpine->mExecutor);
        door->exeVanish();
    }
};  // namespace NrvHeavensDoorDemoObj
