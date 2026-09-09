#include "Game/Map/Sky.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/LiveActor/MirrorReflectionModel.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/SpaceInner.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    const char* cChangeAnimName = "Change";
};  // namespace

namespace NrvSky {
    NEW_NERVE(HostTypeWait, Sky, Wait);
    NEW_NERVE(HostTypeChange, Sky, Change);
};  // namespace NrvSky

Sky::Sky(const char* pName) : LiveActor(pName), mSpaceInner(), mReflectionModel() {
}

void Sky::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    const char* objName = nullptr;
    MR::getObjectName(&objName, rIter);

    initModel(objName);
    MR::connectToSceneSky(this);
    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchReadB(this, rIter);
    MR::useStageSwitchReadAppear(this, rIter);

    if (MR::isEqualString(objName, "SummerSky")) {
        mSpaceInner = new SpaceInner("内側宇宙");
        mSpaceInner->initWithoutIter();

        if (MR::isValidSwitchB(this)) {
            MR::listenStageSwitchOnOffB(this, MR::Functor(this, &Sky::appearSpaceInner), MR::Functor(this, &Sky::disappearSpaceInner));
        }
    }

    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);

    if (arg0 == 0) {
        mReflectionModel = new MirrorReflectionModel(this, "鏡内モデル", objName, getBaseMtx());
        mReflectionModel->initWithoutIter();
    }

    MR::tryStartAllAnim(this, objName);

    if (mReflectionModel) {
        MR::tryStartAllAnim(mReflectionModel, objName);
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
    mPosition.set(MR::getCamPos());
    LiveActor::calcAnim();
}

void Sky::initModel(const char* pModelName) {
    initModelManagerWithAnm(pModelName, nullptr, false);
}

void Sky::control() {
    if (mSpaceInner != nullptr && MR::isValidSwitchB(this)) {
        if (MR::isDead(mSpaceInner)) {
            MR::showModelIfHidden(this);
        } else if (mSpaceInner->isAppeared()) {
            MR::hideModelIfShown(this);
        } else {
            MR::showModelIfHidden(this);
        }
    }
}

void Sky::appearSpaceInner() {
    if (mSpaceInner != nullptr && MR::isValidSwitchB(this)) {
        mSpaceInner->appear();
    }
}

void Sky::disappearSpaceInner() {
    if (mSpaceInner != nullptr && MR::isValidSwitchB(this)) {
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
        MR::startAllAnim(this, ::cChangeAnimName);
    }
}

ProjectionMapSky::ProjectionMapSky(const char* pSkyName) : Sky(pSkyName), mProjmapEffectMtxSetter() {
}

void ProjectionMapSky::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();

    if (mProjmapEffectMtxSetter != nullptr) {
        mProjmapEffectMtxSetter->updateMtxUseBaseMtx();
    }
}

void ProjectionMapSky::initModel(const char* pName) {
    initModelManagerWithAnm(pName, 0, true);

    mProjmapEffectMtxSetter = MR::initDLMakerProjmapEffectMtxSetter(this);
    MR::newDifferedDLBuffer(this);
    mProjmapEffectMtxSetter->updateMtxUseBaseMtx();
}
