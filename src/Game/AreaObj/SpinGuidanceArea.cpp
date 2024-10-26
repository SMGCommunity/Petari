#include "Game/AreaObj/SpinGuidanceArea.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/PlayerActionGuidance.hpp"
#include "Game/Util.hpp"

SpinGuidanceArea::SpinGuidanceArea(int type, const char *pName) : AreaObj(type, pName) {

}

SpinGuidanceArea::~SpinGuidanceArea() {

}

void SpinGuidanceArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::createSceneObj(SceneObj_PlayerActionGuidance);

    const char *objName = nullptr;
    MR::getObjectName(&objName, rIter);

    if (MR::isEqualString(objName, "SpinGuidanceCube")) {
        MR::getPlayerActionGuidance()->createSpinLayout();
    }
    else if (MR::isEqualString(objName, "TamakoroMoveGuidanceCube") || MR::isEqualString(objName, "TamakoroJumpGuidanceCube")) {
        MR::getPlayerActionGuidance()->createTamakoroLayout();
    }
}