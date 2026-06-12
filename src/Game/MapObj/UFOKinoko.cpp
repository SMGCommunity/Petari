#include "Game/MapObj/UFOKinoko.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/MapObj/StageEffectDataTable.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"

namespace NrvUFOKinoko {
    NEW_NERVE(HostTypeWait, UFOKinoko, Wait)
    NEW_NERVE(HostTypeMove, UFOKinoko, Move)
    NEW_NERVE(HostTypeStop, UFOKinoko, Stop)
};  // namespace NrvUFOKinoko

UFOKinoko::UFOKinoko(const char* pName) : MapObjActor(pName) {
}

void UFOKinoko::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    bool arg7 = false;
    MR::getJMapInfoArg7NoInit(rIter, &arg7);
    if (arg7) {
        pArchiveList->addArchive("UFOKinokoLow");
    }
}

UFOKinoko::~UFOKinoko() {
}

void UFOKinoko::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    s32 argShadowType = -1;
    MR::getMapPartsArgShadowType(&argShadowType, rIter);
    bool hasShadow = !MR::isMapPartsShadowTypeNone(argShadowType);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(nullptr);
    info.setupSound(4);
    info.setupGroupClipping(64);
    info.setupSeAppear();
    info.setupRailMover();
    info.setupRotator();
    info.setupBaseMtxFollowTarget();
    info.setupNerve(&NrvUFOKinoko::HostTypeWait::sInstance);
    if (hasShadow) {
        info.setupShadow(nullptr);
    }
    MapObjActorUtil::setupInitInfoColorChangeArg0(&info, rIter);
    bool arg7 = true;
    MR::getJMapInfoArg7NoInit(rIter, &arg7);
    if (arg7) {
        info.setupNoUseLodCtrl();
    }
    initialize(rIter, info);
    MapObjActorUtil::startRotator(this);
    if (MR::isDemoCast(this, nullptr)) {
        MR::tryRegisterDemoActionFunctor(this, MR::Functor(this, &UFOKinoko::startMove), "ＵＦＯキノコ登場");
    }
}

void UFOKinoko::control() {
    MapObjActor::control();
    MR::StageEffect::tryStageEffectMoving(this, mObjectName);
}

void UFOKinoko::initCaseUseSwitchB(const MapObjActorInitInfo&) {
    MR::listenStageSwitchOnB(this, MR::Functor(this, &UFOKinoko::startMove));
}

void UFOKinoko::initCaseNoUseSwitchB(const MapObjActorInitInfo&) {
}

void UFOKinoko::startMove() {
    if (mRailMover != nullptr) {
        MapObjActorUtil::startRailMover(this);
    }
    setNerve(&NrvUFOKinoko::HostTypeMove::sInstance);
}

void UFOKinoko::exeWait() {
}

void UFOKinoko::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::StageEffect::tryStageEffectStart(this, mObjectName);
        MR::startSound(this, "SE_OJ_UFO_KINOKO_MOVE_ST");
    }
    MR::startLevelSound(this, "SE_OJ_LV_UFO_KINOKO_MOVE");
    if (mRailMover != nullptr && !MapObjActorUtil::isRailMoverWorking(this)) {
        setNerve(&NrvUFOKinoko::HostTypeStop::sInstance);
    }
}

void UFOKinoko::exeStop() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_UFO_KINOKO_MOVE_ED");
        MR::StageEffect::tryStageEffectStop(this, mObjectName);
    }
}
