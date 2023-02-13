#include "Game/MapObj/RotateMoveObj.h"
#include "Game/MapObj/StageEffectDataTable.h"
#include "Game/MapObj/MapPartsRotator.h"

void RotateMoveObj::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupRotator();
    info.setupBaseMtxFollowTarget();
    info.setupNerve(&NrvMapObjActor::HostTypeWait::sInstance);
    MapObjActorUtil::setupInitInfoColorChangeArg0(&info, rIter);
    MapObjActorUtil::setupInitInfoTextureChangeArg1(&info, rIter);
    MapObjActorUtil::setupInitInfoTypical(&info, mObjectName);
    initialize(rIter, info);

    bool v5 = true;
    s32 condType = 0;
    MR::getMapPartsArgMoveConditionType(&condType, rIter);

    if (!MR::isMoveStartTypeUnconditional(condType)) {
        setNerve(&NrvRotateMoveObj::HostTypeWaitForPlayerOn::sInstance);
        v5 = false;
    }

    if (MR::isDemoCast(this, nullptr)) {
        MR::FunctorV0M<RotateMoveObj *, void (RotateMoveObj::*)()> setStateFunc = MR::Functor<RotateMoveObj>(this, &RotateMoveObj::setStateMove);

        if (MR::tryRegisterDemoActionFunctor(this, setStateFunc, nullptr)) {
            v5 = false;
        }
    }

    if (MR::isValidSwitchB(this)) {
        v5 = false;
    }

    if (v5) {
        startMapPartsFunctions();
        setNerve(&NrvRotateMoveObj::HostTypeMove::sInstance);
    }
}

/*
void RotateMoveObj::initCaseUseSwitchB(const MapObjActorInitInfo &rInfo) {
    MR::FunctorV0M<RotateMoveObj *, void (RotateMoveObj::*)()> setFunc = MR::Functor<RotateMoveObj>(this, &RotateMoveObj::setStateMove);
    MR::listenStageSwitchOnOffB(this, MR::FunctorV0M<RotateMoveObj *, void (RotateMoveObj::*)(void)>(this, &RotateMoveObj::pauseMapPartsFunctions), setFunc);
}*/

void RotateMoveObj::initCaseNoUseSwitchB(const MapObjActorInitInfo &rInfo) {

}

void RotateMoveObj::setStateMove() {
    if (MR::isRegisteredEffect(this, "Appear")) {
        MR::emitEffect(this, "Appear");
    }

    startMapPartsFunctions();
    setNerve(&NrvMapObjActor::HostTypeMove::sInstance);
}

void RotateMoveObj::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::StageEffect::tryStageEffectStart(this, mObjectName);
    }

    if (!MapObjActorUtil::isRotatorMoving(this)) {
        setNerve(&NrvRotateMoveObj::HostTypeStop::sInstance);
    }
    else {
        if (mRotator->_14) {
            if (MR::isEqualString(mObjectName, "OceanRingRuinsGearBig") || MR::isEqualString(mObjectName, "OceanRingRuinsGearSmall")) {
                if (mRotator->getRotateSpeed() != 0.0f) {
                    MR::StageEffect::tryStageEffectMoving(this, mObjectName);
                }
            }
            else {
                MR::StageEffect::tryStageEffectMoving(this, mObjectName);
            }
        }
    }
}

void RotateMoveObj::exeStop() {
    if (MapObjActorUtil::isRotatorMoving(this)) {
        setNerve(&NrvRotateMoveObj::HostTypeMove::sInstance);
    }
    else if (MR::isFirstStep(this)) {
        MR::StageEffect::tryStageEffectStop(this, mObjectName);
    }
}

void RotateMoveObj::exeWaitForPlayerOn() {
    if (MR::isOnPlayer(this)) {
        setStateMove();
    }
}

namespace NrvRotateMoveObj {
    INIT_NERVE(HostTypeWaitForPlayerOn);
    INIT_NERVE(HostTypeWait);
    INIT_NERVE(HostTypeMove);
    INIT_NERVE(HostTypeStop);
};

RotateMoveObj::~RotateMoveObj() {

}
 