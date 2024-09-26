#include "Game/AreaObj/BgmProhibitArea.hpp"

BgmProhibitArea::~BgmProhibitArea() {

}

namespace {
    BgmMuteSet* findDataElement(const char *pName) {
        for (u32 i = 0; i < 2; i++) {
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

void BgmProhibitArea::movement() {
    if (!MR::isStageStateScenarioOpeningCamera()) {
        TVec3f stack_8;
        stack_8.setInlinePS(*MR::getPlayerPos());
        f32 dist = PSVECDistance(stack_8.toCVec(), _40.toCVec());
        
        if (isInVolume(*MR::getPlayerPos())) {
            _3D = 0;

            if (!_3C && MR::isPlayingStageBgm()) {
                BgmMuteSet* set = findDataElement(MR::getCurrentStageName());
                s32 v10 = set ? set->_4 : -1;
                if (v10 >= 0) {
                    if (dist >= 10000.0f) {
                        MR::setStageBGMState(v10, 0);
                    }
                    else {
                        MR::setStageBGMState(v10, 120);
                    }
                }

                _3C = 1;
            }
        }
        else {
            _3C = 0;
            if (!_3D && MR::isPlayingStageBgm()) {
                BgmMuteSet* set = findDataElement(MR::getCurrentStageName());
                s32 v14 = set ? set->_8 : -1;

                if (v14 >= 0) {
                    MR::setStageBGMState(v14, 120);
                }

                _3D = 1;
            }
        }

        _40.setInlinePS(stack_8);
    }
}

const char* BgmProhibitArea::getManagerName() const {
    return "BgmProhibitArea";
}