#include "Game/Screen/HeatHazeEffect.hpp"
#include "Game/AreaObj/HazeCube.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

HeatHazeEffect::HeatHazeEffect(const char* pName) : LiveActor(pName), _8C(1500.0f) {
}

void HeatHazeEffect::appear() {
    LiveActor::appear();
    MR::startBtk(this, "ShimmerBoard");
}

// HeatHazeEffect::control

HeatHazeDirector::HeatHazeDirector(const char* pName) : NameObj(pName) {
}

void HeatHazeEffect::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("ShimmerBoard", nullptr, false);
    MR::connectToSceneIndirectMapObj(this);
    MR::invalidateClipping(this);
    makeActorDead();
}

void HeatHazeDirector::init(const JMapInfoIter& rIter) {
    mEffect = new HeatHazeEffect("陽炎");
    mEffect->initWithoutIter();

    MR::connectToSceneMapObjMovement(this);
}

void HeatHazeDirector::movement() {
    HazeCube* pHazeCube = static_cast< HazeCube* >(MR::getAreaObj("HazeCube", *MR::getPlayerPos()));

    if (pHazeCube != nullptr) {
        if (MR::isDead(mEffect)) {
            mEffect->_8C = pHazeCube->mDistance;
            mEffect->appear();
        }
    } else if (!MR::isDead(mEffect)) {
        mEffect->kill();
    }
}
