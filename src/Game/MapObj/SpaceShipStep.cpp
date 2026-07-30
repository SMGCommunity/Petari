#include "Game/MapObj/SpaceShipStep.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util.hpp"

namespace NrvSpaceShipStep {
    NEW_NERVE(SpaceShipStepNrvWait, SpaceShipStep, Wait);
    NEW_NERVE(SpaceShipStepNrvMoveSign, SpaceShipStep, MoveSign);
    NEW_NERVE(SpaceShipStepNrvMove, SpaceShipStep, Move);
    NEW_NERVE(SpaceShipStepNrvBreak, SpaceShipStep, Break);
};  // namespace NrvSpaceShipStep

SpaceShipStep::SpaceShipStep(const char* pName) : MapObjActor(pName), _C4(0.0f, 0.0f, 0.0f), _D0(), _D4() {
}

void SpaceShipStep::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(nullptr);
    info.setupSound(4);
    info.setupRailMover();
    info.setupBaseMtxFollowTarget();
    info.setupNerve(GET_NERVE(SpaceShipStep, SpaceShipStepNrvWait));
    initialize(rIter, info);
    f32 radius = 0.0f;
    MR::calcModelBoundingRadius(&radius, this);
    MR::initAndSetRailClipping(&_C4, this, 100.0f, radius);
}

void SpaceShipStep::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Wait", nullptr);
        MR::validateClipping(this);
    }

    if (MR::isOnPlayer(this)) {
        setNerve(GET_NERVE(SpaceShipStep, SpaceShipStepNrvMoveSign));
    }
}

void SpaceShipStep::exeMoveSign() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "OnMario", nullptr);
        MR::startSound(this, "SE_OJ_SPACE_SHIP_STEP_ST");
        MR::invalidateClipping(this);
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(SpaceShipStep, SpaceShipStepNrvMove));
    }
}

void SpaceShipStep::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Wait", nullptr);
        MapObjActorUtil::startAllMapPartsFunctions(this);
    }

    MR::startLevelSound(this, "SE_OJ_LV_SPACE_SHIP_STEP_MV");
}

void SpaceShipStep::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Break", nullptr);
        MR::startSound(this, "SE_OJ_SPACE_SHIP_STEP_BREAK");
        MR::startSound(this, "SE_OJ_SPACE_SHIP_STEP_ED");
    }
}

void SpaceShipStep::control() {
    MapObjActor::control();
    updateResetToInitPosCheck();
}

bool SpaceShipStep::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(SpaceShipStep, SpaceShipStepNrvBreak))) {
        return false;
    }

    if (MR::isMsgExplosionAttack(msg)) {
        setNerve(GET_NERVE(SpaceShipStep, SpaceShipStepNrvBreak));
        return true;
    }

    return false;
}

void SpaceShipStep::updateResetToInitPosCheck() {
    if (!_D4) {
        _D4 = MR::isJudgedToClipFrustum(mPosition, 700.0f) == false;
    } else {
        if (isNerve(GET_NERVE(SpaceShipStep, SpaceShipStepNrvWait)) || !MR::isJudgedToClipFrustum(mPosition, 700.0f)) {
            _D0 = 0;
        } else {
            if (_D0 < 240) {
                _D0++;
            } else {
                MapObjActorUtil::resetRailMoverToInitPos(this);

                if (!MR::isJudgedToClipFrustum(mPosition, 700.0f) && MR::isNearPlayer(this, 3000.0f)) {
                    MR::emitEffect(this, "Appear");
                    MR::startSound(this, "SE_OJ_SPACE_SHIP_STEP_RESET");
                }

                if (MR::isNearPlayer(this, 1000.0f)) {
                    setNerve(GET_NERVE(SpaceShipStep, SpaceShipStepNrvWait));
                } else {
                    setNerve(GET_NERVE(SpaceShipStep, SpaceShipStepNrvMove));
                }

                _D0 = 0;
                _D4 = 0;
            }
        }
    }
}

SpaceShipStep::~SpaceShipStep() {
}
