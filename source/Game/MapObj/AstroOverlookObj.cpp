#include "Game/MapObj/AstroOverlookObj.h"

AstroOverlookObj::AstroOverlookObj(const char *pName) : LiveActor(pName) {
    _8C = 0;
    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);
    makeActorAppeared();
}

void AstroOverlookObj::control() {
    if (MR::getCurrentAstroOverlookAreaObj()) {
        MR::deactivateDefaultGameLayout();
        _8C = 1;
    }
    else {
        if (_8C == 1) {
            MR::activateDefaultGameLayout();
            _8C = 0;
        }
    }
}

AstroOverlookObj::~AstroOverlookObj() {
    
}