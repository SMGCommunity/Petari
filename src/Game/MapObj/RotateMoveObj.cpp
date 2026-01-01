#include "Game/MapObj/RotateMoveObj.hpp"
#include "Game/MapObj/MapPartsRotator.hpp"
#include "Game/MapObj/StageEffectDataTable.hpp"

namespace NrvRotateMoveObj {
    NEW_NERVE(HostTypeWaitForPlayerOn, RotateMoveObj, WaitForPlayerOn);
    NEW_NERVE(HostTypeWait, RotateMoveObj, Wait);
    NEW_NERVE(HostTypeMove, RotateMoveObj, Move);
    NEW_NERVE(HostTypeStop, RotateMoveObj, Stop);
};  // namespace NrvRotateMoveObj

void RotateMoveObj::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupRotator();
    info.setupBaseMtxFollowTarget();
    info.setupNerve(&NrvRotateMoveObj::HostTypeWait::sInstance);
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
        if (MR::tryRegisterDemoActionFunctor(this, MR::Functor< RotateMoveObj >(this, &RotateMoveObj::setStateMove), nullptr)) {
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

void RotateMoveObj::initCaseUseSwitchB(const MapObjActorInitInfo& rInfo) {
    MR::listenStageSwitchOnOffB(this, MR::Functor< RotateMoveObj >(this, &RotateMoveObj::setStateMove),
                                MR::Functor_Inline< MapObjActor >(this, &MapObjActor::pauseMapPartsFunctions));
}

void RotateMoveObj::initCaseNoUseSwitchB(const MapObjActorInitInfo& rInfo) {}

void RotateMoveObj::setStateMove() {
    if (MR::isRegisteredEffect(this, "Appear")) {
        MR::emitEffect(this, "Appear");
    }

    startMapPartsFunctions();
    setNerve(&NrvRotateMoveObj::HostTypeMove::sInstance);
}

void RotateMoveObj::exeWaitForPlayerOn() {
    if (MR::isOnPlayer(this)) {
        setStateMove();
    }
}

void RotateMoveObj::exeWait() {}

void RotateMoveObj::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::StageEffect::tryStageEffectStart(this, mObjectName);
    }

    if (!MapObjActorUtil::isRotatorMoving(this)) {
        setNerve(&NrvRotateMoveObj::HostTypeStop::sInstance);
    } else {
        if (mRotator->_14) {
            if (MR::isEqualString(mObjectName, "OceanRingRuinsGearBig") || MR::isEqualString(mObjectName, "OceanRingRuinsGearSmall")) {
                if (mRotator->getRotateSpeed() != 0.0f) {
                    MR::StageEffect::tryStageEffectMoving(this, mObjectName);
                }
            } else {
                MR::StageEffect::tryStageEffectMoving(this, mObjectName);
            }
        }
    }
}

void RotateMoveObj::exeStop() {
    if (MapObjActorUtil::isRotatorMoving(this)) {
        setNerve(&NrvRotateMoveObj::HostTypeMove::sInstance);
    } else if (MR::isFirstStep(this)) {
        MR::StageEffect::tryStageEffectStop(this, mObjectName);
    }
}
