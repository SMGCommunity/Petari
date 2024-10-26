#include "Game/MapObj/UFOKinoko.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/MapObj/StageEffectDataTable.hpp"

UFOKinoko::UFOKinoko(const char *pName) : MapObjActor(pName) {

}

UFOKinoko::~UFOKinoko() {

}

void UFOKinoko::init(const JMapInfoIter &rIter) {
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
        MR::FunctorV0M<UFOKinoko *, void (UFOKinoko::*)()> functor = MR::Functor<UFOKinoko>(this, &UFOKinoko::startMove);
        MR::tryRegisterDemoActionFunctor(this, functor, "ＵＦＯキノコ登場");
    }
}

void UFOKinoko::control() {
    MapObjActor::control();
    MR::StageEffect::tryStageEffectMoving(this, mObjectName);
}

void UFOKinoko::initCaseUseSwitchB(const MapObjActorInitInfo &) {
    MR::FunctorV0M<UFOKinoko *, void (UFOKinoko::*)()> functor = MR::Functor<UFOKinoko>(this, &UFOKinoko::startMove);
    MR::listenStageSwitchOnB(this, functor);
}

void UFOKinoko::initCaseNoUseSwitchB(const MapObjActorInitInfo &) {

}

void UFOKinoko::startMove() {
    if (mRailMover != nullptr) {
        MapObjActorUtil::startRailMover(this);
    }
    setNerve(&NrvUFOKinoko::HostTypeMove::sInstance);
}

void UFOKinoko::makeArchiveList(NameObjArchiveListCollector *pArchiveList, const JMapInfoIter &rIter) {
    bool arg7 = false;
    MR::getJMapInfoArg7NoInit(rIter, &arg7);
    if (arg7) {
        pArchiveList->addArchive("UFOKinokoLow");
    }
}

void UFOKinoko::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::StageEffect::tryStageEffectStart(this, mObjectName);
        MR::startSound(this, "SE_OJ_UFO_KINOKO_MOVE_ST", -1, -1);
    }
    MR::startLevelSound(this, "SE_OJ_LV_UFO_KINOKO_MOVE", -1, -1, -1);
    if (mRailMover != nullptr && !MapObjActorUtil::isRailMoverWorking(this)) {
        setNerve(&NrvUFOKinoko::HostTypeStop::sInstance);
    }
}

void UFOKinoko::exeStop() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_UFO_KINOKO_MOVE_ED", -1, -1);
        MR::StageEffect::tryStageEffectStop(this, mObjectName);
    }
}

namespace NrvUFOKinoko {
    INIT_NERVE(HostTypeWait)
    INIT_NERVE(HostTypeMove)
    INIT_NERVE(HostTypeStop)

    void HostTypeWait::execute(Spine *pSpine) const {

    }
    void HostTypeMove::execute(Spine *pSpine) const {
        UFOKinoko* ufo = reinterpret_cast<UFOKinoko*>(pSpine->mExecutor);
        ufo->exeMove();
    }
    void HostTypeStop::execute(Spine *pSpine) const {
        UFOKinoko* ufo = reinterpret_cast<UFOKinoko*>(pSpine->mExecutor);
        ufo->exeStop();
    }
};
