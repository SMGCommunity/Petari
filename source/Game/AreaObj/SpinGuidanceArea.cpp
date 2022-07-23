#include "Game/AreaObj/SpinGuidanceArea.h"
#include "Game/Scene/SceneObjHolder.h"
#include "Game/Screen/PlayerActionGuidance.h"

SpinGuidanceArea::SpinGuidanceArea(int type, const char *pName) : AreaObj(type, pName) {

}

SpinGuidanceArea::~SpinGuidanceArea() {

}

void SpinGuidanceArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::createSceneObj(SceneObj_PlayerActionGuidance);

    const char *objName = NULL;
    MR::getObjectName(&objName, rIter);

    if (MR::isEqualString(objName, "SpinGuidanceCube")) {
        MR::getPlayerActionGuidance()->createSpinLayout();
    }
    else if (MR::isEqualString(objName, "TamakoroMoveGuidanceCube") || MR::isEqualString(objName, "TamakoroJumpGuidanceCube")) {
        MR::getPlayerActionGuidance()->createTamakoroLayout();
    }
}