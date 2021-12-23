#include "Game/LiveActor/ShadowDrawer.h"

ShadowDrawer::ShadowDrawer(const char *pName) : NameObj(pName) {
    mController = 0;
}

ShadowController* ShadowDrawer::getController() const {
    return mController;
}

void ShadowDrawer::setShadowController(ShadowController *pController) {
    mController = pController;
}

ShadowDrawer::~ShadowDrawer() {
    
}