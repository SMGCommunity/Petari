#include "Game/AreaObj/BgmProhibitArea.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"

BgmProhibitArea::~BgmProhibitArea() {
}

namespace {
    struct BgmMuteSet {
        /* 0x00 */ const char* mGalaxyName;
        /* 0x04 */ u32 _4;
        /* 0x08 */ u32 _8;
    };

    static BgmMuteSet sBgmMuteSet[] = {{"OceanPhantomCaveGalaxy", 7, 1}, {"SoundMapCodeTest", 7, 1}};
};  // namespace

namespace {
    BgmMuteSet* findDataElement(const char* pName) {
        for (u32 i = 0; i < ARRAY_SIZE(::sBgmMuteSet); i++) {
            BgmMuteSet* set = &::sBgmMuteSet[i];

            if (MR::isEqualString(pName, set->mGalaxyName)) {
                return set;
            }
        }

        return nullptr;
    }
};  // namespace

BgmProhibitArea::BgmProhibitArea(int formType, const char* pName) : AreaObj(formType, pName), _3C(), _3D(), _40(0.0f, 0.0f, 0.0f) {
}

void BgmProhibitArea::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
}

void BgmProhibitArea::movement() {
    if (MR::isStageStateScenarioOpeningCamera()) {
        return;
    }

    TVec3f playerPos = *MR::getPlayerPos();
    f32 distToPlayer = playerPos.distance(_40);

    if (isInVolume(*MR::getPlayerPos())) {
        _3D = false;

        if (!_3C && MR::isPlayingStageBgm()) {
            BgmMuteSet* set = ::findDataElement(MR::getCurrentStageName());
            s32 v10 = set ? set->_4 : -1;

            if (v10 >= 0) {
                if (distToPlayer >= 10000.0f) {
                    MR::setStageBGMState(v10, 0);
                } else {
                    MR::setStageBGMState(v10, 120);
                }
            }

            _3C = true;
        }
    } else {
        _3C = false;

        if (!_3D && MR::isPlayingStageBgm()) {
            BgmMuteSet* set = ::findDataElement(MR::getCurrentStageName());
            s32 v14 = set ? set->_8 : -1;

            if (v14 >= 0) {
                MR::setStageBGMState(v14, 120);
            }

            _3D = true;
        }
    }

    _40 = playerPos;
}
