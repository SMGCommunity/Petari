#include "Game/Map/Sky.hpp"
#include "Game/LiveActor/MirrorReflectionModel.hpp"
#include "Game/Map/SpaceInner.hpp"

namespace NrvSky {
    NEW_NERVE(HostTypeWait, Sky, Wait);
    NEW_NERVE(HostTypeChange, Sky, Change);
};  // namespace NrvSky

namespace {
    const char* cChangeAnimName = "Change";
}

Sky::Sky(const char* pSkyName) : LiveActor(pSkyName) {
    mSpaceInner = 0;
    mReflectionModel = 0;
}

void Sky::init(const JMapInfoIter& rIter) {
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
            MR::listenStageSwitchOnOffB(this, MR::Functor(this, &Sky::appearSpaceInner), MR::Functor(this, &Sky::disappearSpaceInner));
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
    } else {
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

void Sky::initModel(const char* pModelName) {
    initModelManagerWithAnm(pModelName, 0, false);
}

void Sky::control() {
    if (mSpaceInner && MR::isValidSwitchB(this)) {
        if (MR::isDead(mSpaceInner)) {
            MR::showModelIfHidden(this);
        } else {
            if (mSpaceInner->isAppeared()) {
                MR::hideModelIfShown(this);
            } else {
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

void Sky::exeWait() {
    if (MR::isValidSwitchA(this) && MR::isOnSwitchA(this)) {
        setNerve(&NrvSky::HostTypeChange::sInstance);
    }
}

void Sky::exeChange() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, cChangeAnimName);
    }
}

ProjectionMapSky::ProjectionMapSky(const char* pSkyName) : Sky(pSkyName) {
    mMtxSetter = 0;
}

Sky::~Sky() {}

void ProjectionMapSky::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();

    if (mMtxSetter) {
        mMtxSetter->updateMtxUseBaseMtx();
    }
}

void ProjectionMapSky::initModel(const char* pName) {
    initModelManagerWithAnm(pName, 0, true);
    mMtxSetter = MR::initDLMakerProjmapEffectMtxSetter(this);
    MR::newDifferedDLBuffer(this);
    mMtxSetter->updateMtxUseBaseMtx();
}

ProjectionMapSky::~ProjectionMapSky() {}
