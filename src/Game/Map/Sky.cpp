#include "Game/Map/Sky.hpp"
#include "Game/LiveActor/MirrorReflectionModel.hpp"
#include "Game/Map/SpaceInner.hpp"

NrvSky::HostTypeWait NrvSky::HostTypeWait::sInstance;
NrvSky::HostTypeChange NrvSky::HostTypeChange::sInstance;

Sky::Sky(const char *pSkyName) : LiveActor(pSkyName) {
    mSpaceInner = 0;
    mReflectionModel = 0;
}

void Sky::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    const char* objectName = 0;
    MR::getObjectName(&objectName, rIter);
    initModel(objectName);
    MR::connectToSceneSky(this);
    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchReadB(this, rIter);
    MR::useStageSwitchReadAppear(this, rIter);

    if (MR::isEqualString(objectName, "SummerSky")) {
        mSpaceInner = new SpaceInner("内側宇宙");
        mSpaceInner->initWithoutIter();

        if (MR::isValidSwitchB(this)) {
            void (Sky::*d)(void) = &Sky::disappearSpaceInner;
            void (Sky::*a)(void) = &Sky::appearSpaceInner;
            MR::listenStageSwitchOnOffB(this, MR::Functor(this, d), MR::Functor(this, a));
        } 
    }

    s32 arg = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg);

    if (!arg) {
        mReflectionModel = new MirrorReflectionModel(this, "鏡内モデル", objectName, getBaseMtx());
        mReflectionModel->initWithoutIter();
    }

    MR::tryStartAllAnim(this, objectName);

    if (mReflectionModel) {
        MR::tryStartAllAnim(mReflectionModel, objectName);
    }

    MR::invalidateClipping(this);
    MR::registerDemoSimpleCastAll(this);
    initNerve(&NrvSky::HostTypeWait::sInstance);

    if (MR::isValidSwitchAppear(this)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }
    else {
        makeActorAppeared();
    }
}

void Sky::calcAnim() {
    TVec3f pos = MR::getCamPos();
    mPosition.x = pos.x;
    mPosition.y = pos.y;
    mPosition.z = pos.z;
    LiveActor::calcAnim();
}

void Sky::initModel(const char *pModelName) {
    initModelManagerWithAnm(pModelName, 0, false);
}

void Sky::control() {
    if (mSpaceInner && MR::isValidSwitchB(this)) {
        if (MR::isDead(mSpaceInner)) {
            MR::showModelIfHidden(this);
        }
        else {
            if (mSpaceInner->isAppeared()) {
                MR::hideModelIfHidden(this);
            }
            else {
                MR::showModelIfHidden(this);
            }
        }
    }
}

void Sky::appearSpaceInner() {
    if (mSpaceInner && MR::isValidSwitchB(this)) {
        mSpaceInner->appear();
    }
}

void Sky::disappearSpaceInner() {
    if (mSpaceInner && MR::isValidSwitchB(this)) {
        mSpaceInner->disappear();
    }
}

ProjectionMapSky::ProjectionMapSky(const char *pSkyName) : Sky(pSkyName) {
    mMtxSetter = 0;
}

Sky::~Sky() {

}

void ProjectionMapSky::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();

    if (mMtxSetter) {
        mMtxSetter->updateMtxUseBaseMtx();
    }
}

void ProjectionMapSky::initModel(const char *pName) {
    initModelManagerWithAnm(pName, 0, true);
    mMtxSetter = MR::initDLMakerProjmapEffectMtxSetter(this);
    MR::newDifferedDLBuffer(this);
    mMtxSetter->updateMtxUseBaseMtx();
}

namespace NrvSky {
    void HostTypeChange::execute(Spine *pSpine) const {
        Sky* sky = reinterpret_cast<Sky*>(pSpine->mExecutor);

        if (MR::isFirstStep(sky)) {
            MR::startAllAnim(sky, cChangeAnimName);
        }
    }

    void HostTypeWait::execute(Spine *pSpine) const {
        Sky* sky = reinterpret_cast<Sky*>(pSpine->mExecutor);

        if (MR::isValidSwitchA(sky) && MR::isOnSwitchA(sky)) {
            sky->setNerve(&NrvSky::HostTypeChange::sInstance);
        }
    }
};

ProjectionMapSky::~ProjectionMapSky() {

}
