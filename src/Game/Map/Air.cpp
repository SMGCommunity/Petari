#include "Game/Map/Air.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"

Air::Air(const char *pName) : LiveActor(pName) {
    _8C = 0;
    _8D = 0;
    mDistance = 70.0f;
}

void Air::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    const char* object_name = 0;
    MR::getObjectName(&object_name, rIter);
    initModel(object_name);
    MR::connectToSceneAir(this);
    MR::useStageSwitchReadA(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mDistance);
    s32 arg_1 = -1;
    MR::getJMapInfoArg1NoInit(rIter, &arg_1);
    _8D = arg_1 + 1;
    MR::tryStartAllAnim(this, object_name);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    setFarClipping();
    MR::setGroupClipping(this, rIter, 0x10);
    initNerve(&NrvAir::HostTypeOut::sInstance);
    MR::registerDemoSimpleCastAll(this);
    bool registered = MR::tryRegisterDemoCast(this, rIter);

    if (registered) {
        bool val = true;

        void (Air::*fadeOut)(void) = &Air::appearFadeOut;
        bool ret = MR::tryRegisterDemoActionFunctor(this, MR::Functor(this, fadeOut), "消滅");

        if (!ret) {
            void (Air::*fadeIn)(void) = &Air::appearFadeIn;
            ret = MR::tryRegisterDemoActionFunctor(this, MR::Functor(this, fadeIn), 0);

            if (!ret) {
                val = false;
            }
        }

        if (val) {
            makeActorDead();
        }
    }
    else {
        appear();
    }
}

void Air::appear() {
    LiveActor::appear();

    f32 val = 100.0f * mDistance;
    f32 dist = MR::calcDistanceToPlayer(this);

    if (dist < val) {
        MR::tryStartAllAnim(this, "Appear");
        MR::setAllAnimFrameAtEnd(this, "Appear");
        setNerve(&NrvAir::HostTypeIn::sInstance);
    }
    else {
        MR::tryStartAllAnim(this, "Disappear");
        MR::setAllAnimFrameAtEnd(this, "Disappear");
        MR::hideModel(this);
        setNerve(&NrvAir::HostTypeOut::sInstance);
    }
}

bool Air::isDrawing() const {
    if (!MR::isDead(this)) {
        return !MR::isHiddenModel(this);
    }
    
    return false;
}

void Air::initModel(const char *pModelName) {
    initModelManagerWithAnm(pModelName, 0, false);
}

bool Air::tryChange() {
    if (!MR::isValidSwitchA(this)) {
        return false;
    }

    if (_8C) {
        return false;
    }

    if (MR::isOnSwitchA(this)) {
        MR::tryStartAllAnim(this, "Change");
        _8C = 1;
        return true;
    }

    return false;
}

void Air::appearFadeIn() {
    makeActorAppeared();
    MR::tryStartAllAnim(this, "Appear");
    setNerve(&NrvAir::HostTypeIn::sInstance);
}

void Air::appearFadeOut() {
    makeActorAppeared();
    MR::tryStartAllAnim(this, "Disappear");
    setNerve(&NrvAir::HostTypeOut::sInstance);   
}

void Air::exeIn() {
    bool isCameraNotEnd = false;

    if (_8D && !MR::isStartAnimCameraEnd()) {
        isCameraNotEnd = true;
    }

    if (!isCameraNotEnd) {
        tryChange();
        f32 val = (100.0f * (20.0f + mDistance));
        if (val < MR::calcDistanceToPlayer(this)) {
            MR::tryStartAllAnim(this, "Disappear");
            setNerve(&NrvAir::HostTypeOut::sInstance);
        }
    }
}

void Air::exeOut() {
    bool flag = false;

    if (_8D && !MR::isStartAnimCameraEnd()) {
        flag = true;
    }

    if (!flag) {
        tryChange();

        if (!MR::isHiddenModel(this) && MR::isAnyAnimStopped(this, "Disappear")) {
            MR::hideModel(this);
        }

        f32 distMult = (100.0f * mDistance);

        if (MR::calcDistanceToPlayer(this) < distMult) {
            MR::showModel(this);
            MR::tryStartAllAnim(this, "Appear");
            setNerve(&NrvAir::HostTypeIn::sInstance);
        }
    }
}

Air::~Air() {

}

ProjectionMapAir::ProjectionMapAir(const char *pName) : Air(pName) {

}

void ProjectionMapAir::initModel(const char *pModelName) {
    initModelManagerWithAnm(pModelName, nullptr, true);
    ProjmapEffectMtxSetter* mtxSetter = MR::initDLMakerProjmapEffectMtxSetter(this);
    MR::newDifferedDLBuffer(this);
    mtxSetter->updateMtxUseBaseMtx();
}

void Air::setFarClipping() {
    MR::setClippingFarMax(this);
}

AirFar100m::AirFar100m(const char *pName) : Air(pName) {

}

void AirFar100m::setFarClipping() {
    MR::setClippingFar100m(this);
}

PriorDrawAir::PriorDrawAir(const char *pName) : Air(pName) {
    MR::createSceneObj(SceneObj_PriorDrawAirHolder);
    MR::getSceneObj<PriorDrawAirHolder*>(SceneObj_PriorDrawAirHolder)->add(this);
}

PriorDrawAirHolder::PriorDrawAirHolder() : NameObj("先描画大気保持") {
    mAirCount = 0;
}

void PriorDrawAirHolder::add(PriorDrawAir *pAir) {
    s32 airCnt = mAirCount++;
    mAirs[airCnt] = pAir;
}

bool PriorDrawAirHolder::isExistValidDrawAir() const {
    PriorDrawAir** curAir = (PriorDrawAir**)&mAirs;

    for (; curAir != &mAirs[mAirCount]; *curAir++) {
        if ((*curAir)->isDrawing()) {
            return true;
        }
    }

    return false;
}

namespace MR {
    bool isExistPriorDrawAir() {
        if (!MR::isExistSceneObj(SceneObj_PriorDrawAirHolder)) {
            return false;
        }

        return MR::getSceneObj<PriorDrawAirHolder*>(SceneObj_PriorDrawAirHolder)->isExistValidDrawAir();
    }
};

namespace NrvAir {
    INIT_NERVE(HostTypeIn);
    INIT_NERVE(HostTypeOut);
};

AirFar100m::~AirFar100m() {

}

ProjectionMapAir::~ProjectionMapAir() {

}

PriorDrawAir::~PriorDrawAir() {

}

PriorDrawAirHolder::~PriorDrawAirHolder() {

}
