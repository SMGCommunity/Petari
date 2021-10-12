#include "Game/AreaObj/BgmProhibitArea.h"

namespace {
    BgmMuteSet* findDataElement(const char *pName) {
        for (int i = 0; i < 2; i++) {
            BgmMuteSet* ret = &sBgmMuteSet[i];

            if (MR::isEqualString(pName, ret->mGalaxyName)) {
                return ret;
            }
        }

        return 0;
    }
};

BgmProhibitArea::BgmProhibitArea(int a1, const char *pName) : AreaObj(a1, pName) {
    _3C = 0;
    _3D = 0;
    _40.x = 0.0f;
    _40.y = 0.0f;
    _40.z = 0.0f;
}

void BgmProhibitArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
}

/*
void BgmProhibitArea::movement() {
    if (!MR::isStageStateScenarioOpeningCamera()) {
        register TVec3f* playerPos = MR::getPlayerPos();
    }
}
*/

const char* BgmProhibitArea::getManagerName() const {
    return "BgmProhibitArea";
}